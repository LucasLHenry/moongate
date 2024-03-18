#include <Arduino.h>
#include <avr/pgmspace.h>

#include "waveform_generator.hpp"
#include "exponential.h"

#ifndef M
#define M 511
#endif
#ifndef H
#define H 255
#endif


uint16_t asym_lin_map(uint16_t x, uint16_t low, uint16_t mid, uint16_t high) {
    if (x <= 0) return low;
    if (x < H) return (x * (mid - low) >> 8) + low;
    if (x == H) return mid;
    if (x > H) return ((x - H) * (high - mid) >> 8) + mid;
    if (x >= M) return high;
    return mid;
}

uint16_t generate_wave(uint64_t acc, uint16_t ratio, uint16_t shape, uint16_t upslope, uint16_t downslope) {
    uint16_t s_acc {static_cast<uint16_t>(acc >> 23)};
    uint16_t linval {0};
    uint16_t expval {0};
    uint16_t logval {0};

    if (s_acc < ratio) {
        linval = upslope * s_acc;
        expval = pgm_read_word_near(exptable + (linval >> 7));
        logval = (M << 7) - pgm_read_word_near(exptable + (upslope * (ratio - s_acc) >> 7));
    } else {
        linval = downslope * (M - s_acc);
        expval = pgm_read_word_near(exptable + (linval >> 7));
        logval = (M << 7) - pgm_read_word_near(exptable + (downslope * (s_acc - ratio) >> 7));
    }

    return asym_lin_map(shape, expval, linval, logval);
}

uint16_t generate_saw(uint64_t acc) {
    uint16_t s_acc {static_cast<uint16_t>(acc >> 23)};
    return s_acc;
}