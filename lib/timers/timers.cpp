#include "timers.hpp"
#include "samd21/include/samd21g18a.h"



void setup_tc3() {
    TC3->COUNT16.INTENSET.bit.OVF = 1;
    TC3->COUNT16.INTENSET.bit.ERR = 1;
    
    // Configure asynchronous clock source
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_TCC2_TC3_Val;    // select TC3 peripheral channel
    GCLK->CLKCTRL.reg |= GCLK_CLKCTRL_GEN_GCLK0;        // select source GCLK_GEN[0]
    GCLK->CLKCTRL.bit.CLKEN = 1;            // enable TC3 generic clock
    // Configure synchronous bus clock
    PM->APBCSEL.bit.APBCDIV = 0;            // no prescaler
    PM->APBCMASK.bit.TC3_ = 1;                // enable TC3 interface

    // Configure Count Mode (16-bit)
    TC3->COUNT16.CTRLA.bit.MODE = 0x0;

    // Configure Prescaler for divide by 8 (125kHz clock to COUNT)
    TC3->COUNT16.CTRLA.bit.PRESCALER = 0x3;

    // Configure TC3 Compare Mode for compare channel 0
    TC3->COUNT16.CTRLA.bit.WAVEGEN = 0x1;            // "Match Frequency" operation

    // Initialize compare value for x s @ 125kHz (reg = x * 125k)
    TC3->COUNT16.CC[0].reg = 50000;

    // Enable TC3 compare mode interrupt generation
    TC3->COUNT16.INTENSET.bit.MC0 = 0x1;    // Enable match interrupts on compare channel 0 

    // Enable TC3
    TC3->COUNT16.CTRLA.bit.ENABLE = 1;

    // Wait until TC3 is enabled
    while(TC3->COUNT16.STATUS.bit.SYNCBUSY == 1);

    NVIC_SetPriority(TC3_IRQn, 3);
    NVIC_EnableIRQ(TC3_IRQn);
}

void setup_tc4() {
    //TC4->COUNT16.INTENSET.bit.OVF = 1;
    //TC4->COUNT16.INTENSET.bit.ERR = 1;
    //
    //
    //GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_TC
    //// Configure synchronous bus clock
    //PM->APBCSEL.bit.APBCDIV = 0;            // no prescaler
    //PM->APBCMASK.bit.TC4_ = 1;                // enable TC4 interface
    //
    //// Configure Count Mode (16-bit)
    //TC4->COUNT16.CTRLA.bit.MODE = 0x0;
    //
    //// Configure Prescaler for divide by 8 (500kHz clock to COUNT)
    //TC4->COUNT16.CTRLA.bit.PRESCALER = 0x3;
    //
    //// Configure TC3 Compare Mode for compare channel 0
    //TC4->COUNT16.CTRLA.bit.WAVEGEN = 0x1;            // "Match Frequency" operation
    //
    //// Initialize compare value for x s @ 500kHz (reg = x * 500k)
    //TC4->COUNT16.CC[0].reg = 50000;
    //
    //// Enable TC3 compare mode interrupt generation
    //TC4->COUNT16.INTENSET.bit.MC0 = 0x1;    // Enable match interrupts on compare channel 0 
    //
    //// Enable TC3
    //TC4->COUNT16.CTRLA.bit.ENABLE = 1;
    //
    //// Wait until TC3 is enabled
    //while(TC4->COUNT16.STATUS.bit.SYNCBUSY == 1);
    //
    //NVIC_SetPriority(TC4_IRQn, 3);
    //NVIC_EnableIRQ(TC4_IRQn);
}
