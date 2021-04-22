#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lfsr.h"

uint16_t get_bit(uint16_t *x, uint16_t n) {
    return (*x >> n) & 1;
}
void lfsr_calculate(uint16_t *reg) {
   uint16_t ex1 = get_bit(reg, 0) ^ get_bit(reg, 2); 
   uint16_t ex2 = ex1 ^ get_bit(reg, 3);
   uint16_t ex3 = ex2 ^ get_bit(reg, 5);
   uint16_t fid = ex3 << 15;
   *reg >>= 1;
   *reg += fid;
}

