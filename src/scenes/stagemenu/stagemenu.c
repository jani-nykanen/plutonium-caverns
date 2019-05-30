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

// Scene name
static const char* SMENU_SCENE_NAME = "smenu";

// Constants
static const uint16 STAGE_COUNT = 15;
static const int16 ANIM_TIME = 120;

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

// Bitmaps
static Bitmap* bmpSMenu;
static Bitmap* bmpFont;


// Draw stars
static void draw_stars(uint8 i, uint8 x, uint8 y) {

    draw_bitmap_region_fast(bmpSMenu, 64,
        16 + i*16, 16, 16, x*16, y*16);
}


// Draw background
static void smenu_draw_bg() {

    clear_screen(0);

    // Draw planet
    draw_bitmap_region_fast(bmpSMenu,0,0,64,64,
        320-96,100-32);

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


// Initialize
static int16 smenu_init() {

    // Load bitmaps
    if(
        BITMAP("ASSETS/BITMAPS/SMENU.BIN", "smenu")
    ) {

        return 1;
    }

    // Get bitmaps
    bmpSMenu = (Bitmap*)get_asset("smenu");
    bmpFont = (Bitmap*)get_asset("font");

    // Set defaults
    redrawBG = true;
    redrawMenu = true;
    pcpos = vec2(320-96+16,100-32+24);
    cpos = vec2(0, 0);
    animTimer = 0;

    return 0;
}


// Update
static void smenu_update(int16 steps) {

    const int16 ANIM_SPEED = 1;

    // Quit (TEMPORARY!)
    if(input_get_button(3) == StatePressed) {

        app_terminate();
        return;
    }

    if(tr_is_active()) return;

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

        fillOldCursorPos = true;
    }
}


// Draw 
static void smenu_draw() {

    // Background
    if(redrawBG) {

        smenu_draw_bg();
        redrawBG = false;
    }

    // Menu
    smenu_draw_menu();
}


// Dispose
static void smenu_dispose() {

    // ...
}


// Change
static void smenu_on_change(void* param) {

    redrawBG = true;
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
