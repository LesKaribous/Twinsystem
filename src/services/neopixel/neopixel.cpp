#include "neopixel.h"
#include "pin.h"

#define NEOPIXEL_PIN Pin::Led::neopixel   // Change this to match the pin you are using
#define NUM_LEDS 8       // Change this to match the number of LEDs in your strip

NeoPixel::NeoPixel() : Service(NEOPIXEL), strip(Adafruit_NeoPixel(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800)){
    strip.begin();
}

NeoPixel::~NeoPixel(){
}

void NeoPixel::enable(){
    Service::enable();
    for(int i=0; i<NUM_LEDS; i++) {
        strip.setPixelColor(i, 0, 0, 255);
    }
    strip.show();
}

void NeoPixel::disable() {
    Service::disable();
    for(int i=0; i<NUM_LEDS; i++) {
        strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
}

void NeoPixel::update(){
    //TODO Fade color rainbow effect
}
