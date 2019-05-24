// Game scene
// (c) 2019 Jani Nykänen

#include "game.h"

#include <stdbool.h>
#include <stdlib.h>

#include "../../core/graphics.h"
#include "../../core/input.h"
#include "../../core/application.h"
#include "../../core/assets.h"

#include "stage.h"

// Game scene name
static const char* GAME_SCENE_NAME = "game";

// Game components
static Stage* stage;


// Initialize
static int16 game_init() {

    // Load bitmaps
    if(
        BITMAP("ASSETS/BITMAPS/FONT.BIN", "font") ||
        BITMAP("ASSETS/BITMAPS/FRAME.BIN", "frame") ||
        BITMAP("ASSETS/BITMAPS/TILESET.BIN", "tileset") ||
        BITMAP("ASSETS/BITMAPS/ANIM.BIN", "anim") ||
        BITMAP("ASSETS/BITMAPS/ITEMS.BIN", "items") ||
        BITMAP("ASSETS/BITMAPS/PLAYER.BIN", "player")
    ) {

        return 1;
    }

    // Create main components
    stage = create_stage();
    if(stage == NULL) {

        return 1;
    }

    // Initialize components
    init_boulders();
    if(stage_init(stage, "ASSETS/MAPS/0.BIN") == 1) {

        return 1;
    }

    return 0;
}


// Update
static void game_update(int16 steps) {

    // Quit (TEMPORARY!)
    if(input_get_button(4) == Pressed) {

        app_terminate();
    }

    // Update stage
    stage_update(stage, steps);
}


// Draw 
static void game_draw() {

    // Draw stage
    stage_draw(stage);
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
