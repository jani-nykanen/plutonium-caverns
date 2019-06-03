// Intro scene
// (c) 2019 Jani Nykänen

#include "intro.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../../core/graphics.h"
#include "../../core/input.h"
#include "../../core/application.h"
#include "../../core/assets.h"
#include "../../core/transition.h"
#include "../../core/mathext.h"

// Constants
static const int16 WAIT_TIME = 120;

// Intro bitmap
static Bitmap* bmpIntro;

// Bitmap drawn
static boolean bitmapDrawn;

// Timer
static int16 timer;


// Go to title
static void cb_go_to_title() {

    tr_activate(FadeOut, 1, NULL); // Just changes the speed
    app_change_scene("title", (void*)1);
}


// Initialize
static int16 intro_init() {

    // Load intro bitmap
    bmpIntro = load_bitmap("ASSETS/BITMAPS/INTRO.BIN");
    if(bmpIntro == NULL) {

        return 1;
    }

    // Set transition
    tr_activate(FadeOut, 2, NULL);

    // Set defaults
    bitmapDrawn = false;
    timer = WAIT_TIME;

    return 0;
}


// Update
static void intro_update(int16 steps) {

    if(tr_is_active()) return;

    if((timer -= steps) <= 0) {

        tr_activate(FadeIn, 2, cb_go_to_title);
    }
}


// Draw 
static void intro_draw() {

    if(!bitmapDrawn) {

        clear_screen(0);
        
        draw_bitmap_fast(bmpIntro, 
            160-bmpIntro->width/2, 
            100-bmpIntro->height/2);

        bitmapDrawn = true;
    }
}


// Dispose
static void intro_dispose() {

    // ...
}


// Change
static void intro_on_change(void* param) {

    // ...
}



// Get title menu scene
Scene intro_get_scene() {

    Scene s;
    s.init = intro_init;
    s.update = intro_update;
    s.draw = intro_draw;
    s.dispose = intro_dispose;
    s.change = intro_on_change;
    s.name = NULL;

    return s;
}
