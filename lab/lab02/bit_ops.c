#include <stdio.h>
#include "bit_ops.h"

// Return the nth bit of x.
// Assume 0 <= n <= 31
unsigned get_bit(unsigned x,
                 unsigned n) {
    unsigned mask = ((unsigned)1) << n;
    return (x & mask) >> n;
    // return (x >> n) & 1;
}
// Set the nth bit of the value of x to v.
// Assume 0 <= n <= 31, and v is 0 or 1
void set_bit(unsigned * x,
             unsigned n,
             unsigned v) {
    unsigned mask = 1 << n;
    unsigned check = (*x & mask) >> n;
    if (check != v){
        if (check == 0){ // 0 -> 1
            *x = *x | mask;
        } else { // 1 -> 0
            mask ^= 0xff;
            *x = *x & mask;
        }
    }
}
// Flip the nth bit of the value of x.
// Assume 0 <= n <= 31
void flip_bit(unsigned * x,
              unsigned n) {
    unsigned mask = 1 << n;
    unsigned check = (*x & mask) >> n;
    if (check == 0){ // 0 -> 1
            *x = *x | mask;
        } else { // 1 -> 0
            mask ^= 0xff;
            *x = *x & mask;
        }
}
