// A movable boulder, also used for
// bombs.
// (c) 2019 Jani Nykänen

#include "boulder.h"

#include "../../core/graphics.h"
#include "../../core/assets.h"
#include "../../core/mathext.h"

#include <stdio.h>

#include "player.h"
#include "stage.h"

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
    b.pos.x = x;
    b.pos.y = y;
    b.target = b.pos;
    b.isBomb = makeBomb;

    // Set defaults
    b.moving = false;
    b.moveTimer = 0;
    b.redraw = true;    
    b.exist = true;

    return b;
}


// Update boulder
void boulder_update(Boulder* b, void* _pl, void* _s, int steps) {

    Player* pl = (Player*)_pl;
    Stage* s = (Stage*)_s;
    int16 dist;
    Byte2 dir;

    if(!b->exist) return;

    // Only move if the player is moving
    if(!pl->moving && b->moving) {

        b->moving = false;
        b->pos = b->target;
        b->moveTimer = 0;

        // Check if in lava
        if(stage_get_solid_data(s, b->pos.x, b->pos.y) == 3) {

            // Remove lava
            stage_update_tile(s, b->pos.x, b->pos.y, 0);
            stage_update_solid(s, b->pos.x, b->pos.y, 0);

            // Stop existing
            b->exist = false;
            return;
        }

        // Update solid data
        stage_update_solid(s, b->pos.x, b->pos.y, 2);
    }

    // Move
    if(b->moving) {

        b->redraw = true;
        b->moveTimer = pl->moveTimer;
    }
    else {

        // Movement
        if(pl->moving) {

            if(pl->target.x == b->pos.x && 
               pl->target.y == b->pos.y) {

                dir.x = pl->target.x-pl->pos.x;
                dir.y = pl->target.y-pl->pos.y;

                b->target.x = b->pos.x+dir.x;
                b->target.y = b->pos.y+dir.y;

                // Check if free tile
                if(b->target.x < 1 || b->target.x >= s->width-1 ||
                    b->target.y < 1 || b->target.y >= s->height-1 ||
                    stage_get_solid_data(s, b->target.x, b->target.y) == 2) {
                    
                    pl->target = pl->pos;
                    pl->moveTimer = 0;
                    pl->moving = false;
                    b->target = b->pos;
                }
                else {

                    b->moving = true;
                    b->moveTimer = pl->moveTimer;

                    // Update solid data
                    stage_update_solid(s, b->pos.x, b->pos.y, 0);
                }
            }
        }

        // Determine if need for redrawing
        dist = 
            max_int16(
                abs_int16((int16)pl->target.x-(int16)b->pos.x),
                abs_int16((int16)pl->target.y-(int16)b->pos.y)
            );
        if(dist <= 2) {

            b->redraw = true;
        } 

    }
}


// Draw
void boulder_draw(Boulder* b, int dx, int dy) {

    int16 x, y;

    if(!b->redraw) return;

    // Determine render position
    x = b->target.x*16;
    y = b->target.y*16;
    if(b->moving) {

        x += (b->pos.x-b->target.x)*b->moveTimer;
        y += (b->pos.y-b->target.y)*b->moveTimer;

    }
    draw_bitmap_region(bmpTileset, 112, 0, 16, 16,
        dx + x, dy + y, false);

    b->redraw = false;
}
