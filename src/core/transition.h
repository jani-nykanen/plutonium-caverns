// Transition
// (c) 2019 Jani Nykänen

#ifndef __TRANSITION__
#define __TRANSITION__

#include "types.h"

// Mode
enum {
    FadeIn = 0,
    FadeOut = 1
};
typedef uint8 Mode;

// Initialize
void init_transition();

// Activate
void tr_activate(Mode mode, int16 speed, void (*cb)(void));

// Update transition
void tr_update(int16 steps);

// Draw transition
void tr_draw();

// Is active
boolean tr_is_active();

#endif // __TRANSITION__
