#include <Arduino.h>
#include <avr/pgmspace.h>

#include "exponential.h"

#include "module.hpp"

#ifndef M
#define M 511
#endif
#ifndef H
#define H 255
#endif

#define UPSLOPE(x) ((M << 7) / x)
#define DOWNSLOPE(x) ((M << 7) / (M - x))

#define INIT_SHAPE 255
#define INIT_RATIO 255

Module::Module(){}

Module::Module(RwReg* pri_output_reg, RwReg* sec_output_reg):
    shape {INIT_SHAPE},
    ratio {INIT_RATIO},
    upslope {UPSLOPE(INIT_RATIO)},
    downslope {DOWNSLOPE(INIT_RATIO)},
    mode {LFO},
    acc {0},
    phasor {0},
    pri_reg {pri_output_reg},
    sec_reg {sec_output_reg}
{}

uint64_t asym_lin_map(uint16_t x, uint32_t low, uint32_t mid, uint32_t high) {
  if (x <= 0) return low;
  if (x < H) return (x * (mid - low) >> 8) + low;
  if (x == H) return mid;
  if (x > H) return ((x - H) * (high - mid) >> 8) + mid;
  if (x >= M) return high;
  return mid;
}

void Module::update_pri() {
    uint16_t s_acc {(uint16_t)(acc >> 23)};
    uint32_t linval {0};
    uint32_t expval {0};
    uint32_t logval {0};

    if (s_acc < ratio) {
        linval = upslope * s_acc;
        expval = pgm_read_word_near(exptable + (linval >> 7));
        logval = (M << 7) - pgm_read_word_near(exptable + (upslope * (ratio - s_acc) >> 7));
    } else {
        linval = downslope * (M - s_acc);
        expval = pgm_read_word_near(exptable + (linval >> 7));
        logval = (M << 7) - pgm_read_word_near(exptable + (downslope * (s_acc - ratio) >> 7));
    }

    pri_val = asym_lin_map(shape, expval, linval, logval);
}

void Module::update_sec() {
    sec_val = pri_val;
}

void Module::output() {
    *pri_reg = M - (pri_val >> 7);
    *sec_reg = M - (sec_val >> 7);
}