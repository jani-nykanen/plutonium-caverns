// Game stage
// (c) 2019 Jani Nykänen

#include "stage.h"

#include "../../core/err.h"
#include "../../core/graphics.h"
#include "../../core/assets.h"
#include "../../core/mathext.h"
#include "../../core/audio.h"

#include <stdlib.h>
#include <stdio.h>

#include "game.h"

// Initial animation time
static const int8 INITIAL_ANIM_TIME = 32;
static const int8 ANIM_SKIP = 8;


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


// Draw animation
static void stage_draw_animation(Stage* s, int16 topx, int16 topy) {

    int16 sx, sy, sw, sh, dx, dy;
    int16 skip;
    
    // Draw disappearing tile
    if(s->animMode == 1 || s->animMode == 2) {

        skip = s->animTimer / ANIM_SKIP;
        if(skip > 0) {

            fill_rect(topx + s->animPos.x*16, topy + s->animPos.y*16, 
                16, 16, 0);

            // Draw boulder behind if a frozen boulder
            if(s->animMode == 2) {

                draw_bitmap_region(s->bmpTileset, 112,0, 16, 16,
                    topx + s->animPos.x*16, topy + s->animPos.y*16,
                    false);
            }

            // Draw the disappearing tile
            stage_draw_static(s, s->animPos.x, s->animPos.y, 
                s->animPos.x, s->animPos.y,
                topx, topy , skip);
        }
    }
    // Explosion
    else if(s->animMode == 5) {

        // Draw background
        dx = (int16)(s->animPos.x)-1;
        dy = (int16)(s->animPos.y)-1;
        sw = dx + 2;
        sh = dy + 2;
        if(dx < 0) dx = 0; if(sw > s->width-1) sw = s->width-1;
        if(dy < 0) dy = 0; if(sh > s->height-1) sh = s->height-1;
        stage_draw_static(s, 
            dx, dy,
            sw, sh,
            topx, topy, 0);

        // Re-draw the player
        s->pl.redraw = true;
        pl_draw(&s->pl, NULL, topx, topy);

        // Draw explosion
        skip = 4 - s->animTimer / ANIM_SKIP;
        switch (skip)
        {
            case 0:
                sx = 0; sy = 0;
                dx = -8; dy = -8;
                sw = 16; sh = 16;
                break;
            case 1:
                sx = 16; sy = 0;
                dx = -16; dy = -16;
                sw = 32; sh = 32;
                break;
            case 2:
                sx = 48; sy = 0;
                dx = -24; dy = -24;
                sw = 48; sh = 48;
                break;
            case 3:
                sx = 96; sy = 0;
                dx = -24; dy = -24;
                sw = 48; sh = 48;
                break;
            
            default:
                break;
        }
        draw_bitmap_region(s->bmpExp,   
            sx, sy, sw, sh,
            topx+s->animPos.x*16+8 + dx,
            topy+s->animPos.y*16+8 + dy, false);
    }

    // Draw item animation
    if(s->animFrame >= 0) {

        skip = 3 - (s->animTimer/ANIM_SKIP);
        if(skip < 0) skip = 0;
        else if(skip > 2) skip = 2;

        draw_bitmap_region(s->bmpAnim, 
            (s->animFrame+skip)*16, s->pl.spr.row*16, 16, 16,
            topx + s->animPos.x*16, topy + s->animPos.y*16,
            s->pl.flip);
    }     
}

// Draw lava
static void stage_draw_lava(Stage* s, int dx, int dy) {

    int16 skip;

    int16 x = 0;
    int16 y = 0;

    int16 t = s->lavaTimer / FIXED_PREC;
    int16 sx = 16+t;
    int16 sy = 16-t;
    int16 p = s->lavaGlowTimer / FIXED_PREC;
    if(p >= 3) p = 1;
    sx += 32 * p;

    for(; y < s->height; ++ y) {

        for(x = 0; x < s->width; ++ x) {

            if(s->data[y*s->width+x] != 4)
                continue;

            // Check if disappearing lava
            if( s->animTimer > 0 && 
                s->animMode == 4 &&
                x == s->animPos.x && y == s->animPos.y) {

                skip = s->animTimer / ANIM_SKIP;

                fill_rect(dx + x*16, dy + y*16, 16, 16, 0);
                if(skip > 0) {
                    draw_bitmap_region_skip(s->bmpTileset,
                        sx, sy, 16, 16, dx + x*16, dy + y*16, skip, false);
                }
            }
            else {
                draw_bitmap_region_fast(s->bmpTileset,
                    sx, sy, 16, 16, dx + x*16, dy + y*16);
            }
        }
    }
}


