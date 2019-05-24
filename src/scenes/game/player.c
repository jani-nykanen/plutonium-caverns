// Player object
// (c) 2019 Jani Nykänen

#include "player.h"

#include "../../core/bitmap.h"
#include "../../core/assets.h"
#include "../../core/graphics.h"
#include "../../core/sprite.h"

// Bitmaps
static Bitmap* bmpPlayer;


// Initialize players
void init_players() {

    // Get bitmaps
    bmpPlayer = (Bitmap*)get_asset("player");
}


// Create player
Player create_player(uint8 x, uint8 y) {

    Player pl;

    pl.x = x;
    pl.y = y;

    pl.spr = create_sprite(16, 16);

    pl.direction = 0;
    pl.moveTimer = false;
    pl.moveTimer = 0;

    return pl;
}


// Update player
void pl_update(Player* pl, int steps) {

    // ...
}


// Draw player
void pl_draw(Player* pl, int dx, int dy) {

    spr_draw(&pl->spr, bmpPlayer, 
        dx + pl->x*16, dy + pl->y*16, false);
}
