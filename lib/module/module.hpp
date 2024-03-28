#pragma once
#include <Arduino.h>

enum Mode {VCO, LFO, ENV};

class Module {
    public:
        uint16_t shape, ratio;
        uint16_t upslope, downslope;
        Mode mode;
        uint64_t p_acc, p_phasor, s_acc, s_phasor;  // primary, secondary
        uint16_t pri_val;
        uint16_t sec_val;
        Module();
        void accumulate();
};