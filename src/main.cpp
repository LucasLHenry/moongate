#include <Arduino.h>
#define ENCODER_DO_NOT_USE_INTERRUPTS  // comment out if missing ticks
#include <Encoder.h>
#include <OneButton.h>
#include <Mux.h>
#include <FastLED.h>
#include <avr/pgmspace.h>

#include "pins.h"
#include "colours.h"
#include "timers.h"
#include "exponential.h"
// #include "module.hpp"
// #include "waveform_generator.hpp"

#define HZPHASOR 91183 //phasor value for 1 hz.
#define DEFAULT_SHAPE 255
#define DEFAULT_LIN 255

#define M 511
#define H 255
#define UPSLOPE(x) ((M << 7) / x)
#define DOWNSLOPE(x) ((M << 7) / (M - x))
#define MAX(a, b) (((a) > (b))? (a) : (b))
#define MIN(a, b) (((a) < (b))? (a) : (b))
#define CLIP(x, a, b) MAX(MIN(x, b), a)

// Module A, B;

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

typedef enum Mode {
    VCO,
    LFO,
    ENV,
} Mode;

typedef struct Module {
    unsigned long int acc;
    unsigned long int phasor;
    Mode mode;
    unsigned short int ratio;
    unsigned short int shape;
    unsigned long int upslope;
    unsigned long int downslope;
    unsigned short int val;
} Module;

Module m;

static void handle_enc_btn_press();
static void show_leds();
static void update_encoder();

bool trig_val {false};

void setup() {
    // some sort of LED light sequence on startup would be cool

    setupTimers(); // calls TCC0_Handler at 48kHz
    FastLED.addLeds<SK6812, LED_DATA, GRB>(leds, NUM_LEDS);

    enc_btn.attachClick(handle_enc_btn_press);

    constexpr uint64_t default_upslope = UPSLOPE(DEFAULT_SHAPE);
    constexpr uint64_t default_downslope = DOWNSLOPE(DEFAULT_SHAPE);
    m = {0, 0, VCO, DEFAULT_SHAPE, DEFAULT_LIN, default_upslope, default_downslope};
    m.phasor = 100 * HZPHASOR;
}


int32_t rat_pot, shp_pot, rat_cv, shp_cv, new_r, new_s;
void loop() {
    enc_btn.tick();
    update_encoder();
    // show_leds();
    rat_pot = a_mux.read(R_POT_A) >> 1;
    rat_cv = (a_mux.read(R_CV_A) - M) >> 1;
    new_r = CLIP(rat_pot + rat_cv, 0, 511);
    if (new_r >> 1 != m.ratio) {
        m.ratio = new_r;
        if (m.ratio != 0) m.upslope = UPSLOPE(m.ratio);
        if (m.ratio != 511) m.downslope = DOWNSLOPE(m.ratio);
    }

    shp_pot = a_mux.read(S_POT_A) >> 1;
    shp_cv = (a_mux.read(S_CV_A) - M) >> 1;
    new_s = CLIP(shp_pot + shp_cv, 0, 511);
    if (new_s >> 1 != m.shape) m.shape = new_s;
}

static void handle_enc_btn_press() {
    enc_a_is_active = !enc_a_is_active;
    // trig_val = !trig_val;
}

int asym_lin_map(uint16_t x, int low, int mid, int high) {
    if (x <= 0) return low;
    if (x < H) return (x * (mid - low) >> 8) + low;
    if (x == H) return mid;
    if (x > H) return ((x - H) * (high - mid) >> 8) + mid;
    return high;
}

uint32_t linval, expval, logval;
uint64_t shifted_acc;
unsigned short int generator() {
    shifted_acc = m.acc>>23;
    if (shifted_acc < m.ratio) {
        linval = m.upslope * shifted_acc;
        expval = pgm_read_word_near(exptable + (linval >> 7));
        logval = (M << 7) - pgm_read_word_near(exptable + (m.upslope * (m.ratio - shifted_acc) >> 7));
    } else {
        linval = m.downslope * (M - shifted_acc);
        expval = pgm_read_word_near(exptable + (linval >> 7));
        logval = (M << 7) - pgm_read_word_near(exptable + (m.downslope * (shifted_acc - m.ratio) >> 7));
    }
    return asym_lin_map(m.shape, expval, linval, logval) >> 6;
}


void TCC0_Handler() 
{
    if (TCC0->INTFLAG.bit.CNT == 1) {
        m.acc += m.phasor;
        REG_TCC0_CCB2 = 1023 - generator();
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
            if (i == active_a) leds[i] =  PURPLE;
            else leds[i] = BLACK;
        }
    } else {
        for (int i = 0; i < NUM_RING_LEDS; i++) {
            if (i == active_a) leds[i] = RED;
            else if (i == active_b) leds[i] = BLUE;
            else leds[i] = BLACK;
        }
    }
    // show signal indicator LEDs
    FastLED.show();
}