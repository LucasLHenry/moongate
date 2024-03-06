#pragma once
#include <Arduino.h>

enum Mode {VCO, LFO, ENV};

class Module {
    public:
        uint16_t shape, ratio;
        uint16_t upslope, downslope;
        Mode mode;
        uint64_t acc, phasor;
        RwReg* pri_reg;
        RwReg* sec_reg;
        uint16_t pri_val;
        uint16_t sec_val;
        Module();
        Module(RwReg* pri_output_reg, RwReg* sec_output_reg);
        void update_pri();
        void update_sec(Module* other);
        void output();
};