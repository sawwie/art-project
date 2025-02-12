#include <cstdint>
#include <timers.hpp>
#include "api/Common.h"
#include "samd21/include/samd21g18a.h"
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN        5
#define NUMPIXELS 12
#define DELAYVAL 4

volatile uint8_t t;
volatile uint8_t tmp;
uint8_t heartbeat_wf[256] = {
35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,38,41,43,46,48,51,53,55,57,58,59,59,59,59,59,59,58,58,57,56,55,54,52,50,48,47,46,44,42,41,40,38,37,35,35,35,35,37,38,39,40,40,39,36,35,35,35,35,35,35,35,35,52,76,103,130,157,182,205,224,238,248,254,255,251,242,229,212,191,167,140,113,86,61,39,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,36,37,37,38,39,41,42,44,46,48,50,51,53,56,58,61,63,65,67,70,72,75,77,79,82,84,86,88,90,92,94,96,98,100,101,103,103,104,103,103,102,101,100,98,95,92,89,86,82,78,73,68,64,59,55,50,45,41,37,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35
};

void set_pixels(uint8_t r, uint8_t g, uint8_t b);

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void TC3_Handler() {
    TC3->COUNT16.INTFLAG.reg |= 0b00010000;
    t++;
    tmp = heartbeat_wf[t];
    set_pixels(tmp, 0, 0);
}

void TC4_Handler() {
    TC4->COUNT16.INTFLAG.reg |= 0b00010000;
    t++;
    tmp = heartbeat_wf[t];
    set_pixels(0, tmp, 0);
}

void set_pixels(uint8_t r, uint8_t g, uint8_t b) {
    for(int i=0; i<NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(r, g, b));
    }
    pixels.show();
}


void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif
    pixels.begin();
    t = 0;
    //setup_tcc1();
    setup_tc3();
    //setup_tc4();

}

void loop() {
//    set_pixels(t, 0, 0);
}

