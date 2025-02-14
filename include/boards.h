#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <utilities.h>
#include <DFRobotDFPlayerMini.h>

bool buttonPressedFlag = false;
bool startFlag = false;
int millisRandomStart = 0;
unsigned long previousMillis = 0;
const int echoInterval = 500;
const int waitingEchoCount = 20;
bool roleMasterFlag = true;
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct struct_message {
    bool goFlag;
} struct_message;
struct_message myData;
esp_now_peer_info_t peerInfo;

HardwareSerial Serial485(1);
HardwareSerial SerialPlayer(2);
// DFRobotDFPlayerMini myPlayer;

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

void initBoard() {
    Serial.begin(9600);
    Serial.println("Init Board in progress");

    pinMode(SWITCH, OUTPUT);
    pinMode(BUTTON, INPUT);
    pinMode(FOOT_BUTTON, INPUT);

#ifdef MODBUS_COM
    Serial485.begin(115200, SERIAL_8N1, MODBUS_RX, MODBUS_TX);
    Serial.println("MOBUS initialized");
#endif

#ifdef CANBUS_COM
    SPI.begin(CANBUS_SCK, CANBUS_MISO, CANBUS_MOSI);
    Serial.println("CANBUS initialized");
#endif

#ifdef MP3_PLAYER
    SerialPlayer.begin(9600, SERIAL_8N1, PLAYER_RX, PLAYER_TX);
    Serial.println("MP3 player initialized");
#endif
}