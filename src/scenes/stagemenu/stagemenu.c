// Stage menu scene
// (c) 2019 Jani Nykänen

#include "stagemenu.h"

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
#include "../../core/audio.h"

#include "../game/game.h"

// Scene name
static const char* SMENU_SCENE_NAME = "smenu";

// Constants
static const char* FPATH = "SAVE.DAT";
static const uint16 STAGE_COUNT = 17;
static const int16 ANIM_TIME = 120;
static const int16 PLANET_X = 320-64;
static const int16 PLANET_Y = 100;

// (Re)draw flags
static boolean redrawBG;
static boolean redrawMenu;
static boolean fillOldCursorPos;

// Planet cursor pos
static Vector2 pcpos; 
// Menu cursor position
static Vector2 cpos;
// Menu cursor old position
static Vector2 opos;
// Cursor animation timer
static int16 animTimer;

// Stage index
static uint8 stageIndex;
// Stage target
static uint8 stageTarget;

// Bitmaps
static Bitmap* bmpSMenu;
static Bitmap* bmpFont;


// Draw stars
static void draw_stars(uint8 i, uint8 x, uint8 y) {

    draw_bitmap_region_fast(bmpSMenu, 64,
        16 + i*16, 16, 16, x*16, y*16);
}


// Save data
static void smenu_save_data() {

    // Open the file & check if exists
    FILE* f = fopen(FPATH, "wb");
    if(f == NULL) {

        return;
    }

    // Store the current stage index
    fwrite(&stageIndex, sizeof(uint8), 1, f);

    // Close
    fclose(f);
}   



// Draw background
static void smenu_draw_bg() {

    clear_screen(0);

    // Draw stars to random positions
    draw_stars(0, 12,4);
    draw_stars(1, 14,2);
    draw_stars(2, 18,3);
    draw_stars(1, 19,1);

    draw_stars(2, 13,9);
    draw_stars(0, 16,11);
    draw_stars(1, 19,7);
    draw_stars(0, 18,8);

    // Draw planet cursor
    draw_bitmap_region(bmpSMenu, 64, 0, 16, 16,
        pcpos.x-8, pcpos.y-8, false);
}


// Draw planet
static void smenu_draw_planet() {

    int16 animPos;

     // Draw planet
    draw_bitmap_region_fast(bmpSMenu,0,0,64,64,
        PLANET_X-32,PLANET_Y-32);

    // Draw marker
    draw_bitmap_region(bmpSMenu, 64, 0, 16, 16,
        pcpos.x-8, pcpos.y-8, false);

    // Draw cursor
    animPos = ANIM_TIME/2 - abs(animTimer-ANIM_TIME/2);
    animPos /= (ANIM_TIME/8);

    // Draw cursor
    draw_bitmap_region(bmpSMenu, 80, 16, 16, 16,
        pcpos.x-8, pcpos.y-20-animPos, false);

}


// Draw menu cursor
static void smenu_draw_cursor(int16 dx, int16 dy, 
    int16 xoff, int16 yoff) {

    // Compute animation positions
    int16 animPos = abs(animTimer-ANIM_TIME/2);
    animPos /= (ANIM_TIME/8);

    // Hide old cursor
    if(fillOldCursorPos) {

        fill_rect(dx-20 + opos.x*xoff, dy-2 + opos.y*yoff, 
            20, 16, 0);

        fillOldCursorPos = false;
    }

    // Draw cursor
    draw_bitmap_region_fast(bmpSMenu, 80, 0, 16, 16, 
        dx-20+animPos + cpos.x*xoff, dy-2 + cpos.y*yoff);
}


// Draw menu
static void smenu_draw_menu() {

    const int16 HEADER_X = 96;
    const int16 HEADER_Y = 16;

    const int16 BOX_W = 128;
    const int16 BOX_H = 16;

    const int16 BUTTON_X = 24;
    const int16 BUTTON_Y = 32;
    const int16 BUTTON_OFF_X = 96;
    const int16 BUTTON_OFF_Y = 16;

    uint16 i;
    uint16 m = (STAGE_COUNT+1) / 2;

    int16 bx = HEADER_X - 64;
    int16 by = HEADER_Y - 4;

    char buf [10];

    // Menu
    if(redrawMenu) {

        // Draw a box around the header
        fill_rect(bx-2, by-2, BOX_W+4, BOX_H+4, 146);
        fill_rect(bx-1, by-1, BOX_W+2, BOX_H+2, 255);
        fill_rect(bx, by, BOX_W, BOX_H, 0);

        // Draw header
        draw_text_fast(bmpFont, "CHOOSE A STAGE", 
            HEADER_X, HEADER_Y, 0, 0, true);

        // Draw stage buttons
        for(i = 0; i < STAGE_COUNT+1; ++ i) {

            if(i > stageIndex)
                snprintf(buf, 10, "LOCKED");
            else
                snprintf(buf, 10, "STAGE %d", i);

            draw_text_fast(bmpFont, i == 0 ? "BACK" : buf, 
                BUTTON_X + (i/m)*BUTTON_OFF_X, 
                HEADER_Y + BUTTON_Y  + BUTTON_OFF_Y*(i%m), 
                0, 0, false);
        }

        redrawMenu = false;
    }

    // Draw menu cursor
    smenu_draw_cursor(BUTTON_X, HEADER_Y + BUTTON_Y, 
        BUTTON_OFF_X, BUTTON_OFF_Y);
}


