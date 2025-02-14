#include <Arduino.h>
// #include <ESP8266WiFi.h>
// #include <espnow.h>
#include <neoPixel.h>

// MAC: bc:dd:c2:86:9a:96
unsigned long previousMillis = 0;
const int resetInterval = 5000;

typedef struct struct_message {
    bool goFlag;
} struct_message;
struct_message myData;

void lightsUp() {
    setNeoPixel(1, "red");
    delay(50);
    setNeoPixel(2, "yellow");
    delay(50);
    setNeoPixel(3, "yellow");
    delay(50);
    setNeoPixel(4, "green");
}

void lightsDown() {
    for (int i=1; i<=4; i++ ) {
        setNeoPixel(i, "clear");
    }
}

// void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
//     memcpy(&myData, incomingData, sizeof(myData));
//     Serial.print("Bool: ");
//     Serial.println(myData.goFlag);

//     previousMillis = millis();
//     lightsUp();
// }

// void initEspNow() {
//     WiFi.mode(WIFI_STA);

//     if (esp_now_init() != 0) {
//         Serial.println("Error initializing ESP-NOW");
//         return;
//     }
    
//     esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
//     esp_now_register_recv_cb(OnDataRecv);
// }

void setup() {
    Serial.begin(115200);
    delay(3000);
    // Serial.print("ESP Board MAC Address:  ");
    // Serial.println(WiFi.macAddress());

    // initEspNow();
    initNeoPixel();
}

void loop() {
    // unsigned long currentMillis = millis();
    // if (myData.goFlag == true) {
    //     if (currentMillis - previousMillis >= resetInterval) {
    //         lightsDown();
    //         myData.goFlag = false;
    //     }
    // }
    testNeoPixel();
    delay(2000);
    lightsDown();
    delay(2000);
    lightsUp();
    delay(2000);
    lightsDown();
    delay(2000);
}