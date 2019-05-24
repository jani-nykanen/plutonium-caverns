// Game stage
// (c) 2019 Jani Nykänen

#include "stage.h"

#include "../../core/err.h"
#include "../../core/graphics.h"
#include "../../core/assets.h"

#include <stdlib.h>
#include <stdio.h>


// Draw frame
static void draw_box_frame(Bitmap* bmp, 
    int dx, int dy, int w, int h, uint8 c) {

    const int TILE_W = 8;
    const int TILE_H = 8;

    int x = 0;
    int y = 0;
    int sx = 0;
    int sy = 0;

    for(; y < h; ++ y) {

        for(x = 0; x < w; ++ x) {

            // Skip, if empty
            if(x == 0+1 && y > 0 && y < 0+h-1)
                x += w-2;

            // Determine tile source position
            if(x == 0 && y == 0) {

                sx = 0; sy = 0;
            }
            else if(x == w-1 && y == 0) {

                sx = 16; sy = 0;
            }
            else if(x == w-1 && y == h-1) {

                sx = 16; sy = 16;
            }
            else if(x == 0 && y == h-1) {

                sx = 0; sy = 16;
            }
            else if(y == 0) {

                sx = 8; sy = 0;
            }
            else if(y == h-1) {

                sx = 8; sy = 16;
            }
            else if(x == 0) {

                sx = 0; sy = 8;
            }
            else if(x == w-1) {

                sx = 16; sy = 8;
            }

            // Draw tile
            draw_bitmap_region_fast(bmp, sx, sy, 
                TILE_W, TILE_H, dx+x*TILE_W, dy+y*TILE_H);
        }
    }

    // Fill with black
    fill_rect(dx+TILE_W, dy+TILE_W,
        (w-2)*TILE_W, (h-2)*TILE_H, c);

    // Draw shadows
    fill_rect(dx + w*TILE_W, dy+TILE_H, 
        TILE_W, h*TILE_H, 51);
    fill_rect(dx + TILE_W, dy + h*TILE_H, 
        (w-1)*TILE_W, TILE_H, 51);
}


// Draw static tiles
static void stage_draw_static(Stage* s, int dx, int dy) {

    int x = 0;
    int y = 0;

    int sx = 0;
    int sy = 0;

    int t;
    Bitmap* bmp;

    for(; y < s->height; ++ y) {

        for(x = 0; x < s->width; ++ x) {

            t = s->data[y*s->width+x] ;
            if(t < 1)
                continue;

            bmp = s->bmpTileset;
            switch (t)
            {
            // Wall
            case 1:
                sx = 0; sy = 0;
                break;
            // Ice wall
            case 2:
                sx = 0; sy = 16;
                break;
            // Frozen boulder
            case 3:
                sx = 0; sy = 32;
                break;
            // Bomb place
            case 6:
                sx = 112; sy = 16;
                break;
            // Lock
            case 7:
                sx = 112; sy = 48;
                break;
            // Color blocks
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
                sx = 16 + (t-8)*16;
                sy = 48;
                break;
                
            // Switches
            case 14:
            case 15:
            case 16:
            case 30:
            case 31:
            case 32:
                sx = 16 + (t/30)*16 + (t-14)*32;
                sy = 32;
                break;

            // Items
            case 18:
            case 19:
            case 20:
            case 21:
            case 22:
                sx = (t-18)*16;
                sy = 0;
                bmp = s->bmpItems;
                break;

            default:
                continue;
            }

            draw_bitmap_region_fast(bmp,
                sx, sy, 16, 16, dx + x*16, dy + y*16);
        }
    }
}


// Draw lava
static void stage_draw_lava(Stage* s, int dx, int dy) {

    int x = 0;
    int y = 0;

    int t = s->lavaTimer / FIXED_PREC;
    int sx = 16+t;
    int sy = 16-t;
    int p = s->lavaGlowTimer / FIXED_PREC;
    if(p >= 3) p = 1;
    sx += 32 * p;

    for(; y < s->height; ++ y) {

        for(x = 0; x < s->width; ++ x) {

            if(s->data[y*s->width+x] != 4)
                continue;

            draw_bitmap_region_fast(s->bmpTileset,
                sx, sy, 16, 16, dx + x*16, dy + y*16);
        }
    }
}


