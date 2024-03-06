#include <Arduino.h>
#include "algos.hpp"
#include "module.hpp"


#define M16 65535
#define H16 32767

uint16_t rectify(Module* main, Module* aux) {

}

uint16_t sum(Module* main, Module* aux) {
    return (main->pri_val + aux->pri_val) >> 1;
}

uint16_t gate(Module* main, Module* aux) {
    if (aux->pri_val > H16) return main->pri_val;
    return 0;
}

uint16_t difference(Module* main, Module* aux) {
    return (main->pri_val + M16 - aux->pri_val) >> 1;
}

uint16_t exclusive_or(Module* main, Module* aux) {
    uint16_t top_half = main->pri_val & 0xF0;
    uint16_t bottom_half = (main->pri_val & 0x0F) ^ (aux->pri_val & 0x0F);
    return top_half | bottom_half;
}

uint16_t bitcrush(Module* main, Module* aux) {
    // remember that this is doing math with 7 more bits than the output, 
    // so this is effectively 9-7 = 2 bits of crushing
    #define CRUSH_AMT 9
    return (main->pri_val >> CRUSH_AMT) << CRUSH_AMT;
    #undef CRUSH_AMT
}

uint16_t invert(Module* main, Module* aux) {
    return M16 - main->pri_val;
}

uint16_t double_freq(Module* main, Module* aux) {
    
}

uint16_t half_freq(Module* main, Module* aux) {

}

uint16_t clip(Module* main, Module* aux) {
    #define CLIP_LVL 50000
    if (main->pri_val > CLIP_LVL) return CLIP_LVL;
    return main->pri_val;
    #undef CLIP_LVL
}

uint16_t saturate(Module* main, Module* aux) {

}

uint16_t ring_modulate(Module* main, Module* aux) {
    return (uint16_t)((main->pri_val * (uint64_t)aux->pri_val) >> 16);
}