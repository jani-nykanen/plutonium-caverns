// Math extensions
// (c) 2019 Jani Nykänen

#ifndef __MATHEXT_H__
#define __MATHEXT_H__

#include "../core/types.h"

// Negative modulo
int16 neg_mod(int16 m, int16 n);

// Maximum & minimum for short
int16 max_int16(int16 a, int16 b);
int16 min_int16(int16 a, int16 b);

// Absolute value
int16 abs_int16(int16 a);

#endif //__MATHEXT_H__
