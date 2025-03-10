#include <mp3tf16p.h>
#include <utilities.h>

MP3Player mp3(9,10);
// MP3Player mp3(5,4);
// MP3Player mp3(10,9);
// MP3Player mp3(PLAYER_RX,PLAYER_TX);

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("player begin");
    mp3.initialize();
    delay(1000);
    Serial.println("playing");
    mp3.playTrackNumber(2, 30);
    mp3.playTrackNumber(3, 30);
}

void loop() {
    mp3.serialPrintStatus(MP3_ALL_MESSAGE);
    mp3.playTrackNumber(2, 30);
    mp3.playTrackNumber(3, 30);
    Serial.println("hallo");
}



// #include <mp3tf16p.h>
// #include <utilities.h>

// MP3Player mp3(PLAYER_TX,PLAYER_RX);
// // MP3Player mp3(PLAYER_RX,PLAYER_TX);

// void setup() {
//     Serial.begin(115200);
//     delay(3000);
//     Serial.println("player begin");
//     mp3.initialize();
//     mp3.playTrackNumber(1, 30);
// }

// void loop() {
//     mp3.serialPrintStatus(MP3_ALL_MESSAGE);
// }

//  #include "Arduino.h"
//  #include "DFRobotDFPlayerMini.h"
//  #include "utilities.h"
 
// HardwareSerial FPSerial(1);

//  DFRobotDFPlayerMini myDFPlayer;
//  void printDetail(uint8_t type, int value);
 
//  void setup()
//  {
//  #if (defined ESP32)
// //    FPSerial.begin(9600, SERIAL_8N1, /*rx =*/PLAYER_RX, /*tx =*/PLAYER_TX);
//    FPSerial.begin(9600, SERIAL_8N1, /*rx =*/PLAYER_TX, /*tx =*/PLAYER_RX);
//  #else
//    FPSerial.begin(9600);
//  #endif
 
//    Serial.begin(115200);
 
//    Serial.println();
//    Serial.println(F("DFRobot DFPlayer Mini Demo"));
//    Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
   
//    if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
//      Serial.println(F("Unable to begin:"));
//      Serial.println(F("1.Please recheck the connection!"));
//      Serial.println(F("2.Please insert the SD card!"));
//      while(true){
//        delay(0); // Code to compatible with ESP8266 watch dog.
//      }
//    }
//    Serial.println(F("DFPlayer Mini online."));
   
//    myDFPlayer.volume(10);  //Set volume value. From 0 to 30
//    myDFPlayer.play(1);  //Play the first mp3
//  }
 
//  void loop()
//  {
//    static unsigned long timer = millis();
   
//    if (millis() - timer > 3000) {
//      timer = millis();
//      myDFPlayer.next();  //Play next mp3 every 3 second.
//    }
   
//    if (myDFPlayer.available()) {
//      printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
//    }
//  }
 
//  void printDetail(uint8_t type, int value){
//    switch (type) {
//      case TimeOut:
//        Serial.println(F("Time Out!"));
//        break;
//      case WrongStack:
//        Serial.println(F("Stack Wrong!"));
//        break;
//      case DFPlayerCardInserted:
//        Serial.println(F("Card Inserted!"));
//        break;
//      case DFPlayerCardRemoved:
//        Serial.println(F("Card Removed!"));
//        break;
//      case DFPlayerCardOnline:
//        Serial.println(F("Card Online!"));
//        break;
//      case DFPlayerUSBInserted:
//        Serial.println("USB Inserted!");
//        break;
//      case DFPlayerUSBRemoved:
//        Serial.println("USB Removed!");
//        break;
//      case DFPlayerPlayFinished:
//        Serial.print(F("Number:"));
//        Serial.print(value);
//        Serial.println(F(" Play Finished!"));
//        break;
//      case DFPlayerError:
//        Serial.print(F("DFPlayerError:"));
//        switch (value) {
//          case Busy:
//            Serial.println(F("Card not found"));
//            break;
//          case Sleeping:
//            Serial.println(F("Sleeping"));
//            break;
//          case SerialWrongStack:
//            Serial.println(F("Get Wrong Stack"));
//            break;
//          case CheckSumNotMatch:
//            Serial.println(F("Check Sum Not Match"));
//            break;
//          case FileIndexOut:
//            Serial.println(F("File Index Out of Bound"));
//            break;
//          case FileMismatch:
//            Serial.println(F("Cannot Find File"));
//            break;
//          case Advertise:
//            Serial.println(F("In Advertise"));
//            break;
//          default:
//            break;
//        }
//        break;
//      default:
//        break;
//    }
   
//  }