// Player object
// (c) 2019 Jani Nykänen

#include "player.h"

#include "../../core/bitmap.h"
#include "../../core/assets.h"
#include "../../core/graphics.h"
#include "../../core/sprite.h"
#include "../../core/input.h"
#include "../../core/mathext.h"

#include "stage.h"

// Constants
static const int8 MOVE_TIME = 32;

// Bitmaps
static Bitmap* bmpPlayer;


// Check if free tile
static bool pl_check_free_tile(Player* pl, Stage* s, uint8 tx, uint8 ty) {

    uint8 t = stage_get_solid_data(s, tx, ty);
    return t == 0 || t == 2;
}


// Get down state
static int16 get_down_state(int16 arrow) {
    
    int16 state = input_get_arrow_key(arrow);
    if(state == StateUp || state == StateReleased)
        return -1;
    else
        return state == StatePressed ? 1 : 0;
    
}


// Control player
static void pl_control(Player* pl, Stage* s) {

    uint8 tx = pl->pos.x;
    uint8 ty = pl->pos.y;
    uint8 dir;
    int16 state;
    boolean flip = false;

    // Check arrow keys
    if( (state = get_down_state(ArrowLeft)) >= 0) {

        -- tx;
        dir = 3;
        flip = true;
    }
    else if( (state = get_down_state(ArrowRight)) >= 0) {

        ++ tx;
        dir = 2;
    }
    else if( (state = get_down_state(ArrowUp)) >= 0) {

        -- ty;
        dir = 1;
    }
    else if( (state = get_down_state(ArrowDown)) >= 0) {

        ++ ty;
        dir = 0;
    }

    // If movement
    if(tx != pl->pos.x || ty != pl->pos.y) {

        pl->direction = dir;
        pl->flip = flip;
        pl->redraw = true;

        // Check if free
        if( (tx > 0 && ty > 0 && tx < s->width-1 && ty < s->height-1) &&
            pl_check_free_tile(pl, s, tx, ty) ) {

            // Start moving
            pl->moveTimer = MOVE_TIME;
            pl->moving = true;
            pl->redraw = true;

            pl->target = byte2(tx, ty);
        }
        // Otherwise activate possibly solid tile
        else if(state == 1) {

            if(stage_activate_tile(pl, tx, ty, s)) {

                pl->acting = true;
                pl->direction = dir;
                pl->flip = flip;
            }
        }
    }
}


// Animate
static void pl_animate(Player* pl, int steps) {

    const int16 ANIM_SPEED = 8;

    // "Acting"
    if(pl->acting) {

        pl->redraw = true;  
        pl->spr.frame = 4;
        pl->spr.row = min_int16(pl->direction, 2);
        pl->acting = false;
        return;
    }

    if(pl->moving) {

        spr_animate(&pl->spr, min_int16(pl->direction, 2), 
            0, 3, ANIM_SPEED, steps);
    }
    else {
        
        if(pl->spr.frame != 0)
            pl->redraw = true;

        pl->spr.frame = 0;
        pl->spr.row = min_int16(pl->direction, 2);
    }
}


// Initialize players
void init_players() {

    // Get bitmaps
    bmpPlayer = (Bitmap*)get_asset("player");
}


// Create player
Player create_player(uint8 x, uint8 y) {

    Player pl;

    pl.pos = byte2(x, y);
    pl.target = byte2(x, y);

    pl.spr = create_sprite(16, 16);

    // Set defaults
    pl.direction = 0;
    pl.moveTimer = false;
    pl.moveTimer = 0;
    pl.redraw = true;
    pl.flip = false;
    pl.acting = false;

    pl.itemsChanged = true;

    return pl;
}


// Update player
void pl_update(Player* pl, void* _s, int steps) {

    Stage* s = (Stage*)_s;

    // Check input
    if(pl->moveTimer <= 0) {

        pl_control(pl, s);
    }

    // Animate
    pl_animate(pl, steps);

    // Update move timer
    if(pl->moveTimer > 0) {

        pl->moveTimer -= 1 * steps;
        pl->redraw = true;

        if(pl->moveTimer <= 0) {

            pl->moveTimer = 0;
            // Move to the target
            pl->pos = pl->target;
            pl->moving = false;

            // Check item collisions
            stage_item_collision(pl, s);
        }
    }
}


// Draw player
void pl_draw(Player* pl, void* _s,  int dx, int dy) {

    int x, y;
    Stage* s = (Stage*)_s;

    if(!pl->redraw) return;

    // Determine render position
    x = pl->target.x*16;
    y = pl->target.y*16;
    if(pl->moving) {

        x += (pl->pos.x-pl->target.x)*(pl->moveTimer/2);
        y += (pl->pos.y-pl->target.y)*(pl->moveTimer/2);

    }

     // Redraw bottom tiles
    stage_draw_static(s, pl->pos.x-1, pl->pos.y-1, 
        pl->pos.x+1, pl->pos.y+1, dx, dy, 0);

    // Draw sprite
    spr_draw(&pl->spr, bmpPlayer, 
        dx + x, dy + y, pl->flip);

    pl->redraw = false;
}
