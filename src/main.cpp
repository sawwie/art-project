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

volatile uint8_t t_3;
volatile uint8_t t_4;
volatile uint8_t t_5;

volatile uint8_t tc3_val;
volatile uint8_t tc4_val;
volatile uint8_t tc5_val;

volatile uint8_t tmp;

volatile uint8_t heartbeat_wf1[256] = {
35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,38,41,43,46,48,51,53,55,57,58,59,59,59,59,59,59,58,58,57,56,55,54,52,50,48,47,46,44,42,41,40,38,37,35,35,35,35,37,38,39,40,40,39,36,35,35,35,35,35,35,35,35,52,76,103,130,157,182,205,224,238,248,254,255,251,242,229,212,191,167,140,113,86,61,39,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,36,37,37,38,39,41,42,44,46,48,50,51,53,56,58,61,63,65,67,70,72,75,77,79,82,84,86,88,90,92,94,96,98,100,101,103,103,104,103,103,102,101,100,98,95,92,89,86,82,78,73,68,64,59,55,50,45,41,37,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35
};

volatile uint8_t heartbeat_wf2[256] = {
    35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,38,41,43,46,48,51,53,55,57,58,59,59,59,59,59,59,58,58,57,56,55,54,52,50,48,47,46,44,42,41,40,38,37,35,35,35,35,37,38,39,40,40,39,36,35,35,35,35,35,35,35,35,52,76,103,130,157,182,205,224,238,248,254,255,251,242,229,212,191,167,140,113,86,61,39,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,36,37,37,38,39,41,42,44,46,48,50,51,53,56,58,61,63,65,67,70,72,75,77,79,82,84,86,88,90,92,94,96,98,100,101,103,103,104,103,103,102,101,100,98,95,92,89,86,82,78,73,68,64,59,55,50,45,41,37,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35
};

volatile uint8_t heartbeat_wf3[256] = {
    35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,38,41,43,46,48,51,53,55,57,58,59,59,59,59,59,59,58,58,57,56,55,54,52,50,48,47,46,44,42,41,40,38,37,35,35,35,35,37,38,39,40,40,39,36,35,35,35,35,35,35,35,35,52,76,103,130,157,182,205,224,238,248,254,255,251,242,229,212,191,167,140,113,86,61,39,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,36,37,37,38,39,41,42,44,46,48,50,51,53,56,58,61,63,65,67,70,72,75,77,79,82,84,86,88,90,92,94,96,98,100,101,103,103,104,103,103,102,101,100,98,95,92,89,86,82,78,73,68,64,59,55,50,45,41,37,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35
};

volatile void set_pixels_1(uint8_t r, uint8_t g, uint8_t b);
volatile void set_pixels_2(uint8_t r, uint8_t g, uint8_t b);
volatile void set_pixels_3(uint8_t r, uint8_t g, uint8_t b);

Adafruit_NeoPixel tc3_slave(NUMPIXELS, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel tc4_slave(NUMPIXELS, 4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel tc5_slave(NUMPIXELS, 5, NEO_GRB + NEO_KHZ800);

void TC3_Handler() {
    TC3->COUNT16.INTFLAG.reg |= 0b00010000;
    if (TC3->COUNT16.CC[0].reg < 5000 && tmp >= 200) {
        tc3_val = 255;
    } else {
        if (TC3->COUNT16.CC[0].reg > 9000) {
            TC3->COUNT16.CC[0].reg -= 10;
        } else {
            TC3->COUNT16.CC[0].reg -= 2;
        }
        t_3++;
        tc3_val = heartbeat_wf1[t_3];
        tmp = tc3_val;
    }
    set_pixels_1(tc3_val, 0, 0);
}

void TC4_Handler() {
    TC4->COUNT16.INTFLAG.reg |= 0b00010000;
    t_4++;
    tc4_val = heartbeat_wf2[t_4];
    set_pixels_2(tc4_val, 0, 0);
}

void TC5_Handler() {
    TC5->COUNT16.INTFLAG.reg |= 0b00010000;
    t_5++;
    tc5_val = heartbeat_wf3[t_5];
    set_pixels_3(tc5_val, 0, 0);
}

volatile void set_pixels_1(uint8_t r, uint8_t g, uint8_t b) {
    for(int i=0; i<NUMPIXELS; i++) {
        tc3_slave.setPixelColor(i, tc3_slave.Color(r, g, b));
    }
    tc3_slave.show();
}

volatile void set_pixels_2(uint8_t r, uint8_t g, uint8_t b) {
    for(int i=0; i<NUMPIXELS; i++) {
        tc4_slave.setPixelColor(i, tc4_slave.Color(r, g, b));
    }
    tc4_slave.show();
}

volatile void set_pixels_3(uint8_t r, uint8_t g, uint8_t b) {
    for(int i = 0; i < NUMPIXELS; i++) {
        tc5_slave.setPixelColor(i, tc5_slave.Color(r, g, b));
    }
    tc5_slave.show();
}


void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif
    noInterrupts();
    tc3_slave.begin();
    tc4_slave.begin();
    tc5_slave.begin();
    t_3 = 0;
    t_4 = 0;
    t_5 = 0;
    setup_tc3();
    setup_tc4();
    setup_tc5();
    interrupts();
}

void loop() {
//    set_pixels(t, 0, 0);
}

