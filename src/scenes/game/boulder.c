// A movable boulder, also used for
// bombs.
// (c) 2019 Jani Nykänen

#include "boulder.h"

#include "../../core/graphics.h"
#include "../../core/assets.h"

#include <stdio.h>

// Bitmaps
static Bitmap* bmpTileset;


// Initialize boulders
void init_boulders() {

    // Get bitmaps
    bmpTileset = (Bitmap*)get_asset("tileset");
}


// Create a boulder
Boulder create_boulder(uint8 x, uint8 y, bool makeBomb) {

    Boulder b;
    b.x = x;
    b.y = y;
    b.isBomb = makeBomb;

    // Set defaults
    b.moving = false;
    b.moveTimer = 0;
    b.redraw = true;    
    b.exist = true;

    return b;
}


// Update boulder
// TODO: (void*) => Player*
void boulder_update(Boulder* b, void* pl, int steps) {

    // ...
}


// Draw
void boulder_draw(Boulder* b, int dx, int dy) {

    if(!b->redraw) return;

    draw_bitmap_region(bmpTileset, 112, 0, 16, 16,
        dx + b->x*16, dy + b->y*16, false);

    b->redraw = false;
}
