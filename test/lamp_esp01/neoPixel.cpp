#include <neoPixel.h>

#define NEO_PIN     2
#define LED_COUNT   16*4

Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);

bool initNeoPixel() {
    strip.begin();
    strip.setBrightness(100);
    return true;
}

void testNeoPixel() {
    strip.clear();
    delay(500);
    for(int i=0; i<LED_COUNT; i++) {
        int red = random(0, 256);
        int green = random(0, 256);
        int blue = random(0, 256);
        
        strip.setPixelColor(i, red, green, blue);
        // strip.setPixelColor(i, 0, 255, 0);
        strip.show();
        delay(50);
    }
    // strip.clear();
    lightsDown();
}

void setNeoPixel(int segmnet, String color) {
    int numLED = 16;
    int edgeLED = numLED * segmnet;

    for(int i = edgeLED - numLED; i < edgeLED; i ++) {
        if (color == "clear") {
            strip.setPixelColor(i, 0, 0, 0);
        } else if (color == "red") {
            strip.setPixelColor(i, 255, 0, 0);
        } else if (color == "yellow") {
            strip.setPixelColor(i, 255, 255, 0);
        } else if (color == "green") {
            strip.setPixelColor(i, 0, 255, 0);
        }
        strip.show();
    }
}

void lightsUp() {
    setNeoPixel(1, "red");
    delay(100);
    setNeoPixel(2, "yellow");
    delay(100);
    setNeoPixel(3, "yellow");
    delay(100);
    setNeoPixel(4, "green");
}

void lightsDown() {
    for (int i=1; i<=4; i++ ) {
        setNeoPixel(i, "clear");
    }
}