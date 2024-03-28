#include <Arduino.h>
#include "pins.h"
#include "colours.h"
#include "timers.h"
#include "module.hpp"
#include "waveform_generator.hpp"
#include <Encoder.h>
#include <OneButton.h>
#include <Mux.h>
#include <FastLED.h>

#define HZPHASOR 91183 //phasor value for 1 hz.

Module A, B;

// Adafruit_NeoPixel leds(NUM_LEDS, LED_DATA, NEO_GRB + NEO_KHZ800);
CRGB leds[NUM_LEDS];

Encoder enc(ALGO_ENC_1, ALGO_ENC_2);
long int enc_a_pos, enc_b_pos;
int active_a, active_b;
long int enc_pos, new_enc_pos, enc_change;
#define ENC_DIV 2

OneButton enc_btn(ALGO_BTN, true, true);
bool enc_a_is_active = true;

using namespace admux;
Mux a_mux(admux::Pin(MUX_A, INPUT, PinType::Analog), Pinset(MUX_S0, MUX_S1, MUX_S2));
Mux b_mux(admux::Pin(MUX_B, INPUT, PinType::Analog), Pinset(MUX_S0, MUX_S1, MUX_S2));



static void handle_enc_btn_press();
static void show_leds();
static void update_encoder();

long unsigned int accumulator1 = 0;
long unsigned int phasor1 = 0;

bool trig_val {false};

void setup() {
    // pinMode(1, OUTPUT);
    // pinMode(9, OUTPUT);
    // pinMode(2, OUTPUT);
    // pinMode(3, OUTPUT);
    // pinMode(13, OUTPUT);
    // pinMode(TRIG_OUT_A, OUTPUT);
    A = Module();
    // B = Module(&REG_TCC0_CC1, &REG_TCC0_CC3);
    // set pin modes

    Serial.begin(9600);
    // some sort of LED light sequence on startup would be cool

    setupTimers(); // calls TCC0_Handler at 48kHz
    FastLED.addLeds<SK6812, LED_DATA, GRB>(leds, NUM_LEDS);

    enc_btn.attachClick(handle_enc_btn_press);
    A.p_phasor = 100 * HZPHASOR;
    phasor1 = 100 * HZPHASOR;
}

void loop() {
    enc_btn.tick();
    update_encoder();
    show_leds();
    
    // digitalWrite(TRIG_OUT_A, (trig_val)? HIGH : LOW);
    // Serial.println(511- (accumulator1 >> 23));
    generate_wave(&A, true);
    A.accumulate();
}

static void handle_enc_btn_press() {
    enc_a_is_active = !enc_a_is_active;
    // trig_val = !trig_val;
}

void TCC0_Handler() 
{
    if (TCC0->INTFLAG.bit.CNT == 1) {
        // A.accumulate();
        // // B.accumulate();
        // delayMicroseconds(4);  // modify to deal with weird spikes
        // A.update_pri();
        // // B.update_pri();
        // // A.update_sec(&B);
        // // B.update_sec(&A);
        // REG_TCC0_CC0 = 511 - (A.pri_val >> 7);
        accumulator1 += phasor1;
        //
        delayMicroseconds(4);
        // REG_TCC0_CC2 = 1023 - (generate_wave(&A, true) >> 6);
        REG_TCC0_CC2 = 1023 - generate_saw(accumulator1);
                //delayMicroseconds(4);
        while (TCC0->SYNCBUSY.bit.CC2);
        TCC0->INTFLAG.bit.CNT = 1;
    }
}

static void update_encoder() {
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
}

static void show_leds() {
    // show ring LEDs
    if (active_a == active_b) {
        for (int i = 0; i < NUM_RING_LEDS; i++) {
            if (i == active_a) leds[i] =  PLORANGE;
            else leds[i] = BLACK;
        }
    } else {
        for (int i = 0; i < NUM_RING_LEDS; i++) {
            if (i == active_a) leds[i] = ORANGE;
            else if (i == active_b) leds[i] = PURPLE;
            else leds[i] = BLACK;
        }
    }
    // show signal indicator LEDs
    FastLED.show();
}