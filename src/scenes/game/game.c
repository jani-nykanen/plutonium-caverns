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

// Bitmaps
static Bitmap* bmpFont;
static Bitmap* bmpItems;

// Game components
static Stage* stage;

// Item counts
static uint8 pickaxe;
static uint8 shovel;
static uint8 bombs;
static uint8 keys;
static uint8 gems;
static uint8 maxGems;

// Render flags
static boolean redrawHUD;


// Draw an energy bar
static void draw_energy_bar(int x, int y, uint8 max, uint8 val) {

    uint8 i;
    // Draw corresponding enery bars
    for(i = 0; i < max; ++ i) {

        draw_bitmap_region_fast(bmpItems, 
            (i < val ? 19 : 3), 33,
            10, 14, 
            x+i*9, 
            y+1);
    }
}


// Draw HUD
static void game_draw_hud() {

    const int TOP_X = 24*8+24;
    const int TOP_Y = 24;
    const int FRAME_WIDTH = 80;

    const int ITEM_START_Y = 28;
    const int ITEM_X = 4;
    const int ITEM_OFF_X = 20;
    const int ITEM_OFF_Y = 22;

    const int BAR_MAX = 5;

    uint8 i = 0;

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
        BAR_MAX, pickaxe);
    draw_energy_bar(TOP_X+ITEM_X+ITEM_OFF_X, 
        TOP_Y+ITEM_START_Y+ITEM_OFF_Y+1, 
        BAR_MAX, shovel);

    // Draw collectable item icons
    draw_bitmap_region_fast(bmpItems, 48, 0, 16, 16,
        TOP_X+ITEM_X, TOP_Y+ITEM_START_Y+ITEM_OFF_Y*2);
    draw_bitmap_region_fast(bmpItems, 0, 0, 16, 16,
        TOP_X+ITEM_X, TOP_Y+ITEM_START_Y+ITEM_OFF_Y*3);

    // Draw collectable item counts
    draw_text_fast(bmpFont, "\2\60", 
        TOP_X+ITEM_X+ITEM_OFF_X, 
        TOP_Y+ITEM_START_Y+ITEM_OFF_Y*2 +4, 
        0, 0, false);
    draw_text_fast(bmpFont, "\2\60", 
        TOP_X+ITEM_X+ITEM_OFF_X, 
        TOP_Y+ITEM_START_Y+ITEM_OFF_Y*3 +4, 
        0, 0, false);

    // Draw gems
    for(i = 0; i < 2; ++ i) {

        draw_bitmap_region_fast(bmpItems, 80, 0, 16, 16,
            TOP_X+ITEM_X+i*ITEM_OFF_X, 
            TOP_Y+ITEM_START_Y+ITEM_OFF_Y*4);
    }
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
        BITMAP("ASSETS/BITMAPS/PLAYER.BIN", "player")
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

    // Get object counts
    pickaxe = stage->tmap->layers[0][0] -1;
    shovel = stage->tmap->layers[0][1] -1;
    bombs = stage->tmap->layers[0][2] -1;
    keys = 0;
    gems = 0;
    maxGems = 2; // TODO: Read from tilemap!

    // Set defaults
    redrawHUD = true;

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

    // Draw HUD
    if(redrawHUD) {

        game_draw_hud();
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
