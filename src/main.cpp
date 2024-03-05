#include <Arduino.h>
#include <avr/pgmspace.h>
#include "pins.h"
#include "timers.h"
#include "module.hpp"

Module A, B;

void setup() {
  A = Module(&REG_TCC0_CC0, &REG_TCC0_CC2);
  B = Module(&REG_TCC0_CC1, &REG_TCC0_CC3);
}

void loop() {
  A.update_pri();
  A.update_sec();
  A.output();
}
