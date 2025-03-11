#include "Arduino.h"
#include <DFPlayerMini_Fast.h>
#include <esp_now.h>
#include <WiFi.h>
#include <utilities.h>

DFPlayerMini_Fast myMP3;

const long sendYouAreSlaveInterval = 1000;
const long ledInterval = 500;

unsigned long previousMillis = 0;
unsigned long previousMillisLed = 0;
const int waitIfSlave = 5000;

int buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 150;

bool execute = false;
bool roleMaster = false;
bool ledState = LOW;
int bounceCounter = 0;

#ifdef DEVICE_1
    #define DEVICE_ID "bmx_gate_1"
    uint8_t broadcastAddress[] = {0xBC, 0xDD, 0xC2, 0x86, 0x9A, 0x96};
#else 
    #define DEVICE_ID "bmx_gate_2"
    uint8_t broadcastAddress[] = {0x5C, 0xCF, 0x7F, 0x7D, 0x6C, 0x4A};
#endif

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("[ESP-NOW] ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void sendStringData(String msg) {
    uint8_t *buffer = (uint8_t*) msg.c_str();
    size_t sizeBuff = sizeof(buffer) * msg.length();
    esp_now_send(broadcastAddress, buffer, sizeBuff);
}

void initESPNow() {
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_send_cb(OnDataSent);
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
    
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
}

String getValue(String data, char separator, int index) {
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length()-1;

    for(int i=0; i<=maxIndex && found<=index; i++) {
        if(data.charAt(i)==separator || i==maxIndex) {
            found++;
            strIndex[0] = strIndex[1]+1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void executeNow(int millisRandomStart) {
    if (roleMaster) {
        Serial.print("[DICE]: throwing dice for random ms delay: ");
        millisRandomStart = random(RDM_STR, RDM_END);
        Serial.println(millisRandomStart);

        String msg485 = DEVICE_ID + String(",");
        msg485 += String(millisRandomStart) + ",";
        modbusSerial.println(msg485);
    }

    Serial.println("[PLAY]: ready.mp3");
    myMP3.play(3);
    delay(6000);
    
    Serial.print("[DICE]: delay for ");
    Serial.println(millisRandomStart);
    delay(millisRandomStart);
    
    digitalWrite(SWITCH_PIN, LOW);
    String msgESPNow = DEVICE_ID + String(",1,1,");
    Serial.print("[ESP NOW]: ");
    Serial.println(msgESPNow);
    sendStringData(msgESPNow);
    
    Serial.println("[PLAY]: go.mp3");
    myMP3.play(2);
    delay(4000);
    digitalWrite(SWITCH_PIN, HIGH);
    Serial.println("execute finish, all idle");
}

void setup() {
    initBoard();
    initESPNow();
    delay(3000);
    myMP3.begin(playerSerial, true);
    Serial.println("Setting volume to max");
    myMP3.volume(30);
    previousMillis = millis();
}

void loop() {
    unsigned long currentMillis = millis();
    // int reading = digitalRead(FOOT_PIN);
    // digitalWrite(LED_PIN, reading);
    
    // if (currentMillis - previousMillisLed >= ledInterval) {
    //     previousMillisLed = currentMillis;

    //     if (ledState == LOW) {
    //         ledState = HIGH;
    //     } else {
    //         ledState = LOW;
    //     }
      
    //     digitalWrite(LED_PIN, ledState);
    // }
    
    /*
    if (modbusSerial.available() > 0) {
        String payload = modbusSerial.readStringUntil ('\n');
        Serial.print("payload: ");
        Serial.println(payload);
    }
    if (currentMillis - previousMillis >= sendYouAreSlaveInterval) {
        previousMillis = currentMillis;
        String msg = String(DEVICE_ID) + ",you_are_slave";
        modbusSerial.println(msg);
        Serial.println(msg);
    }
    */

    if (roleMaster) {
        if (currentMillis - previousMillis >= sendYouAreSlaveInterval) {
            previousMillis = currentMillis;
            String msg = String(DEVICE_ID) + ",you_are_slave,";
            modbusSerial.println(msg);
            Serial.println(msg);
        }

        delay(1);
        int reading = digitalRead(FOOT_PIN);
        if (reading == HIGH) {
            bounceCounter ++;
        } else {
            bounceCounter = 0;
        }
        if (bounceCounter > 500) {
            digitalWrite(LED_PIN, HIGH);
            executeNow(random(RDM_STR, RDM_END));
        } else {
            digitalWrite(LED_PIN, LOW);
        }
    } else {
        if (modbusSerial.available() > 0) {
            String payload = modbusSerial.readStringUntil('\n');
            String master = getValue(payload, ',', 0);
            String msg = getValue(payload, ',', 1);
            
            Serial.print("payload: ");
            Serial.println(payload);

            if (msg == "you_are_slave") {
                roleMaster = false;
                Serial.println("I am still slave");
            } else if (msg.toInt() >= RDM_STR) {
                executeNow(getValue(payload, ',', 1).toInt());
            }
            previousMillis = millis();
        } else if (currentMillis - previousMillis >= waitIfSlave) {
            roleMaster = true;
            Serial.println("I am as master now");
            myMP3.play(4);
            delay(2500);
        }
    } 
}
