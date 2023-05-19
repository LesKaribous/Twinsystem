#include "modules/neopixel/neopixel.h"
#include "pin.h"


#define NEOPIXEL_PIN Pin::Led::neopixel   // Change this to match the pin you are using
#define NUM_LEDS 8       // Change this to match the number of LEDs in your strip

NeoPixel::NeoPixel() : Module(NEOPIXEL), strip(Adafruit_NeoPixel(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800)){
    strip.begin();
}

NeoPixel::~NeoPixel(){
}

unsigned long start = 0;

void NeoPixel::enable(){
    start = millis();
    Module::enable();
    for(int i=0; i<NUM_LEDS; i++) {
        strip.setPixelColor(i, 0, 0, 255);
    }
    strip.show();
}

void NeoPixel::disable() {
    Module::disable();
    for(int i=0; i<NUM_LEDS; i++) {
        strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
}


int i = 0;
void NeoPixel::update(){
    //TODO Fade color rainbow effect
    if(millis() - start > 8000 && i == 0){
        for(int k=0; k<NUM_LEDS; k++) {
            strip.setPixelColor(i, 0, 255, 0);
        }
    }

    if(millis() - start > 16000 && i == 2){
        for(int k=0; k<NUM_LEDS; k++) {
            strip.setPixelColor(i, 255, 0, 0);
        }
    }

}
