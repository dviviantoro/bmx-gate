#include <boards.h>

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void initEspNow() {
    WiFi.mode(WIFI_STA);
    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_send_cb(OnDataSent);
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
    
    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
}

void initPlayer() {
    mp3_set_serial(SerialPlayer);
    mp3_set_volume(10);

    if (!myPlayer.begin(SerialPlayer, /*isACK = */true, /*doReset = */true)) {
        Serial.println(F("Unable to begin:"));
        Serial.println(F("1. Please recheck the connection!"));
        Serial.println(F("2. Please insert the SD card!"));
        while(true);
    }
    
    myPlayer.setTimeOut(500);
    myPlayer.volume(10);
    myPlayer.outputDevice(DFPLAYER_DEVICE_SD);
    Serial.println(F("DFPlayer Mini online."));
}

void setup() {
    initBoard();
    Serial.println("disini1");
    initPlayer();
    initEspNow();
    digitalWrite(SWITCH, HIGH);
    // myPlayer.play(1);
    // Serial.println("disini2");
    // myPlayer.play(2);
    // Serial.println("disini3");
    // myPlayer.play(3);

    Serial.println(myPlayer.readState()); //read mp3 state
    Serial.println(myPlayer.readVolume()); //read current volume
    Serial.println(myPlayer.readEQ()); //read EQ setting
    Serial.println(myPlayer.readFileCounts()); //read all file counts in SD card
    Serial.println(myPlayer.readCurrentFileNumber()); //read current play file number
    Serial.println(myPlayer.readFileCountsInFolder(3)); //read file counts in folder SD:/03
}

void loop() {
    unsigned long currentMillis = millis();

    // listening if I am slave
    if (currentMillis - previousMillis >= echoInterval) {
        while (Serial485.available() > 0) {
            String incomingSerial = Serial485.readString();
            if (getValue(incomingSerial, ',', 0) == "slave" && getValue(incomingSerial, ',', 1) == "0") {
                roleMasterFlag = false;
            }
        }
    }

    if (roleMasterFlag == false) {
        while (Serial485.available() > 0) {
            String incomingSerial = Serial485.readString();
            String header = getValue(incomingSerial, ',', 0);
            int action = getValue(incomingSerial, ',', 1).toInt();
            // 0=define slave, 1=get ready, 2=go, 3=back state

            // listen for ready flag
            if (header == "slave" && action == 1) {
                myPlayer.play(1);
            }
            // listen for go flag
            else if (header == "slave" && action == 2) {
                myPlayer.play(2);
                // kirim command lampu
                digitalWrite(SWITCH, LOW);
            }   
        }
    }

    // foot button pressed
    if (startFlag == false || digitalRead(FOOT_BUTTON) == HIGH) {
        buttonPressedFlag = true;
    }

    // get ready
    if (buttonPressedFlag == true && digitalRead(FOOT_BUTTON) == LOW) {
        startFlag = true;
        buttonPressedFlag = false;
        millisRandomStart = random(2000, 6000);
        previousMillis = currentMillis;
        // kirim modbus
        Serial485.print("slave,1\r\n");
        // play mp3
        myPlayer.play(1);
    }

    // go
    if (startFlag == true || buttonPressedFlag == false) {
        if (currentMillis - previousMillis >= millisRandomStart) {
            // kirim modbus
            Serial485.print("slave,2\r\n");
            // play mp3
            myPlayer.play(2);
            // nyalakan lampu
            myData.goFlag = true;
            esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
            if (result == ESP_OK) {
                Serial.println("Sent with success");
            } else {
                Serial.println("Error sending the data");
            }
            // mungkin perlu delay sedikit
            digitalWrite(SWITCH, LOW);
        }
    }

}