// Add a boulder
static void stage_add_boulder(Stage* s, uint8 x, uint8 y) {

    uint8 i = 0;
    // Find the first boulder that does
    // not exist
    for(; i < s->bcount; ++ i) {

        if(!s->boulders[i].exist) {

            s->boulders[i] = create_boulder(x, y, false);
            break;
        }
    }
}


// Parse objects
static void stage_parse_objects(Stage* s) {

    uint8 x, y;
    uint8 t;
    for(y = 0; y < s->height; ++ y) {

        for(x = 0; x < s->width; ++ x) {

            t = s->data[y*s->width+x];

            // Boulder
            if(t == 5) {

                stage_add_boulder(s, x, y);
            }
            // Player
            else if(t == 17) {

                s->pl = create_player(x, y);
            }
        }
    }
}


// Create a stage object
Stage* create_stage() {

    // Allocate memory
    Stage* s = (Stage*)malloc(sizeof(Stage));
    if(s == NULL) {

        THROW_MALLOC_ERR;
        return NULL;
    }

    // Get bitmaps
    s->bmpFrame = (Bitmap*)get_asset("frame");
    s->bmpTileset = (Bitmap*)get_asset("tileset");
    s->bmpItems = (Bitmap*)get_asset("items");

    return s;
}


// Initialize a stage
int stage_init(Stage* s, const char* mapPath) {

    int i, tileid;
    int size;

    // Load map
    Tilemap* t = load_tilemap(mapPath);
    if(t == NULL) {

        return 1;
    }

    // Copy data & compute the amount of
    // boulders
    size = t->width*(t->height-1);
    s->data = (uint8*)malloc(sizeof(uint8)*size);
    s->solid = (uint8*)malloc(sizeof(uint8)*size);
    if(s->data == NULL || s->solid == NULL) {

        THROW_MALLOC_ERR;
        return 1;
    }
    s->bcount = 0;
    for(i = 0; i < size; ++ i) {

        tileid = t->layers[0] [i+t->width] -16;
        s->data[i] = tileid;
        s->solid[i] = 0;

        // If boulder
        if(tileid == 5 || tileid == 3) {

            ++ s->bcount;
        }
    }
    s->width = t->width;
    s->height = t->height-1;

    // Set defaults
    s->frameDrawn = false;
    s->staticDrawn = false;
    s->lavaTimer = 0;
    s->lavaGlowTimer = 0;

    // Allocate memory
    s->boulders = (Boulder*)calloc(s->bcount, sizeof(Boulder));
    if(s->boulders == NULL) {

        THROW_MALLOC_ERR;
        return 1;
    }
    // Make inactive
    for(i = 0; i < s->bcount; ++ i) {

        s->boulders[i].exist = false;
    }
    // Parse objects
    stage_parse_objects(s);

    return 0;
}


// Update stage
void stage_update(Stage* s, int steps) {

    const int LAVA_SPEED = 8;
    const int LAVA_GLOW_SPEED = 4;

    uint8 i;

    // Update lava timers
    s->lavaTimer += LAVA_SPEED * steps;
    s->lavaTimer %= 16 * FIXED_PREC;

    s->lavaGlowTimer += LAVA_GLOW_SPEED * steps;
    s->lavaGlowTimer %= 4 * FIXED_PREC;

    // Update boulders
    for(i = 0; i < s->bcount; ++ i) {

        boulder_update(&s->boulders[i], NULL, steps);
    }

    // Update players
    pl_update(&s->pl, steps);
}


// Draw stage
void stage_draw(Stage* s) {

    const int RIGHT_FRAME_WIDTH = 12;

    int w;
    uint8 i;
    int topx = 24;
    int topy = 16;

    // Draw frames
    if(!s->frameDrawn) {

        // Clear background
        clear_screen(123);

        // Left
        w = s->width*2 +2;
        draw_box_frame(s->bmpFrame,
            16, 8, w, s->height*2 +2, 0);

        // Right
        draw_box_frame(s->bmpFrame,
            (w+2)*8+16, 8, 
            RIGHT_FRAME_WIDTH, s->height*2 +2, 0);

        s->frameDrawn = true;
    }

    // Draw static tiles
    if(!s->staticDrawn) {

        stage_draw_static(s, topx, topy);
        s->staticDrawn = true;
    }

    // Draw lava
    stage_draw_lava(s, topx, topy);

    // Draw boulders
    for(i = 0; i < s->bcount; ++ i) {

        boulder_draw(&s->boulders[i], topx, topy);
    }

    // Draw player
    pl_draw(&s->pl, topx, topy);
}


