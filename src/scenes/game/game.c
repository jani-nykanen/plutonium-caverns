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

#include "../../menu.h"

#include "stage.h"

// Game scene name
static const char* GAME_SCENE_NAME = "game";

// Bitmaps
static Bitmap* bmpFont;
static Bitmap* bmpItems;

// Game components
static Stage* stage;
static Menu pauseMenu;

// Render flags
static boolean redrawHUD;

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


// Reset callback
static void cb_reset_stage() {

    redrawHUD = true;
    stage_reset(stage);
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
static void cb_audio() {}
static void cb_quit() {
    pauseMenu.active = false;
    stage_redraw(stage);
    tr_activate(FadeIn, 2, app_terminate);
}
// Create pause menu
static void game_create_pause_menu() {

    // Create menu
    pauseMenu = create_menu();
    
    // Add buttons
    menu_add_button(&pauseMenu, "RESUME", cb_resume);
    menu_add_button(&pauseMenu, "RESTART", cb_reset);
    menu_add_button(&pauseMenu, "AUDIO: ON", cb_audio);
    menu_add_button(&pauseMenu, "QUIT", cb_quit);
}


// Initialize
static int16 game_init() {

    // Load bitmaps
    if(
        BITMAP("ASSETS/BITMAPS/FONT.BIN", "font") ||
        BITMAP("ASSETS/BITMAPS/FRAME.BIN", "frame") ||
        BITMAP("ASSETS/BITMAPS/TILESET.BIN", "tileset") ||
        BITMAP("ASSETS/BITMAPS/ANIM.BIN", "anim") ||
        BITMAP("ASSETS/BITMAPS/ITEMS.BIN", "items") ||
        BITMAP("ASSETS/BITMAPS/PLAYER.BIN", "player") ||
        BITMAP("ASSETS/BITMAPS/EXP.BIN", "exp")
    ) {

        return 1;
    }

    // Get bitmaps
    bmpFont = (Bitmap*) get_asset("font");
    bmpItems = (Bitmap*) get_asset("items");

    // Create main components
    stage = create_stage();
    if(stage == NULL) {

        return 1;
    }

    // Initialize components
    init_boulders();
    init_players();
    if(stage_init(stage, "ASSETS/MAPS/0.BIN") == 1) {

        return 1;
    }

    // Set defaults
    redrawHUD = true;

    // Create pause menu
    game_create_pause_menu();

    // Set transition
    tr_activate(FadeOut, 2, NULL);

    return 0;
}


// Update
static void game_update(int16 steps) {

    // Quit (TEMPORARY!)
    if(input_get_button(3) == StatePressed) {

        cb_quit();
        return;
    }

    if(tr_is_active()) return;

    // Check pause
    if(pauseMenu.active) {

        menu_update(&pauseMenu, steps);
        return;
    }
    // Pause game
    else if(input_get_button(2) == StatePressed) {

        menu_activate(&pauseMenu, 0);
        return;
    }

    // Reset stage
    if(input_get_button(1) == StatePressed) {

        tr_activate(FadeIn, 2, cb_reset_stage);
        return;
    }

    // Update stage
    stage_update(stage, steps);
}


// Draw 
static void game_draw() {

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


// Draw info
void game_redraw_info(Player* pl) {

    const int TOP_X = 24*8+24;
    const int TOP_Y = 24;
    const int FRAME_WIDTH = 80;

    const int ITEM_START_Y = 28;
    const int ITEM_X = 4;
    const int ITEM_OFF_X = 20;
    const int ITEM_OFF_Y = 22;

    const int BAR_MAX = 5;

    uint8 i = 0;
    char buf[8];

    // Draw stage name
    draw_text_fast(bmpFont, "STAGE 1", 
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
    snprintf(buf, 8, "\2%d", (int16)pl->keys);
    draw_text_fast(bmpFont, buf, 
        TOP_X+ITEM_X+ITEM_OFF_X, 
        TOP_Y+ITEM_START_Y+ITEM_OFF_Y*2 +4, 
        0, 0, false);
    snprintf(buf, 8, "\2%d", (int16)pl->bombs);
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
