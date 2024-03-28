#include <Arduino.h>
#include "module.hpp"

uint16_t asym_lin_map(uint16_t x, uint16_t low, uint16_t mid, uint16_t high);
uint16_t generate_wave(Module* m, bool is_pri);
uint16_t generate_saw(uint64_t acc);