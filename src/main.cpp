#include <Arduino.h>
// #include <avr/pgmspace.h>
#include "pins.h"
// #include "timers.h"
// #include "module.hpp"
#include <Adafruit_NeoPixel.h>
#include <Encoder.h>
#include <OneButton.h>

// Module A, B;

#define ORANGE    0xFF7000
#define RED       0xFF0000
#define WHITE     0xFFFFFF
#define BLACK     0x000000
#define TURQUOISE 0x00FFFF
#define PINK      0xFF00FF
#define PLORANGE  0xBF3880
#define PURPLE    0x7F00FF

Adafruit_NeoPixel strip(NUM_LEDS, LED_DATA, NEO_GRB + NEO_KHZ800);

Encoder enc(ALGO_ENC_1, ALGO_ENC_2);
long int enc_pos = 0;
long int new_enc_pos = 0;
long int enc_change = 0;
long int enc_a_pos, enc_b_pos;
int active_a, active_b;
#define ENC_DIV 2

OneButton enc_btn(ALGO_BTN, true, true);
bool enc_a_is_active = true;

static void handle_enc_btn_press();

void setup() {
  // A = Module(&REG_TCC0_CC0, &REG_TCC0_CC2);
  // B = Module(&REG_TCC0_CC1, &REG_TCC0_CC3);
  // set pin modes

  Serial.begin(9600);
  // some sort of LED light sequence on startup would be cool

  //setupTimers(); // calls TCC0_Handler at 48kHz
  strip.begin();
  strip.show();

  enc_btn.attachClick(handle_enc_btn_press);
}

void loop() {
  enc_btn.tick();

  // encoder
  new_enc_pos = enc.read();
  enc_change = new_enc_pos - enc_pos;
  enc_pos = new_enc_pos;
  if (enc_a_is_active) {
    enc_a_pos += enc_change;
    active_a = (enc_a_pos >> ENC_DIV) % NUM_RING_LEDS;
    if (active_a < 0) 
      active_a += 16;
  } else {
    enc_b_pos += enc_change;
    active_b = (enc_b_pos >> ENC_DIV) % NUM_RING_LEDS;
    if (active_b < 0) 
      active_b += 16;
  }
  

  // LEDs
  if (active_a == active_b) {
    for (int i = 0; i < NUM_RING_LEDS; i++) {
      if (i == active_a) strip.setPixelColor(i, PLORANGE);
      else strip.setPixelColor(i, BLACK);
    }
  } else {
    for (int i = 0; i < NUM_RING_LEDS; i++) {
      if (i == active_a) strip.setPixelColor(i, ORANGE);
      else if (i == active_b) strip.setPixelColor(i, PURPLE);
      else strip.setPixelColor(i, BLACK);
    }
  }
  strip.show();
}

static void handle_enc_btn_press() {
  enc_a_is_active = (enc_a_is_active)? false : true;
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
