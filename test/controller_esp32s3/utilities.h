#define RX_PLAYER 4
#define TX_PLAYER 5

#define SWITCH_PIN 9
#define FOOT_PIN 10
#define TX_485 12
#define RX_485 11
#define BUTTON_PIN 13

#define RDM_STR  2000
#define RDM_END  6000
// #define DEVICE_1
#define DEVICE_2

#define LED_PIN  48

HardwareSerial modbusSerial(1);
HardwareSerial playerSerial(2);

void initBoard() {
    Serial.begin(9600);
    playerSerial.begin(9600, SERIAL_8N1, RX_PLAYER, TX_PLAYER);
    modbusSerial.begin(115200, SERIAL_8N1, RX_485, TX_485);

    pinMode(SWITCH_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(SWITCH_PIN, HIGH);
    pinMode(FOOT_PIN, INPUT);
}