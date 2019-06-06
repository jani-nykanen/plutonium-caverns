// Game scene
// (c) 2019 Jani Nykänen

#include "game.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../core/graphics.h"
#include "../../core/input.h"
#include "../../core/application.h"
#include "../../core/assets.h"
#include "../../core/transition.h"
#include "../../core/err.h"
#include "../../core/audio.h"

#include "../../menu.h"

#include "stage.h"

// Game scene name
static const char* GAME_SCENE_NAME = "game";

// Constants
static const int16 CLEAR_TIME = 120;

// Bitmaps
static Bitmap* bmpFont;
static Bitmap* bmpItems;

// Game components
static Stage* stage;
static Menu pauseMenu;

// Render flags
static boolean redrawHUD;
static boolean redrawClear;
// Other flags
static boolean stageClear;
static boolean assetsLoaded;

// Stage clear timer
static int16 clearTimer;

// Stage index
static uint8 stageIndex;


// Draw an energy bar
static void draw_energy_bar(int x, int y, uint8 max, uint8 val) {

    uint8 i;
    int16 sx = 3 + val*16;
    // Draw corresponding enery bars
    for(i = 0; i < max; ++ i) {

        draw_bitmap_region_fast(bmpItems, 
            (i < val ? sx : 3), 33,
            10, 14, 
            x+i*9, 
            y+1);
    }
}


// Draw stage clear
static void game_draw_stage_clear() {

    const int16 WIDTH = 8*14;
    const int16 HEIGHT = 3*8;

    int16 dx = stage->topLeft.x + stage->width*8;
    int16 dy = stage->topLeft.y + stage->height*8;

    // Draw box
    fill_rect(dx-WIDTH/2-2, dy-HEIGHT/2-2, WIDTH+4, HEIGHT+4, 146);
    fill_rect(dx-WIDTH/2-1, dy-HEIGHT/2-1, WIDTH+2, HEIGHT+2, 255);
    fill_rect(dx-WIDTH/2, dy-HEIGHT/2, WIDTH, HEIGHT, 0);

    // Draw text
    draw_text_fast(bmpFont, "STAGE CLEAR", dx, dy-4, 0, 0, true);
}


// Reset callback
static void cb_reset_stage() {

    redrawHUD = true;
    stage_reset(stage);
}
// Change scene callback
static void cb_change() {

    boolean state = stage->pl.gems == stage->pl.maxGems;
    app_change_scene("smenu", (void*)state);
}

// Menu callbacks
static void cb_resume() {

    pauseMenu.active = false;
    stage_redraw(stage);
}
static void cb_reset() {

    pauseMenu.active = false;
    stage_redraw(stage);
    tr_activate(FadeIn, 2, cb_reset_stage);
    
}
static void cb_audio() {

    // Toggle audio & change the text
    boolean audioEnabled = audio_enabled();
    audio_toggle();
    if(!audioEnabled) {
        pauseMenu.text[2] [9] = 'N';
        pauseMenu.text[2] [10] = ' ';
    }
    else {
        pauseMenu.text[2] [9] = 'F';
        pauseMenu.text[2] [10] = 'F';
    }
    pauseMenu.redraw = true;
}
static void cb_quit() {
    pauseMenu.active = false;
    stage_redraw(stage);
    tr_activate(FadeIn, 2, cb_change);
}
// Create pause menu
static void game_create_pause_menu() {

    // Create menu
    pauseMenu = create_menu(0);
    
    // Add buttons
    menu_add_button(&pauseMenu, "RESUME", cb_resume);
    menu_add_button(&pauseMenu, "RESTART", cb_reset);
    menu_add_button(&pauseMenu, "AUDIO: ON ", cb_audio);
    menu_add_button(&pauseMenu, "QUIT", cb_quit);
}


// Initialize
static int16 game_init() {

    // Create main components
    stage = create_stage();
    if(stage == NULL) {

        return 1;
    }

    // Set defaults
    redrawHUD = true;
    clearTimer = 0;
    stageClear = false;
    redrawClear = false;
    assetsLoaded = false;

    // Create pause menu
    game_create_pause_menu();

    return 0;
}


// Victory callback
static void cb_win() {

    // Clear assets
    game_clear_assets();

    // Go to the story screen
    app_change_scene("story", (void*)1);
}


// Update
static void game_update(int16 steps) {

    if(tr_is_active()) return;

    // Stage clear
    if(stageClear) {

        clearTimer -= 1 * steps;
        if(clearTimer <= 0) {

            tr_activate(FadeIn, 2, cb_change);
        }

        return;
    }

    // Check pause
    if(pauseMenu.active) {

        menu_update(&pauseMenu, steps);
        return;
    }
    // Pause game
    else if(input_get_button(2) == StatePressed ||
            input_get_button(3) == StatePressed) {

        menu_activate(&pauseMenu, 0);

        // Sound
        audio_play(S_BEEP1);

        return;
    }

    // Reset stage
    if(input_get_button(1) == StatePressed) {

        tr_activate(FadeIn, 2, cb_reset_stage);
        audio_play(S_BEEP1);

        return;
    }

    // Update stage
    stage_update(stage, steps);

    // Check if the stage is clear
    if(stage->pl.maxGems > 0 && stage->pl.gems == stage->pl.maxGems) {

        stageClear = true;
        redrawClear = true;
        clearTimer = CLEAR_TIME;

        // Sound
        audio_play(S_VICTORY);
    }

    // Check if the game is beaten
    if(stage->pl.victory) {

        tr_activate(FadeIn, 1, cb_win);
    }
}