// Compute planet cursor position
static void smenu_compute_pcursor_pos() {

    static const int16 RADIUS = 11;

    int16 i;

    if(cpos.x == 0 && cpos.y == 0) 
        return;
    
    i = cpos.x * (STAGE_COUNT+1)/2 + cpos.y;

    srand(i);
    pcpos.x = PLANET_X + rand() % (RADIUS*2) - RADIUS;
    pcpos.y = PLANET_Y + rand() % (RADIUS*2) - RADIUS;
}


// Initialize
static int16 smenu_init() {

    // Set defaults
    redrawBG = true;
    redrawMenu = true;
    animTimer = 0;
    stageIndex = 1;

    // Set cursor positions
    pcpos = vec2(320-96+16,100-32+24);
    cpos = vec2(0, 1);
    smenu_compute_pcursor_pos();

    return 0;
}


// Go to stage callback
static void cb_goto_stage() {
    
    if(stageTarget != 0) {

        app_change_scene("game", (void*)stageTarget);
    }
    else {

        app_change_scene("title", NULL);
    }
}


// Esc key cb
static void cb_escape() {

    app_change_scene("title", NULL);
}


// Update
static void smenu_update(int16 steps) {

    const int16 ANIM_SPEED = 1;

    stageTarget = cpos.x * (STAGE_COUNT+1)/2 + cpos.y;

    // Quit (TEMPORARY!)
    if(input_get_button(3) == StatePressed) {

        audio_play(S_BEEP4);

        tr_activate(FadeIn, 2, cb_escape);
        return;
    }

    if(tr_is_active()) return;

    // Enter pressed
    if(input_get_button(2) == StatePressed &&
       stageTarget <= stageIndex) {

        audio_play(S_BEEP3);

        tr_activate(FadeIn, 2, cb_goto_stage);
        return;
    }

    // Animate cursor
    animTimer += ANIM_SPEED * steps;
    animTimer %= ANIM_TIME;

    //
    // Update cursor position
    //
    opos = cpos;
    // Vertical
    if(input_get_arrow_key(ArrowDown) == StatePressed)
        ++ cpos.y;
    else if(input_get_arrow_key(ArrowUp) == StatePressed)
        -- cpos.y;
    cpos.y = neg_mod(cpos.y, (STAGE_COUNT+1)/2);

    // Horizontal
    if(input_get_arrow_key(ArrowRight) == StatePressed)
        ++ cpos.x;
    else if(input_get_arrow_key(ArrowLeft) == StatePressed)
        -- cpos.x;
    cpos.x = neg_mod(cpos.x, 2);

    if(cpos.x != opos.x || cpos.y != opos.y) {

        smenu_compute_pcursor_pos();
        fillOldCursorPos = true;

        audio_play(S_BEEP2);
    }
}


// Draw 
static void smenu_draw() {

    // Background
    if(redrawBG) {

        smenu_draw_bg(); 
        redrawBG = false;
    }

    // Planet
    smenu_draw_planet();

    // Menu
    smenu_draw_menu();
}


// Dispose
static void smenu_dispose() {

    // ...
}


// Change
static void smenu_on_change(void* param) {

    boolean s = (boolean)param;

    if(s && stageTarget == stageIndex) {

        ++ stageIndex;

        // Save data
        smenu_save_data();
    }

    redrawBG = true;
    redrawMenu = true;
}


// Get smenu_ scene
Scene smenu_get_scene() {

    Scene s;
    s.init = smenu_init;
    s.update = smenu_update;
    s.draw = smenu_draw;
    s.dispose = smenu_dispose;
    s.change = smenu_on_change;
    s.name = SMENU_SCENE_NAME;

    return s;
}


// Initialize assets
void smenu_init_assets() {

    // Get bitmaps
    bmpSMenu = (Bitmap*)get_asset("smenu");
    bmpFont = (Bitmap*)get_asset("font");
}


// Load the save data
boolean smenu_load_file() {

    // Open the file & check if exists
    FILE* f = fopen(FPATH, "rb");
    if(f == NULL) {

        return false;
    }

    // Read current stage index
    fread(&stageIndex, sizeof(uint8), 1, f);

    // Close
    fclose(f);

    return true;
}   


// Clear progress
void smenu_clear_progress() {

    // Remove file
    remove(FPATH);

    // Reset values
    stageIndex = 1;
    stageTarget = 1;
    cpos.x = 0;
    cpos.y = 1;
}
