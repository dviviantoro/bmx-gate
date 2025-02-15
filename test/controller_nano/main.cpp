#include <Arduino.h>
#include <mp3tf16p.h>
#include <utilities.h>
#include <SoftwareSerial.h>

SoftwareSerial SerialESP01 (TX_ESP01, RX_ESP01);
SoftwareSerial Serial485 (TX_485, RX_485);
MP3Player mp3(TX_PLAYER,RX_PLAYER);
unsigned long previousMillis = 0;
const int echoInterval = 500;

int buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

bool execute = false;

void executeAsMaster() {
    int millisRandomStart = random(RDM_STR, RDM_END);
    Serial.print("random millis: ");
    Serial.println(millisRandomStart);
    
    Serial.println("step 1");
    mp3.playTrackNumber(3, 30);
    delay(5000);
    Serial.println("step 2");
    delay(millisRandomStart);
    
    Serial.println("step 3");
    digitalWrite(SWITCH_PIN, LOW);
    Serial.println("step 4");
    String msg = DEVICE_ID + String(",") + "1";
    mp3.playTrackNumber(2, 30);
    
    Serial.println("action finish");
    SerialESP01.println(msg);
}

void setup() {
    Serial.begin(9600);
    SerialESP01.begin(115200);
    Serial485.begin(115200);

    delay(1000);
    mp3.initialize();

    pinMode(SWITCH_PIN, OUTPUT);
    digitalWrite(SWITCH_PIN, HIGH);
    pinMode(FOOT_PIN, INPUT);

    mp3.playTrackNumber(2, 30);
    Serial.println("System ready");
}

void loop() {
    int reading = digitalRead(FOOT_PIN);
    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }
    
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading != buttonState) {
            buttonState = reading;
            
            if (buttonState == HIGH) {
                Serial.println("Button HIGH");
                executeAsMaster();
                execute = true;
            }
        }
    }
    lastButtonState = reading;
    
    if (SerialESP01.available()) {
        String payload = Serial.readStringUntil('\n');
        Serial.println("from nano: ");
        Serial.println(payload);
    }

    // if (execute == true) {
    //     executeAsMaster();
    //     execute = false;
    // }


    // unsigned long currentMillis = millis();

    // // listening if I am slave
    // if (currentMillis - previousMillis >= echoInterval) {
    //     while (Serial485.available() > 0) {
    //         String incomingSerial = Serial485.readString();
    //         if (getValue(incomingSerial, ',', 0) == "slave" && getValue(incomingSerial, ',', 1) == "0") {
    //             roleMasterFlag = false;
    //         }
    //     }
    // }

    // if (roleMasterFlag == false) {
    //     while (Serial485.available() > 0) {
    //         String incomingSerial = Serial485.readString();
    //         String header = getValue(incomingSerial, ',', 0);
    //         int action = getValue(incomingSerial, ',', 1).toInt();
    //         // 0=define slave, 1=get ready, 2=go, 3=back state

    //         // listen for ready flag
    //         if (header == "slave" && action == 1) {
    //             myPlayer.play(1);
    //         }
    //         // listen for go flag
    //         else if (header == "slave" && action == 2) {
    //             myPlayer.play(2);
    //             // kirim command lampu
    //             digitalWrite(SWITCH, LOW);
    //         }   
    //     }
    // }

    // // foot button pressed
    // if (startFlag == false || digitalRead(FOOT_BUTTON) == HIGH) {
    //     buttonPressedFlag = true;
    // }

    // // get ready
    // if (buttonPressedFlag == true && digitalRead(FOOT_BUTTON) == LOW) {
    //     startFlag = true;
    //     buttonPressedFlag = false;
    //     millisRandomStart = random(2000, 6000);
    //     previousMillis = currentMillis;
    //     // kirim modbus
    //     Serial485.print("slave,1\r\n");
    //     // play mp3
    //     myPlayer.play(1);
    // }

    // // go
    // if (startFlag == true || buttonPressedFlag == false) {
    //     if (currentMillis - previousMillis >= millisRandomStart) {
    //         // kirim modbus
    //         Serial485.print("slave,2\r\n");
    //         // play mp3
    //         myPlayer.play(2);
    //         // nyalakan lampu
    //         myData.goFlag = true;
    //         esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    //         if (result == ESP_OK) {
    //             Serial.println("Sent with success");
    //         } else {
    //             Serial.println("Error sending the data");
    //         }
    //         // mungkin perlu delay sedikit
    //         digitalWrite(SWITCH, LOW);
    //     }
    // }

}
