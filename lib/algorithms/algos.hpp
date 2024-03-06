#include <Arduino.h>
#include "algos.hpp"
#include "module.hpp"

#ifndef M
#define M 511
#endif
#ifndef H
#define H 255
#endif

uint16_t rectify(Module* main, Module* aux);
uint16_t sum(Module* main, Module* aux);
uint16_t gate(Module* main, Module* aux);
uint16_t difference(Module* main, Module* aux);
uint16_t exclusive_or(Module* main, Module* aux);
uint16_t bitcrush(Module* main, Module* aux);
uint16_t invert(Module* main, Module* aux);
uint16_t double_freq(Module* main, Module* aux);
uint16_t half_freq(Module* main, Module* aux);
uint16_t clip(Module* main, Module* aux);
uint16_t saturate(Module* main, Module* aux);
uint16_t ring_modulate(Module* main, Module* aux);