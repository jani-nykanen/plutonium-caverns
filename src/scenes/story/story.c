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
#include "../../core/audio.h"

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

// Story pointer
static uint8 storyPointer;
// Character timer
static int16 chrTimer;
// Character pos
static int16 chrPos;
// Text length
static uint16 len;

// Story textes
static const char* STORY[] = { 
"YOU ARE AN ALIEN SPACE TRAVELER, FAR\n"
"AWAY FROM HOME. YOU HAVE STRANDED A\n"
"A MYSTERIOUS PLANET. YOU MUST EXPLORE\n"
"DARK CAVERNS AND COLLECT PLUTONIUM\n"
"GEMS TO FUEL UP YOUR SHIP AND ESCAPE\n"
"THIS TERRIBLE PLANET."
,
"WITH THE POWER OF PLUTONIUM\n"
"YOU FUEL YOU SHIP AND ESCAPE THIS\n"
"COLD, COLD PLANET. NOW YOU CAN\n"
"FINALLY RETURN YOUR HOME, A PLANET\n"
"CALLED... EARTH.\n"

};


// Go to the stage menu
static void cb_go_to_stage() {

    // Remove picture
    destroy_bitmap(bmpStory);

    // Reinitialize assets
    smenu_init_assets();

    // Change scene
    app_change_scene(storyPointer == 0 ? "smenu" : "intro", (void*)0);
}


// Initialize
static int16 story_init() {

    // Get font
    bmpFont = (Bitmap*)get_asset("font");

    // Set defaults
    bgDrawn = false;
    chrTimer = LETTER_TIME;
    chrPos = 0;

    len = strlen(STORY[storyPointer]);
    
    return 0;
}


// Update
static void story_update(int16 steps) {

    if(tr_is_active()) return;

    // Check enter or esc keys
    if(input_get_button(2) == StatePressed ||
       input_get_button(3) == StatePressed) {

        tr_activate(FadeIn, 2, cb_go_to_stage);

        // Sound
        audio_play(S_BEEP3);
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

    if(!tr_is_active()) {
        
        // Draw text (temp)
        draw_substr_fast(bmpFont, STORY[storyPointer], STORY_X, STORY_Y, 0 , 1,
            chrPos, chrPos+1, false);
    }
}


// Dispose
static void story_dispose() {

    // ...
}


// Change
static void story_on_change(void* param) {

    // Load intro bitmap
    bmpStory = load_bitmap(
        (uint8)param == 0 ?
        "ASSETS/BITMAPS/STORY1.BIN" :
        "ASSETS/BITMAPS/STORY2.BIN"
        );
    if(bmpStory == NULL) {

        app_terminate();
    }

    // Reset values
    bgDrawn = false;
    chrTimer = LETTER_TIME;
    chrPos = 0;

    storyPointer = (uint8)param;
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
