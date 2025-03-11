#include "Arduino.h"
#include <DFPlayerMini_Fast.h>

HardwareSerial playerSerial(1);
DFPlayerMini_Fast myMP3;

void setup() {
    Serial.begin(115200);
    // playerSerial.begin(9600, SERIAL_8N1, 13, 12);
    playerSerial.begin(9600, SERIAL_8N1, 4, 5);
    myMP3.begin(playerSerial, true);
    
    delay(1000);
    
    Serial.println("Setting volume to max");
    myMP3.volume(30);
    
    Serial.println("Looping track 1");
    // myMP3.loop(2);
    // myMP3.loop(3);

    // myMP3.play(2);
    // myMP3.play(3);
    // myMP3.play(1);
}

void loop() {
  myMP3.play(3);
  Serial.println(myMP3.isPlaying());
  delay(5000);
  Serial.println(myMP3.isPlaying());
  myMP3.play(2);
  Serial.println(myMP3.isPlaying());
  delay(5000);
  Serial.println(myMP3.isPlaying());
  myMP3.play(1);
  Serial.println(myMP3.isPlaying());
  delay(5000);
}