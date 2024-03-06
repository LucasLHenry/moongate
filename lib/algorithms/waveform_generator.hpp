#include <Arduino.h>

uint16_t asym_lin_map(uint16_t x, uint16_t low, uint16_t mid, uint16_t high);
uint16_t generate_wave(uint64_t acc, uint16_t ratio, uint16_t shape, uint16_t upslope, uint16_t downslope);