// Math extensions
// (c) 2019 Jani Nykänen

#include "mathext.h"


// Negative modulo
int16 neg_mod(int16 m, int16 n) {

    if(m < 0) {

        return n - (-m % n) -1;
    }
    return m % n;
}


// Maximum & minimum for short
int16 max_int16(int16 a, int16 b) {

    return a < b ? b : a;
}
int16 min_int16(int16 a, int16 b) {

    return a < b ? a : b;
}


// Absolute value
int16 abs_int16(int16 a) {

    return a >= 0 ? a : -a;
}
