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

Module::Module():
    shape {INIT_SHAPE},
    ratio {INIT_RATIO},
    upslope {UPSLOPE(INIT_RATIO)},
    downslope {DOWNSLOPE(INIT_RATIO)},
    mode {LFO},
    p_acc {0},
    p_phasor {0},
    s_acc {0},
    s_phasor {0}
{}

void Module::accumulate() {
    p_acc += p_phasor;
    s_acc += s_phasor;
}