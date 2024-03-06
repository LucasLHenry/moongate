#include <Arduino.h>
#include <avr/pgmspace.h>

#include "exponential.h"
#include "waveform_generator.hpp"

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
    p_acc {0},
    p_phasor {0},
    s_acc {0},
    s_phasor {0},
    pri_reg {pri_output_reg},
    sec_reg {sec_output_reg}
{}

void Module::accumulate() {
    p_acc += p_phasor;
    s_acc += s_phasor;
}

void Module::update_pri() {
    pri_val = generate_wave(p_acc, ratio, shape, upslope, downslope);
}

void Module::update_sec(Module* other) {
    sec_val = pri_val;
}

void Module::output() {
    *pri_reg = M - (pri_val >> 7);
    *sec_reg = M - (sec_val >> 7);
}