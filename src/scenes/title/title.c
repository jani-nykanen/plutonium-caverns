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

// Flags
static boolean logoLoaded;
static boolean logoDrawn;

// Press enter timer
static int8 enterTimer;
// Phase
static uint8 phase;

// Bitmaps
static Bitmap* bmpLogo;
static Bitmap* bmpFont;

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


// Initialize
static int16 title_init() {

    // Load logo & font
    if(BITMAP("ASSETS/BITMAPS/LOGO.BIN", "logo") ||
       BITMAP("ASSETS/BITMAPS/FONT.BIN", "font")) {

        return 1;
    }
    bmpLogo = (Bitmap*)get_asset("logo");
    bmpFont = (Bitmap*)get_asset("font");

    // Pass data to global components
    init_menus();

    // Set defaults
    logoLoaded = false;
    logoDrawn = false;
    enterTimer = 59;
    phase = 0;

    // Create menu
    title_create_pause_menu();

    return 0;
}


// Update
static void title_update(int16 steps) {

    // Quit
    if(input_get_button(3) == StatePressed) {

        tr_activate(FadeIn, 2, app_terminate);
        return;
    }

    if(tr_is_active()) return;

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

    if(!logoDrawn) {

        clear_screen(0);

        // Draw logo
        draw_bitmap_fast(bmpLogo, 160-bmpLogo->width/2, LOGO_Y);

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
        menu_draw(&menu, 160, ENTER_Y);
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
