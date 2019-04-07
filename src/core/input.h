// Input handling
// (c) 2019 Jani Nykänen

#ifndef __INPUT__
#define __INPUT__

#include "types.h"

// Arrow key enumerations
enum {

    ArrowRight = 0,
    ArrowUp = 1,
    ArrowLeft = 2,
    ArrowDown = 3,
};

// Key states
enum {
    Up = 0,
    Down = 1,
    Pressed = 2,
    Released = 3,
};


// Initialize
void init_input();

// Destroy
void destroy_input();

// Get key state
int16 input_get_key(int16 id);
// Get arrow key state
int16 input_get_arrow_key(int16 id);

// Add a "button"
void input_add_button(int16 index, int16 key);
// Get a "button" state
int16 input_get_button(int16 id);

#endif // __INPUT__
