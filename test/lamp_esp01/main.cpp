#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <neoPixel.h>

// #define DEVICE_ID "bmx_gate_1"
#define DEVICE_ID "bmx_gate_2"

unsigned long previousMillis = 0;
unsigned long previousMillisReset = 0;
const long interval = 1000;
const int resetInterval = 5000;
bool state = false;

// if you need to parse the incoming data
String getValue(String data, char separator, int index) {
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length()-1;

    for(int i=0; i<=maxIndex && found<=index; i++)
    {
        if(data.charAt(i)==separator || i==maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1]+1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
    char* buff = (char*) incomingData;
    String buffStr = String(buff);

    String device = getValue(buffStr, ',', 0);
    int message = getValue(buffStr, ',', 1).toInt();
    if (device == DEVICE_ID) {
        if (message == 1) {
            Serial.println("lamp device triggered");
            lightsUp();
            state = true;
            previousMillisReset = millis();
        }
    }

    Serial.println(buffStr);
    Serial.println(device);
    Serial.println(message);
}

void initEspNow() {
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    
    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
    esp_now_register_recv_cb(OnDataRecv);
}

void setup() {
    Serial.begin(9600);
    delay(3000);
    Serial.print("ESP Board MAC Address:  ");
    Serial.println(WiFi.macAddress());

    initEspNow();
    initNeoPixel();
    testNeoPixel();
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        Serial.println("still alive now");
    }
    
    if (state == true) {
        if (currentMillis - previousMillisReset >= resetInterval) {
            Serial.println("Lights will down");
            lightsDown();
            state = false;
        }
    }
}