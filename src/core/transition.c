// Transition
// (c) 2019 Jani Nykänen

#include "transition.h"

#include <stdlib.h>

#include "graphics.h"

// Initial time
static const int16 TR_INITIAL_TIME = 70;

// Params
static Mode mode;
static int16 timer;
static int16 speed;
static boolean active;
static void (*callback)(void);


// Initialize
void init_transition() {

    active = false;
    callback = NULL;
}


// Activate
void tr_activate(Mode m, int16 s, 
    void (*cb)(void)) {

    mode = m;
    speed = s;
    active = true;
    callback = cb;

    timer = TR_INITIAL_TIME;
}


// Update transition
void tr_update(int16 steps) {

    if(!active) return;

    // Update timer
    timer -= steps * speed;
    if(timer <= 0) {

        if(mode == FadeOut) {

            timer = 0;

            set_palette_darkness(0);
            active = false;
        }
        else {

            timer += TR_INITIAL_TIME;
            if(callback != NULL) {

                callback();
            }
            mode = FadeOut;
        }
    }
}


// Draw transition
void tr_draw() {

    int16 level;

    if(!active) return;

    // Set the corresponding darkness level
    level = timer / 10;
    if(mode == FadeIn)
        level = 7-level;

    set_palette_darkness(level);
}


// Is active
boolean tr_is_active() {

    return active;
}
