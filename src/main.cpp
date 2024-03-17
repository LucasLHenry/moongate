#include <Arduino.h>
// #include <avr/pgmspace.h>
#include "pins.h"
// #include "timers.h"
// #include "module.hpp"
#include <Adafruit_NeoPixel.h>
#include <Encoder.h>

// Module A, B;

#define ORANGE    0xFF7000
#define RED       0xFF0000
#define WHITE     0xFFFFFF
#define BLACK     0x000000
#define TURQUOISE 0x00FFFF
#define PINK      0xFF00FF
#define PURPLE    0x7F00FF

Adafruit_NeoPixel strip(NUM_LEDS, LED_DATA, NEO_GRB + NEO_KHZ800);
Encoder enc(ALGO_ENC_1, ALGO_ENC_2);
long int enc_pos = 0;
int active_a, active_b;
#define ENC_DIV 2

void setup() {
  // A = Module(&REG_TCC0_CC0, &REG_TCC0_CC2);
  // B = Module(&REG_TCC0_CC1, &REG_TCC0_CC3);
  // set pin modes

  Serial.begin(9600);
  // some sort of LED light sequence on startup would be cool

  //setupTimers(); // calls TCC0_Handler at 48kHz
  strip.begin();
  strip.show();
}

void loop() {
  enc_pos = enc.read();

  int active_led = (enc_pos >> ENC_DIV) % NUM_RING_LEDS;
  Serial.println(active_led);
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < NUM_RING_LEDS) {
      if (i == active_led) strip.setPixelColor(i, ORANGE);
      else strip.setPixelColor(i, BLACK);
    }
  }
  strip.show();
  
  // for (int i = 0; i < NUM_LEDS; i++) {
  //   strip.setPixelColor(i, PURPLE);
  //   strip.setPixelColor((i + 4) % NUM_LEDS, ORANGE);
  //   strip.show();
  //   delay(500);
    
  //   strip.setPixelColor(i, BLACK);
  //   strip.setPixelColor((i + 4) % NUM_LEDS, BLACK);
  //   strip.show();
  // }
}

// void TCC0_Handler() 
// {
//   if (TCC0->INTFLAG.bit.CNT == 1) {
//     A.accumulate();
//     B.accumulate();
//     delayMicroseconds(4);  // modify to deal with weird spikes
//     A.update_pri();
//     B.update_pri();
//     A.update_sec(&B);
//     B.update_sec(&A);
//     A.output();
//     B.output();
//     TCC0->INTFLAG.bit.CNT = 1;
//   }
// }
