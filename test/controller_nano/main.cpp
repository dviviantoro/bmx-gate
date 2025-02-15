#include <Arduino.h>
#include <mp3tf16p.h>
#include <utilities.h>
#include <SoftwareSerial.h>

SoftwareSerial SerialESP01 (TX_ESP01, RX_ESP01);
SoftwareSerial Serial485 (TX_485, RX_485);
MP3Player mp3(TX_PLAYER,RX_PLAYER);

unsigned long previousMillis = 0;
// const int echoInterval = 200;
const int waitIfSlave = 5000;
int checkIfSlaveCounter = 0;

int buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

String masterName;
bool execute = false;
bool roleMaster = false;

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

void executeAsMaster() {
    int millisRandomStart = random(RDM_STR, RDM_END);
    Serial.print("random millis: ");
    mp3.playTrackNumber(3, 30);
    delay(5000);
    delay(millisRandomStart);
    digitalWrite(SWITCH_PIN, LOW);
    String msg = DEVICE_ID + String(",1,");
    Serial.println(msg);
    SerialESP01.println(msg);
    mp3.playTrackNumber(2, 30);
    
    Serial.println("action finish");
}

void executeNow(int millisRandomStart) {
    if (roleMaster) {
        millisRandomStart = random(RDM_STR, RDM_END);
        String msg485 = DEVICE_ID + String(",");
        msg485 += String(millisRandomStart) + ",";
        Serial485.println(msg485);
    }

    mp3.playTrackNumber(3, 30);
    delay(5000);
    delay(millisRandomStart);
    digitalWrite(SWITCH_PIN, LOW);
    String msg = DEVICE_ID + String(",1,");
    // Serial.println(msg);
    SerialESP01.println(msg);
    mp3.playTrackNumber(2, 30);
}

void setup() {
    Serial.begin(9600);
    SerialESP01.begin(9600);
    Serial485.begin(9600);

    delay(1000);
    mp3.initialize();

    pinMode(SWITCH_PIN, OUTPUT);
    digitalWrite(SWITCH_PIN, HIGH);
    pinMode(FOOT_PIN, INPUT);

    mp3.playTrackNumber(2, 30);
    Serial.println("System ready, born as slave");
}

void loop() {
    unsigned long currentMillis = millis();

    if (roleMaster) {
        int reading = digitalRead(FOOT_PIN);
        if (reading != lastButtonState) lastDebounceTime = millis();
        
        if ((millis() - lastDebounceTime) > debounceDelay) {
            if (reading != buttonState) {
                buttonState = reading;
                
                if (buttonState == HIGH) {
                    Serial.println("Button HIGH");
                    // executeAsMaster();
                    executeNow(random(RDM_STR, RDM_END));
                }
            }
        }
        lastButtonState = reading;
    } else if (!roleMaster) {
        if (Serial485.available()) {
            String payload = Serial.readStringUntil('\n');
            String masterID = getValue(payload, ',', 0);
            
            executeNow(getValue(payload, ',', 1).toInt());
        } else {
            if (currentMillis - previousMillis >= waitIfSlave) {
                roleMaster = true;
                Serial.println("I am as master now");
            }
        }
    }
}
