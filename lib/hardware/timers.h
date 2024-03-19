#pragma once
#include <Arduino.h>

void setupTimers()
{
    GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |          // Improve duty cycle
                    GCLK_GENCTRL_GENEN |        // Enable generic clock gen
                    GCLK_GENCTRL_SRC_DFLL48M |  // Select 48MHz as source
                    GCLK_GENCTRL_ID(4);         // Select GCLK4
    while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

    // Set clock divider of 1 to generic clock generator 4
    GCLK->GENDIV.reg = GCLK_GENDIV_DIV(1) |         // Divide 48 MHz by 1
                        GCLK_GENDIV_ID(4);           // Apply to GCLK4 4
    while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

    // Enable GCLK4 and connect it to TCC0 and TCC1
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |        // Enable generic clock
                        GCLK_CLKCTRL_GEN_GCLK4 |    // Select GCLK4
                        GCLK_CLKCTRL_ID_TCC0_TCC1;  // Feed GCLK4 to TCC0/1
    while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

    // Divide counter by 1 giving 48 MHz (20.83 ns) on each TCC0 tick
    TCC0->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV1_Val);

    // Use "Normal PWM" (single-slope PWM): count up to PER, match on CC[n]
    TCC0->WAVE.reg = TCC_WAVE_WAVEGEN_NPWM;         // Select NPWM as waveform
    while (TCC0->SYNCBUSY.bit.WAVE);                // Wait for synchronization

    // Set the period (the number to count to (TOP) before resetting timer)
    TCC0->PER.reg = 1023;
    while (TCC0->SYNCBUSY.bit.PER);

    // Set PWM signal to output 50% duty cycle
    // n for CC[n] is determined by n = x % 4 where x is from WO[x]
    TCC0->CC[2].reg = 511;
    while (TCC0->SYNCBUSY.bit.CC2);

    // Configure PA18 (D10 on Arduino Zero) to be output
    PORT->Group[PORTA].DIRSET.reg = PORT_PA18;      // Set pin as output
    PORT->Group[PORTA].OUTCLR.reg = PORT_PA18;      // Set pin to low

    // Enable the port multiplexer for PA18
    PORT->Group[PORTA].PINCFG[18].reg |= PORT_PINCFG_PMUXEN;

    // Connect TCC0 timer to PA18. Function F is TCC0/WO[2] for PA18.
    // Odd pin num (2*n + 1): use PMUXO
    // Even pin num (2*n): use PMUXE
    PORT->Group[PORTA].PMUX[9].reg = PORT_PMUX_PMUXE_F;


    TCC0->INTENSET.reg = 0;
    TCC0->INTENSET.bit.CNT = 1; // enable count interrupt
    TCC0->INTENSET.bit.MC0 = 0;

    // enable interrupts and being output
    NVIC_EnableIRQ(TCC0_IRQn);
    TCC0->CTRLA.reg |= TCC_CTRLA_ENABLE;
    return;
    // Enable output (start PWM)
    TCC0->CTRLA.reg |= (TCC_CTRLA_ENABLE);
    while (TCC0->SYNCBUSY.bit.ENABLE);  
    return;
    // for some reason the code below isn't working, so I'm going to try and write it all over again

    // TCC0 is a timer/counter, and ticks up it's value on a clock input until it reaches it's max value
    // when it reaches it's max value, it goes back to 0. TCC0 has 4 CC registers (CC0, CC1, CC2, CC3)
    // that can be used to generate PWM signals. This works by comparing each of them to the TCC0 counter
    // when CCx is greater than TCC0, the signal is high, otherwise, the signal is low. Additionally,
    // an interrupt can be called whenever TCC0 reaches it's top and resets, thus allowing for synchronized
    // changes to CCx. To configure this, several things must be done. First, the clock input to TCC0 must
    // be set up at a certain frequency (using divide down). Then it must be sent to TCC0 by setting it as 
    // TCC0's input. then TCC0 must be configured with a top value (effectively the period in clock ticks)
    // and a mode. Also can reverse the polarity of it, (so it counts down instead?). Then the signals
    // created by CCx must be routed to output pins, which are limited in which ones they can be. Finally,
    // interrupts must be set up to call whenever TCC0 reaches it's top value.

    // Enable and configure generic clock generator 4
    GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |          // Improve duty cycle
                        GCLK_GENCTRL_GENEN |        // Enable generic clock gen
                        GCLK_GENCTRL_SRC_DFLL48M |  // Select 48MHz as source
                        GCLK_GENCTRL_ID(4);         // Select GCLK4
    while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

    // Set clock divider of 1 to generic clock generator 4
    GCLK->GENDIV.reg = GCLK_GENDIV_DIV(1) |         // Divide 48 MHz by 1
                       GCLK_GENDIV_ID(4);           // Apply to GCLK4 4
    while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

    // Divide counter by 1 giving 48MHz
    TCC0->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV1_Val);

    // Set the period (the number to count to (TOP) before resetting timer)
    TCC0->PER.reg = 48 - 1; // effectively divides by 48, so count is 1MHz saw wave
    while (TCC0->SYNCBUSY.bit.PER);

    // set pins to be outputs, and clear their values
    // PORT->Group[PORTA].DIRSET.reg = PORT_PA04;
    // PORT->Group[PORTA].OUTCLR.reg = PORT_PA04;
    // PORT->Group[PORTA].DIRSET.reg = PORT_PA05;
    // PORT->Group[PORTA].OUTCLR.reg = PORT_PA05;
    PORT->Group[PORTA].DIRSET.reg = PORT_PA18;
    PORT->Group[PORTA].OUTCLR.reg = PORT_PA18;
    // PORT->Group[PORTA].DIRSET.reg = PORT_PA19;
    // PORT->Group[PORTA].OUTCLR.reg = PORT_PA19;


    // connect output pins to peripheral muxes instead of normal operation
    // PORT->Group[PORTA].PINCFG[04].reg |= PORT_PINCFG_PMUXEN;
    // PORT->Group[PORTA].PINCFG[05].reg |= PORT_PINCFG_PMUXEN;
    PORT->Group[PORTA].PINCFG[18].reg |= PORT_PINCFG_PMUXEN;
    // PORT->Group[PORTA].PINCFG[19].reg |= PORT_PINCFG_PMUXEN;

    // connect output pins to the correct waveform outputs
    // consult datasheet for this stuff
    // PORT->Group[PORTA].PMUX[2].reg = PORT_PMUX_PMUXE_E;  // PA04 -> WO[0]
    // PORT->Group[PORTA].PMUX[2].reg = PORT_PMUX_PMUXO_E;  // PA05 -> WO[1]
    PORT->Group[PORTA].PMUX[9].reg = PORT_PMUX_PMUXE_F;  // PA18 -> WO[2]
    // PORT->Group[PORTA].PMUX[9].reg = PORT_PMUX_PMUXO_F;  // PA19 -> WO[3]

    TCC0->INTENSET.reg = 0;
    TCC0->INTENSET.bit.CNT = 1; // enable count interrupt
    TCC0->INTENSET.bit.MC0 = 0;

    // enable interrupts and being output
    NVIC_EnableIRQ(TCC0_IRQn);
    TCC0->CTRLA.reg |= TCC_CTRLA_ENABLE;

    // REG_GCLK_GENDIV = GCLK_GENDIV_DIV(2) |          // Divide the 48MHz clock source by divisor N=1: 48MHz/1=48MHz
    //                   GCLK_GENDIV_ID(4);            // Select Generic Clock (GCLK) 4
    // while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

    // REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
    //                   GCLK_GENCTRL_GENEN |         // Enable GCLK4
    //                   GCLK_GENCTRL_SRC_DFLL48M |   // Set the 48MHz clock source
    //                   GCLK_GENCTRL_ID(4);          // Select GCLK4
    // while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization
    
    // //enable our 4 pins to be PWM outputs
    // PORT->Group[g_APinDescription[1].ulPort].PINCFG[g_APinDescription[1].ulPin].bit.PMUXEN = 1;
    // PORT->Group[g_APinDescription[9].ulPort].PINCFG[g_APinDescription[9].ulPin].bit.PMUXEN = 1;
    // PORT->Group[g_APinDescription[2].ulPort].PINCFG[g_APinDescription[2].ulPin].bit.PMUXEN = 1;
    // PORT->Group[g_APinDescription[3].ulPort].PINCFG[g_APinDescription[3].ulPin].bit.PMUXEN = 1;

    // //assign the 4 outputs to the PWM registers on PMUX
    // PORT->Group[g_APinDescription[1].ulPort].PMUX[g_APinDescription[1].ulPin >> 1].reg = PORT_PMUX_PMUXE_E;  // D3 is on PA11 = odd      
    // PORT->Group[g_APinDescription[9].ulPort].PMUX[g_APinDescription[9].ulPin >> 1].reg |= PORT_PMUX_PMUXO_E; // D11 is on PA08 = even 
    // PORT->Group[g_APinDescription[2].ulPort].PMUX[g_APinDescription[2].ulPin >> 1].reg |= PORT_PMUX_PMUXE_F;  // D3 is on PA11 = odd
    // PORT->Group[g_APinDescription[3].ulPort].PMUX[g_APinDescription[3].ulPin >> 1].reg |= PORT_PMUX_PMUXO_F; // D11 is on PA08 = even

  

    // // Feed GCLK4 to TCC0 and TCC1
    // REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TCC0 and TCC1
    //                   GCLK_CLKCTRL_GEN_GCLK0 |     // Select GCLK4
    //                   GCLK_CLKCTRL_ID_TCC0_TCC1;   // Feed GCLK4 to TCC0 and TCC1
    // while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization


    // // Dual slope PWM operation: timers countinuously count up to PER register value then down 0
    // REG_TCC0_WAVE |= TCC_WAVE_POL(0xF) |           // Reverse the output polarity on all TCC0 outputs
    //                   TCC_WAVE_WAVEGEN_DSBOTH |
    //                   TCC_WAVE_WAVEGEN_NFRQ;     // Setup dual slope PWM on TCC0
    // while (TCC0->SYNCBUSY.bit.WAVE);               // Wait for synchronization

    // // Each timer counts up to a maximum or TOP value set by the PER register,
    // // this determines the frequency of the PWM operation: Freq = 48Mhz/(2*N*PER)
    // REG_TCC0_PER = 0x1FF;                           // Set the FreqTcc and period of the PWM on TCC0
    // while (TCC0->SYNCBUSY.bit.PER);                // Wait for synchronization
  
    // REG_TCC0_CC1 = 10;                             // TCC1 CC1 - on D3  50% pin 9
    // while (TCC0->SYNCBUSY.bit.CC1);                   // Wait for synchronization
    // REG_TCC0_CC0 = 50;                             // TCC1 CC0 - on D11 50% pin 1
    // while (TCC0->SYNCBUSY.bit.CC0);                   // Wait for synchronization
    // REG_TCC0_CC2 = 200;                             // TCC1 CC1 - on D3  50% pin 2
    // while (TCC0->SYNCBUSY.bit.CC2);                   // Wait for synchronization
    // REG_TCC0_CC3 = 254;                             // TCC1 CC0 - on D11 50% pin 3
    // while (TCC0->SYNCBUSY.bit.CC3);                   // Wait for synchronization
  
    // // Divide the GCLOCK signal by 1 giving  in this case 48MHz (20.83ns) TCC1 timer tick and enable the outputs
    // REG_TCC0_CTRLA |= TCC_CTRLA_PRESCALER_DIV1 |    // Divide GCLK4 by 1
    //                   TCC_CTRLA_ENABLE;             // Enable the TCC0 output
    // while (TCC0->SYNCBUSY.bit.ENABLE);              // Wait for synchronization
}