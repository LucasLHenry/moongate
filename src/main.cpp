#include <Arduino.h>
#include <avr/pgmspace.h>
#include <fastLED.h>
#include "pins.h"
#include "timers.h"
#include "module.hpp"

Module A, B;
CRGB leds[NUM_LEDS];

void setup() {
  A = Module(&REG_TCC0_CC0, &REG_TCC0_CC2);
  B = Module(&REG_TCC0_CC1, &REG_TCC0_CC3);

  FastLED.addLeds<LED_TYPE, LED_DATA>(leds, NUM_LEDS);

  setupTimers(); // calls TCC0_Handler at 48kHz
}

void loop() {
}

void TCC0_Handler() 
{
  if (TCC0->INTFLAG.bit.CNT == 1) {
    A.accumulate();
    B.accumulate();
    delayMicroseconds(4);  // modify to deal with weird spikes
    A.update_pri();
    B.update_pri();
    A.update_sec(&B);
    B.update_sec(&A);
    A.output();
    B.output();
    TCC0->INTFLAG.bit.CNT = 1;
  }
}
