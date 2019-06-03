// Title scene
// (c) 2019 Jani Nykänen

#include "title.h"

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

#include "../../menu.h"

#include "../game/game.h"
#include "../stagemenu/stagemenu.h"

// Scene name
static const char* TITLE_SCENE_NAME = "title";

// Constants
static int16 INITIAL_LOGO_TIME = 120;

// Flags
static boolean logoLoaded;
static boolean logoDrawn;
static boolean bgDrawn;

// Logo timer
static int16 logoTimer;
// Press enter timer
static int8 enterTimer;
// Phase
static uint8 phase;

// Bitmaps
static Bitmap* bmpLogo;
static Bitmap* bmpFont;
static Bitmap* bmpSMenu;

// Menu
static Menu menu;

// Start game
static void start_game() {

    // Clear data
    ass_remove("logo");
    logoLoaded = false;

    // Load game assets
    if(game_load_assets() == 1) {

        app_terminate();
        return;
    }

    // Reinitialize
    smenu_init_assets();

    app_change_scene("smenu", NULL);
}

// Callbacks
static void cb_start() {

    tr_activate(FadeIn, 2, start_game);
}
static void cb_audio() {

}
static void cb_clear() {

}
static void cb_quit() {

    tr_activate(FadeIn, 2, app_terminate);
}


// Create pause menu
static void title_create_pause_menu() {

    // Create menu
    menu = create_menu(3);
    
    // Add buttons
    menu_add_button(&menu, "START GAME", cb_start);
    menu_add_button(&menu, "AUDIO: ON", cb_audio);
    menu_add_button(&menu, "CLEAR DATA", cb_clear);
    menu_add_button(&menu, "QUIT GAME", cb_quit);
}


// Draw stars
static void draw_stars(uint8 i, uint8 x, uint8 y) {

    draw_bitmap_region_fast(bmpSMenu, 64,
        16 + i*16, 16, 16, x*16, y*16);
}


// Draw background
static void title_draw_background() {

    const int16 PLANET_X = 320-80;
    const int16 PLANET_Y = 96;

    clear_screen(0);

    // Draw planet
    draw_bitmap_region_fast(bmpSMenu, 0, 0, 64, 64, 
        PLANET_X, PLANET_Y);

    // Draw stars
    draw_stars(0, 2, 4);
    draw_stars(1, 1, 2);
    draw_stars(0, 1, 6);

    draw_stars(2, 4, 7);
    draw_stars(1, 1, 9);
    draw_stars(0, 3, 10);

    draw_stars(0, 16, 3);
    draw_stars(1, 19, 1);
    draw_stars(2, 18, 5);
}


// Initialize
static int16 title_init() {

    // Load global stuff (or "global enough")
    if( BITMAP("ASSETS/BITMAPS/FONT.BIN", "font") ||
       BITMAP("ASSETS/BITMAPS/SMENU.BIN", "smenu")) {

        return 1;
    }
    bmpFont = (Bitmap*)get_asset("font");
    bmpSMenu = (Bitmap*)get_asset("smenu");

    // Pass data to global components
    init_menus();

    // Set defaults
    logoLoaded = false;
    logoDrawn = false;
    bgDrawn = false;
    enterTimer = 59;
    phase = 0;
    logoTimer = INITIAL_LOGO_TIME;

    // Create menu
    title_create_pause_menu();

    // Set transition
    tr_activate(FadeOut, 1, NULL);

    return 0;
}


// Update
static void title_update(int16 steps) {

    const int16 LOGO_SPEED = 2;

    // Quit
    if(input_get_button(3) == StatePressed) {

        tr_activate(FadeIn, 2, app_terminate);
        return;
    }

    if(tr_is_active())  {

        // Update logo timer
        logoTimer -= LOGO_SPEED * steps;
        if(logoTimer < 0)
            logoTimer = 0;

        return;
    }
    else {

        logoTimer = 0;
    }

    if(phase == 0) {

        // Update enter timer
        enterTimer += steps;
        if(enterTimer >= 60) {

            enterTimer -= 60;
        }

        // Check enter key
        if(input_get_button(2) == StatePressed) {

            ++ phase;
            menu_activate(&menu, 0);
        }

    }
    else if(phase == 1) {

        menu_update(&menu, steps);
    }   
}


// Draw 
static void title_draw() {

    const int16 LOGO_Y = 16;
    const int16 COPYRIGHT_Y = -10;
    const int16 ENTER_Y = 144;
    const int16 MENU_Y = 136;

    toggle_clipping(true);

    if(!bgDrawn) {

        title_draw_background();
        bgDrawn = true;
    }

    if(!logoDrawn || logoTimer > 0) {

        // Draw logo
        fill_rect(160-bmpLogo->width/2, LOGO_Y-logoTimer-16, 
            bmpLogo->width, 16, 0);
        draw_bitmap_fast(bmpLogo, 160-bmpLogo->width/2, LOGO_Y-logoTimer);

        // Draw copyright
        draw_text_fast(bmpFont, "\4 2019 JANI NYK\5NEN", 
            160, 200+COPYRIGHT_Y, 0, 0, true);

        logoDrawn = true;
    }

    if(phase == 0) {

        // Draw "PRESS ENTER"
        if(enterTimer < 30) {

            draw_text_fast(bmpFont, "PRESS ENTER", 
                        160, ENTER_Y, 0, 0, true);
        }
        else {

            fill_rect(160 - 8*6,ENTER_Y-8, 16*6, 16, 0);
        }

    }
    else {

        // Draw menu
        menu_draw(&menu, 160, MENU_Y);
    }
}


// Dispose
static void title_dispose() {

    // ...
}


// Change
static void title_on_change(void* param) {

    // Remove game assets
    game_clear_assets();

    // Load own assets
    if(!logoLoaded) {

        // Load logo
        if(BITMAP("ASSETS/BITMAPS/LOGO.BIN", "logo")) {

            app_terminate();
            return;
        }
        bmpLogo = (Bitmap*)get_asset("logo");
        bmpFont = (Bitmap*)get_asset("font");
    }
    logoLoaded = true;
    logoDrawn = false;
    bgDrawn = false;

    menu.redraw = true;
}



// Get title menu scene
Scene title_get_scene() {

    Scene s;
    s.init = title_init;
    s.update = title_update;
    s.draw = title_draw;
    s.dispose = title_dispose;
    s.change = title_on_change;
    s.name = TITLE_SCENE_NAME;

    return s;
}
