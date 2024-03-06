#pragma once
#include <Arduino.h>

enum Mode {VCO, LFO, ENV};

class Module {
    public:
        uint16_t shape, ratio;
        uint64_t upslope, downslope;
        Mode mode;
        uint64_t acc, phasor;
        RwReg* pri_reg;
        RwReg* sec_reg;
        uint64_t pri_val;
        uint64_t sec_val;
        Module();
        Module(RwReg* pri_output_reg, RwReg* sec_output_reg);
        void update_pri();
        void update_sec(Module* other);
        void output();
};

uint64_t asym_lin_map(uint16_t x, uint32_t low, uint32_t mid, uint32_t high);