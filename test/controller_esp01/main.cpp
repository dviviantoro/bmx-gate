#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

// MAC: bc:dd:c2:86:9a:96
uint8_t broadcastAddress[] = {0xBC, 0xDD, 0xC2, 0x86, 0x9A, 0x96};
unsigned long previousMillis = 0;
const long interval = 1000;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
    Serial.println(sendStatus == 0 ? "[ESP-NOW] delivery success" : "[ESP-NOW] delivery fail");
}

void sendStringData(String msg) {
    uint8_t *buffer = (uint8_t*) msg.c_str();
    size_t sizeBuff = sizeof(buffer) * msg.length();
    esp_now_send(broadcastAddress, buffer, sizeBuff);
}

void setup() {
    Serial.begin(9600);
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_register_send_cb(OnDataSent);
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        Serial.println("still alive now");
    }
    
    if (Serial.available()) {
        String payload = Serial.readStringUntil('\n');
        delay(2000);
        Serial.println("from esp01: ");
        Serial.println(payload);
        sendStringData(payload);
    }
}