// Add a boulder
static void stage_add_boulder(Stage* s, uint8 x, uint8 y, uint8 type) {

    uint8 i = 0;
    // Find the first boulder that does
    // not exist
    for(; i < s->bcount; ++ i) {

        if(!s->boulders[i].exist) {

            s->boulders[i] = create_boulder(x, y, type);
            s->solid[y*s->width+x] = 2;
            break;
        }
    }
}


// Parse objects (plus pass data to certain the player
// object)
static void stage_parse_objects(Stage* s) {

    uint8 x, y;
    uint8 t;
    uint8 maxGems = 0;
    for(y = 0; y < s->height; ++ y) {

        for(x = 0; x < s->width; ++ x) {

            t = s->data[y*s->width+x];

            // Boulder
            if(t == 5) {

                stage_add_boulder(s, x, y, 0);
                s->data[y*s->width+x] = 0;
            }
            // Player
            else if(t == 17) {

                s->pl = create_player(x, y);
                s->data[y*s->width+x] = 0;
            }
            // Gem
            else if(t == 22) {

                ++ maxGems;
            }
            // Black hole
            else if(t == 23) {

                stage_add_boulder(s, x, y, 2);
                s->data[y*s->width+x] = 0;
            }
        }
    }

    // Get object counts
    s->pl.pickaxe = s->tmap->layers[0][0] -1;
    s->pl.shovel = s->tmap->layers[0][1] -1;
    s->pl.bombs = s->tmap->layers[0][2] -1;
    s->pl.keys = 0;
    s->pl.gems = 0;
    s->pl.maxGems = maxGems;
}


