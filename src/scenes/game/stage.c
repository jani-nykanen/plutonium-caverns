// Game stage
// (c) 2019 Jani Nykänen

#include "stage.h"

#include "../../core/err.h"
#include "../../core/graphics.h"
#include "../../core/assets.h"

#include <stdlib.h>


// Draw frame
static void draw_box_frame(Bitmap* bmp, int dx, int dy, int w, int h) {

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
        (w-2)*TILE_W, (h-2)*TILE_H, 0);

    // Draw shadows
    fill_rect(dx + w*TILE_W, dy+TILE_H, 
        TILE_W, h*TILE_H, 51);
    fill_rect(dx + TILE_W, dy + h*TILE_H, 
        (w-1)*TILE_W, TILE_H, 51);
}


// Draw stage walls
static void stage_draw_walls(Stage* s, int dx, int dy) {

    int x = 0;
    int y = 0;

    int sx = 0;
    int sy = 0;

    for(; y < s->height; ++ y) {

        for(x = 0; x < s->width; ++ x) {

            if(s->data[y*s->width+x] != 1)
                continue;

            draw_bitmap_region_fast(s->bmpTileset,
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

    return s;
}


// Initialize a stage
int stage_init(Stage* s, const char* mapPath) {

    int i;
    int size;

    // Load map
    Tilemap* t = load_tilemap(mapPath);
    if(t == NULL) {

        return 1;
    }

    // Copy data
    size = t->width*(t->height-1);
    s->data = (int*)malloc(size);
    s->solid = (int*)malloc(size);
    if(s->data == NULL || s->solid == NULL) {

        THROW_MALLOC_ERR;
        return 1;
    }
    for(i = 0; i < size; ++ i) {

        s->data[i] = t->layers[0] [i+t->width] -16;
        s->solid[i] = 0;
    }
    s->width = t->width;
    s->height = t->height-1;

    // Set defaults
    s->frameDrawn = false;
    s->staticDrawn = false;
    s->lavaTimer = 0;
    s->lavaGlowTimer = 0;

    return 0;
}


// Update stage
void stage_update(Stage* s, int step) {

    const int LAVA_SPEED = 12;
    const int LAVA_GLOW_SPEED = 6;

    // Update lava timers
    s->lavaTimer += LAVA_SPEED * step;
    s->lavaTimer %= 16 * FIXED_PREC;

    s->lavaGlowTimer += LAVA_GLOW_SPEED * step;
    s->lavaGlowTimer %= 4 * FIXED_PREC;
}


// Draw stage
void stage_draw(Stage* s) {

    const int RIGHT_FRAME_WIDTH = 12;

    int w;
    int topx = 24;
    int topy = 16;

    // Draw frames
    if(!s->frameDrawn) {

        // Clear background
        clear_screen(123);

        // Left
        w = s->width*2 +2;
        draw_box_frame(s->bmpFrame,
            16, 8, w, s->height*2 +2);

        // Right
        draw_box_frame(s->bmpFrame,
            (w+2)*8+16, 8, 
            RIGHT_FRAME_WIDTH, s->height*2 +2);

        s->frameDrawn = true;
    }

    // Draw walls
    if(!s->staticDrawn) {

        stage_draw_walls(s, topx, topy);
        s->staticDrawn = true;
    }

    // Draw lava
    stage_draw_lava(s, topx, topy);
}


