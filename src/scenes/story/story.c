// Story scene
// (c) 2019 Jani Nykänen

#include "story.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "../../core/graphics.h"
#include "../../core/input.h"
#include "../../core/application.h"
#include "../../core/assets.h"
#include "../../core/transition.h"
#include "../../core/mathext.h"

#include "../stagemenu/stagemenu.h"

// Scene name
static const char* STORY_SCENE_NAME = "story";

// Constants
static const int16 LETTER_TIME = 3;

// Bitmaps
static Bitmap* bmpFont;
static Bitmap* bmpStory;

// Is the background drawn
static boolean bgDrawn;

// Character timer
static int16 chrTimer;
// Character pos
static int16 chrPos;
// Text length
static uint16 len;

// Story textes
static const char* STORY_1 = 
"YOU ARE AN ALIEN SPACE TRAVELER, FAR\n"
"AWAY FROM HOME. YOU WERE EXPLORING A\n"
"MYSTERIOUS DWARF PLANET NAMED \"PLUTO\".\n"
"SUDDENLY YOUR SHIP RAN OUT OF FUEL AND\n"
"YOU HAD TO LAND THE SURFACE OF THIS\n"
"COLD PLANET. TO GET YOUR SHIP WORKING\n"
"AGAIN, YOU MUST EXPLORE THE DARK\n"
"CAVERNS OF PLUTO AND TRY TO FIND\n"
"ENOUGH PLUTONIUM GEMS TO FUEL YOUR\n"
"SHIP AND RETURN TO YOUR HOME PLANET.";


// Go to the stage menu
static void cb_go_to_stage() {

    // Remove picture
    destroy_bitmap(bmpStory);

    // Reinitialize assets
    smenu_init_assets();

    // Change scene
    app_change_scene("smenu", (void*)0);
}


// Initialize
static int16 story_init() {

    // Get font
    bmpFont = (Bitmap*)get_asset("font");

    // Set defaults
    bgDrawn = false;
    chrTimer = LETTER_TIME;
    chrPos = 0;

    len = strlen(STORY_1);
    
    return 0;
}


// Update
static void story_update(int16 steps) {

    if(tr_is_active()) return;

    // Check enter or esc keys
    if(input_get_button(2) == StatePressed ||
       input_get_button(3) == StatePressed) {

        tr_activate(FadeIn, 2, cb_go_to_stage);
    }

    // Update char timer
    if(chrPos <= len &&
        (chrTimer -= steps) <= 0) {

        chrTimer += LETTER_TIME;
        ++ chrPos;
    }
}


// Draw 
static void story_draw() {

    const int16 PICTURE_Y = 16;
    const int16 STORY_Y = 96;
    const int16 STORY_X = 8;

    // Draw background
    if(!bgDrawn) {

        clear_screen(0);
        
        draw_bitmap_fast(bmpStory, 
            160-bmpStory->width/2, 
            PICTURE_Y);

        bgDrawn = true;

        
    }

    // Draw text (temp)
    draw_substr_fast(bmpFont, STORY_1, STORY_X, STORY_Y, 0 , 1,
        chrPos, chrPos+1, false);
}


// Dispose
static void story_dispose() {

    // ...
}


// Change
static void story_on_change(void* param) {

    // Load intro bitmap
    bmpStory = load_bitmap("ASSETS/BITMAPS/STORY1.BIN");
    if(bmpStory == NULL) {

        app_terminate();
    }

    // Reset values
    bgDrawn = false;
    chrTimer = LETTER_TIME;
    chrPos = 0;
}



// Get title menu scene
Scene story_get_scene() {

    Scene s;
    s.init = story_init;
    s.update = story_update;
    s.draw = story_draw;
    s.dispose = story_dispose;
    s.change = story_on_change;
    s.name = STORY_SCENE_NAME;

    return s;
}