// Set solid
static void stage_set_solid(Stage* s) {

    uint8 x, y;
    uint8 t;
    int16 p;
    for(y = 0; y < s->height; ++ y) {

        for(x = 0; x < s->width; ++ x) {

            t = 0;
            p = y*s->width+x;
            switch (s->data[p])
            {

            // Solid wall
            case 1:
            // Color blocks
            case 8:
            case 9:
            case 10:
                t = 1;
                break;

            // Lava
            case 4:
                t = 3;
                break;

            // Switches
            case 14:
            case 15:
            case 16:
            case 30:
            case 31:
            case 32:
                t = 4;
                break;

            // Lock
            case 7:
                t = 6;
                break;

            // Frozen wall
            case 2:
                t = 5;
                break;
            // Frozen boulder
            case 3:
                t = 7;
                break;

            // Bomb place
            case 6:
                t = 8;
                break;
            
            default:
                break;
            }
            s->solid[p] = t;
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

    s->initialized = false;

    return s;
}
// Destroy
void destroy_stage(Stage* s) {

    if(s == NULL) return;
    stage_refactor(s);
    free(s);
}


// Destroy a stage object
void stage_refactor(Stage* s) {

    if(s == NULL || !s->initialized) return;

    if(s->data != NULL) free(s->data);
    if(s->solid != NULL) free(s->solid);
    if(s->boulders != NULL) free(s->boulders);
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
    s->tmap = t;

    // Copy data & compute the amount of
    // boulders
    size = t->width*(t->height-1);
    s->data = (uint8*)malloc(sizeof(uint8)*size);
    s->solid = (uint8*)malloc(sizeof(uint8)*size);
    if(s->data == NULL || s->solid == NULL) {

        THROW_MALLOC_ERR;
        return 1;
    }
    s->bcount = 1;
    for(i = 0; i < size; ++ i) {

        tileid =  (uint8)max_int16(t->layers[0] [i+t->width], 16) -16;
        s->data[i] = tileid;
        s->solid[i] = 0;

        // If boulder
        if(tileid == 5 || tileid == 3 || tileid == 6 || tileid == 23) {

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
    s->animTimer = 0;
    s->animPos = byte2(0, 0);
    s->animMode = 0;
    s->topLeft = vec2(24, 16);

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

    // Set solid data
    stage_set_solid(s);

    // Parse objects
    stage_parse_objects(s);

    s->initialized = true;

    return 0;
}


// Initialize assets
void stage_init_assets(Stage* s) {

    // Get bitmaps
    s->bmpFrame = (Bitmap*)get_asset("frame");
    s->bmpTileset = (Bitmap*)get_asset("tileset");
    s->bmpItems = (Bitmap*)get_asset("items");
    s->bmpAnim = (Bitmap*)get_asset("anim");
    s->bmpExp = (Bitmap*)get_asset("exp");
    s->bmpShip = (Bitmap*)get_asset("ship");
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

    // Update animation timer
    if(s->animTimer > 0) {

        s->animTimer -= steps;
        if(s->animTimer <= 0) {

            s->animTimer = 0;

            if(s->animMode != 5)
                s->data[s->animPos.y*s->width+s->animPos.x] = 0;
            else {

                // Needed to get rid of certain
                // "artefacts"
                stage_draw_static(s, 
                    s->animPos.x-1, s->animPos.y-1,
                    s->animPos.x+1, s->animPos.y+1,
                    s->topLeft.x, s->topLeft.y, 0);
                s->pl.redraw = true;
            }

            // Make sure the player is not moving
            s->pl.moving = false;
            s->pl.moveTimer = 0;
            s->pl.target = s->pl.pos;

            // Create a boulder, if destroying a frozen
            // boulder
            if(s->animMode == 2) {

                stage_add_boulder(s, s->animPos.x, s->animPos.y, 0);
            }
        }

        return;
    }

    // Update boulders
    for(i = 0; i < s->bcount; ++ i) {

        boulder_update(&s->boulders[i], (void*)&s->pl, (void*)s, steps);
    }

    // Update players
    pl_update(&s->pl, (void*)s, steps);
}


// Draw stage
void stage_draw(Stage* s) {

    const int RIGHT_FRAME_WIDTH = 12;

    int16 w;
    uint8 i;
    int16 topx = s->topLeft.x;
    int16 topy = s->topLeft.y;

    toggle_clipping(false);

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

        stage_draw_static(s, 
            0, 0, s->width-1, s->height-1,
            topx, topy, 0);
        s->staticDrawn = true;
    }

    // Draw lava
    stage_draw_lava(s, topx, topy);

    // Draw animation
    if(s->animTimer > 0)
        stage_draw_animation(s, topx, topy);

    // Draw player
    pl_draw(&s->pl, (void*)s, topx, topy);

    // Draw boulders
    for(i = 0; i < s->bcount; ++ i) {

        boulder_draw(&s->boulders[i], topx, topy);
    } 

    toggle_clipping(true);
}


// Draw static tiles
void stage_draw_static(Stage* s, 
    uint8 startx, uint8 starty, uint8 ex, uint8 ey,
    int16 dx, int16 dy, int16 skip) {

    uint8 x = 0;
    uint8 y = 0;

    int16 sx = 0;
    int16 sy = 0;
    int16 sw = 16;
    int16 sh = 16;

    int8 jx = 0;
    int8 jy = 0;

    uint8 t;
    Bitmap* bmp;

    for(y = starty; y <= ey; ++ y) {

        for(x = startx; x <= ex; ++ x) {

            sw = 16;
            sh = 16;
            jx = 0;
            jy = 0;

            t = s->data[y*s->width+x] ;
            if(t == 0) {

                fill_rect(dx + x*16, dy + y*16, 16, 16, 0);
                continue;
            }

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
                if(t <= 16)
                    sx = 16 + (t-14)*32; 
                else
                    sx = 32 + (t-30)*32;
                

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

            // Ship
            case 24:
                sx = 0;
                sy = 0;
                sw = 32;
                sh = 32;
                jx = -1;
                jy = -1;
                bmp = s->bmpShip;
                break;
            default:
                continue;
            }

            if(skip > 0) {

                draw_bitmap_region_skip(bmp,
                    sx, sy, 16, 16, dx + x*16, dy + y*16, skip,
                    false);
            }
            else {
                
                draw_bitmap_region_fast(bmp,
                    sx, sy, sw, sh, dx + (x+jx)*16, dy + (y+jy)*16);
            }
        }
    }
}


// Update solid data
void stage_update_solid(Stage* s, uint8 x, uint8 y, 
    uint8 value) {
    
    if(x > s->width-1 || y > s->height-1) 
        return;

    s->solid[y*s->width+x] = value;
}


// Get solid tile data
uint8 stage_get_solid_data(Stage* s, uint8 x, uint8 y) {

    if(x > s->width-1 || y > s->height-1) 
        return 1;

    return s->solid[y*s->width+x];
}


// Update tile data
void stage_update_tile(Stage* s, uint8 x, uint8 y, uint8 value) {

    if(x > s->width-1 || y > s->height-1) 
        return;

    // Activate lava death animation
    if(s->data[y*s->width+x] == 4 && value == 0) {

        stage_set_animation(s, 4, x, y);
        s->animFrame = -1;
        return;
    }

    s->data[y*s->width+x] = value;
}


// Get tile data
uint8 stage_get_tile_data(Stage* s, uint8 x, uint8 y) {

    if(x > s->width-1 || y > s->height-1) 
        return 0;

    return s->data[y*s->width+x];
}


// Item collision
void stage_item_collision(Player* pl, Stage* s) {

    uint8 t = s->data[pl->pos.y*s->width+pl->pos.x];
    boolean remove = false;

    switch (t)
    {
    case 18:
        ++ pl->keys;
        remove = true;
        break;

    case 19:
        pl->pickaxe = (uint8)min_int16(5, pl->pickaxe+3);
        remove = true;
        break;

    case 20:
        pl->shovel = (uint8)min_int16(5, pl->shovel+3);
        remove = true;
        break;

    case 21:
        ++ pl->bombs;
        remove = true;
        break;

    case 22:
        ++ pl->gems;
        remove = true;
        break;

    case 24:
        pl->victory = true;
        break;

    default:
        break;
    }

    if(remove) {

        s->data[pl->pos.y*s->width+pl->pos.x] = 0;
        game_redraw_info(&s->pl);

         // Play sound
        audio_play(S_ITEM);
    }
}


// Activation event
boolean stage_activate_tile(Player* pl, uint8 tx, uint8 ty, Stage* s) {

    uint16 i;
    uint16 j = ty * s->width + tx;
    uint8 t = s->solid[j];
    int16 dif;
    
    s->animFrame = -1;

    // Switch
    switch (t)
    {
    case 4:

        // Toggle blocks
        for(i = 0; i < s->width*s->height; ++ i) {

            dif = ((s->data[j]-1)%16)-((s->data[i]-1)%16);
            if(dif == 3) {

                s->solid[i] = 1;
                s->data[i] -= 3;
            }
            else if(dif == 6) {

                s->solid[i] = 0;
                s->data[i] += 3;
            }
        }

        // Make sure boulders are re-drawn
        for(i = 0; i < s->bcount; ++ i) {

            s->boulders[i].redraw = true;
        }

        // Toggle switch
        if(s->data[j] < 17) {

            s->data[j] += 16;
        }
        else {

            s->data[j] -= 16;
        }

        stage_draw_static(s, 1, 1, s->width-2, s->height-2, 
            s->topLeft.x, s->topLeft.y, 0);

        // Sound
        audio_play(S_BEEP2);
    
        break;
        
    // Lock
    case 6: 
        if(pl->keys > 0) {

            stage_update_solid(s, tx, ty, 0);
            stage_set_animation(s, 1, tx, ty);

            -- pl->keys;
            game_redraw_info(pl);

            // Sound
            audio_play(S_ACTIVATE);

            return true;
        }
        break;

    // Frozen
    case 5: 
    case 7:

        if(pl->pickaxe > 0) {

            stage_update_solid(s, tx, ty, 0);
            stage_set_animation(s, t == 5 ? 1 : 2, tx, ty);

            -- pl->pickaxe;
            game_redraw_info(pl);

            s->animFrame = 0;

            // Play sound
            audio_play(S_BREAK);

            return true;
        }
        break;

    // Lava
    case 3:

        if(pl->shovel > 0) {

            stage_update_solid(s, tx, ty, 0);
            stage_set_animation(s, 4, tx, ty);

            -- pl->shovel;
            game_redraw_info(pl);

            s->animFrame = 3;

            // Play sound
            audio_play(S_BREAK);

            return true;
        }
        break;

    // Bomb place
    case 8:
    {   

        if(pl->bombs > 0) {
            // Add a bomb
            stage_add_boulder(s, tx, ty, 1);
            stage_update_solid(s, tx, ty, 0);
            stage_update_tile(s, tx, ty, 0);

            pl->forceRelease = true;
            -- pl->bombs;
            
            game_redraw_info(pl);

            // Sound
            audio_play(S_ACTIVATE);

            return true;
        }
        break;
    }

    default:
        break;
    }

    return false;
}


// Set animation
void stage_set_animation(Stage* s, uint8 mode, uint8 x, uint8 y) {

    s->animMode = mode;
    s->animTimer = INITIAL_ANIM_TIME;
    s->animPos = byte2(x, y);
}


// Detonate a bbomb
void stage_detonate(Stage* s, uint8 dx, uint8 dy) {

    uint8 x, y;
    uint8 t;
    int16 p;
    int16 i;

    stage_update_solid(s, dx, dy, 0);

    for(y = dy-1; y <= dy+1; ++ y) {

        for(x = dx-1; x <= dx+1; ++ x) {

            // Do not destroy the borders
            // or the tile under the bomb
            if(x == 0 || y == 0 || 
               x == s->width-1 || y == s->height-1)
                continue;

            p = y*s->width+x;
            t = s->data[p];
            switch (t)
            {
            // Wall or solid color block
            case 1:
            case 7:
            case 8:
            case 9:
            case 10:
                s->data[p] = 4;
                s->solid[p] = 3;
                break;

            // Ice
            case 2:
            case 3:
                s->data[p] = 0;
                s->solid[p] = 0;
                break;

            default:
                break;
            }
        }
    }

    // Destroy boulders, if nearby
    // Draw boulders
    for(i = 0; i < s->bcount; ++ i) {

        boulder_check_detonation(&s->boulders[i], (void*)s, dx, dy);
    } 

    // Set animation
    stage_set_animation(s, 5, dx, dy);

}


// Reset
void stage_reset(Stage* s) {

    int16 i;
    uint8 tileid;

    // Reset tile & solid data
    for(i = 0; i < s->width*s->height; ++ i) {

        tileid =  (uint8)max_int16(s->tmap->layers[0] [i+s->tmap->width], 16) -16;
        s->data[i] = tileid;
        s->solid[i] = 0;

    }

    // Set defaults
    s->staticDrawn = false;
    s->lavaTimer = 0;
    s->lavaGlowTimer = 0;
    s->animTimer = 0;
    s->animPos = byte2(0, 0);
    s->animMode = 0;
    s->topLeft = vec2(24, 16);

    // Make bouldesr inactive
    for(i = 0; i < s->bcount; ++ i) {

        s->boulders[i].exist = false;
    }

    // Set solid data
    stage_set_solid(s);

    // Parse objects
    stage_parse_objects(s);
}


// Redraw
void stage_redraw(Stage* s) {

    int16 i;

    // Set the render flags for the tiles
    s->staticDrawn = false;
    // Set render flags for the objects
    s->pl.redraw = true;
    for(i = 0; i < s->bcount; ++ i) {

        s->boulders[i].redraw = true;
    } 
}
