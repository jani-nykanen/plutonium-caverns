// Game scene
// (c) 2019 Jani Nykänen

#include "game.h"

#include <stdbool.h>
#include <stdlib.h>

#include "../../core/graphics.h"

// Game scene name
static const char* GAME_SCENE_NAME = "game";

// Test bitmap
static Bitmap* bmpFont;



// Initialize
static int16 game_init() {

    // Load font
    bmpFont = load_bitmap("ASSETS/BITMAPS/FONT.BIN");

    return 0;
}


// Update
static void game_update(int16 steps) {

    // ...
}


// Draw 
static void game_draw() {

    clear_screen(170);

    draw_text_fast(bmpFont, "HELLO WORLD!", 8, 8, 0, 0, false);
}


// Dispose
static void game_dispose() {

    // ...
}


// Get game scene
Scene game_get_scene() {

    Scene s;
    s.init = game_init;
    s.update = game_update;
    s.draw = game_draw;
    s.dispose = game_dispose;
    s.name = GAME_SCENE_NAME;

    return s;
}
