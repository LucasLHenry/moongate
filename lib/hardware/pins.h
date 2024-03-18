// pin assignments for the SAM D21
// use these names to refer to them
#pragma once
#include <FastLED.h>


#define LIN_TIME_A A6
#define SIG_IN_A A1
#define MUX_A A2
#define PRI_OUT_A A4  // don't know what PWM reg this corresponds to
#define SEC_OUT_A A3  // same as above
#define TRIG_OUT_A 4

#define LIN_TIME_B A0
#define SIG_IN_B A7
#define MUX_B A5
#define PRI_OUT_B 10  // don't know what PWM reg this corresponds to
#define SEC_OUT_B 12  // same as above
#define TRIG_OUT_B 6

#define LED_DATA 2
#define NUM_LEDS 23
#define NUM_RING_LEDS 16
#define LED_TYPE NEOPIXEL
#define FLW_LED 16
#define TRIG_B_LED 17
#define PRI_B_LED 18
#define SEC_B_LED 19
#define SEC_A_LED 20
#define PRI_A_LED 21
#define TRIG_A_LED 22



#define ALGO_ENC_1 11
#define ALGO_ENC_2 5
#define ALGO_BTN 13
#define FLW_BTN 7

#define MUX_S0 3
#define MUX_S1 1
#define MUX_S2 0