// Draw 
static void game_draw() {

    // Draw stage clear
    if(stageClear) {

        if(redrawClear) {

            game_draw_stage_clear();
            redrawClear = false;
        }
        return;
    }

    if(pauseMenu.active) {

        // Draw pause
        menu_draw(&pauseMenu, 
            stage->topLeft.x + stage->width*8, 
            stage->topLeft.y + stage->height*8);
        return;
    }

    // Draw stage
    stage_draw(stage);

    // (Re)draw info
    if(redrawHUD) {

        game_redraw_info(&stage->pl);
        redrawHUD = false;
    }

}


// Dispose
static void game_dispose() {

    // Destroy allocated data
    destroy_stage(stage);
}


// Change
static void game_on_change(void* param) {

    char path[32];
    snprintf(path, 32, "ASSETS/MAPS/%d.BIN", (int16)param);

    stage_refactor(stage);
    if(stage_init(stage, path) == 1) {

        // TODO: Error handling...
        app_terminate();
    }

    // Set re-render flags
    redrawHUD = true;
    redrawClear = false;
    stageClear = false;

    stageIndex = (uint8)param;

    // Set audio button text
    audio_toggle();
    cb_audio();
}


// Get game scene
Scene game_get_scene() {

    Scene s;
    s.init = game_init;
    s.update = game_update;
    s.draw = game_draw;
    s.dispose = game_dispose;
    s.change = game_on_change;
    s.name = GAME_SCENE_NAME;

    return s;
}


// Draw info
void game_redraw_info(Player* pl) {

    const int TOP_X = 24*8+24;
    const int TOP_Y = 24;
    const int FRAME_WIDTH = 80;

    const int ITEM_START_Y = 28;
    const int ITEM_X = 2;
    const int ITEM_OFF_X = 20;
    const int ITEM_OFF_Y = 22;

    const int BAR_MAX = 5;

    uint8 i = 0;
    char buf[10];

    // Draw stage name
    snprintf(buf, 10, "STAGE %d", (int16)stageIndex);
    draw_text_fast(bmpFont, buf, 
        TOP_X + FRAME_WIDTH/2, TOP_Y+8,0,0, true);

    // Draw consumable item icons
    draw_bitmap_region_fast(bmpItems, 16, 0, 16, 16,
        TOP_X+ITEM_X, TOP_Y+ITEM_START_Y);
    draw_bitmap_region_fast(bmpItems, 32, 0, 16, 16,
        TOP_X+ITEM_X, TOP_Y+ITEM_START_Y+ITEM_OFF_Y);

    // Draw corresponding energy bars
    draw_energy_bar(TOP_X+ITEM_X+ITEM_OFF_X, 
        TOP_Y+ITEM_START_Y+1, 
        BAR_MAX, pl->pickaxe);
    draw_energy_bar(TOP_X+ITEM_X+ITEM_OFF_X, 
        TOP_Y+ITEM_START_Y+ITEM_OFF_Y+1, 
        BAR_MAX, pl->shovel);

    // Draw collectable item icons
    draw_bitmap_region_fast(bmpItems, 0, 0, 16, 16,
        TOP_X+ITEM_X, TOP_Y+ITEM_START_Y+ITEM_OFF_Y*2);
    draw_bitmap_region_fast(bmpItems, 48, 0, 16, 16,
        TOP_X+ITEM_X, TOP_Y+ITEM_START_Y+ITEM_OFF_Y*3);

    // Draw collectable item counts
    snprintf(buf, 10, "\2%d", (int16)pl->keys);
    draw_text_fast(bmpFont, buf, 
        TOP_X+ITEM_X+ITEM_OFF_X, 
        TOP_Y+ITEM_START_Y+ITEM_OFF_Y*2 +4, 
        0, 0, false);
    snprintf(buf, 10, "\2%d", (int16)pl->bombs);
    draw_text_fast(bmpFont, buf, 
        TOP_X+ITEM_X+ITEM_OFF_X, 
        TOP_Y+ITEM_START_Y+ITEM_OFF_Y*3 +4, 
        0, 0, false);

    // Draw gems
    for(i = 0; i < pl->maxGems; ++ i) {

        draw_bitmap_region_fast(bmpItems, 
            i < pl->gems ? 64 : 80, 0, 16, 16,
            TOP_X+ITEM_X+i*ITEM_OFF_X, 
            TOP_Y+ITEM_START_Y+ITEM_OFF_Y*4);
    }
}


// Clear game assets
void game_clear_assets() {
    
    // Not working, omitting
    if(assetsLoaded) {

        ass_remove("frame");
        ass_remove("tileset");
        ass_remove("anim");
        ass_remove("items");
        ass_remove("player");
        ass_remove("exp");
        ass_remove("ship");
    }
    assetsLoaded = false;
}


// Load assets
int16 game_load_assets() {

    if(assetsLoaded) {

        return 0;
    }
    else {

        assetsLoaded = true;
        if(
            BITMAP("ASSETS/BITMAPS/FRAME.BIN", "frame") ||
            BITMAP("ASSETS/BITMAPS/TILESET.BIN", "tileset") ||
            BITMAP("ASSETS/BITMAPS/ANIM.BIN", "anim") ||
            BITMAP("ASSETS/BITMAPS/ITEMS.BIN", "items") ||
            BITMAP("ASSETS/BITMAPS/PLAYER.BIN", "player") ||
            BITMAP("ASSETS/BITMAPS/EXP.BIN", "exp") ||
            BITMAP("ASSETS/BITMAPS/SHIP.BIN", "ship")) {

            return 1;
        }

        // Get bitmaps
        bmpFont = (Bitmap*) get_asset("font");
        bmpItems = (Bitmap*) get_asset("items");

        // Pass bitmaps to the game components
        stage_init_assets(stage);
        init_boulders();
        init_players();

        return 0;

    }   
}
