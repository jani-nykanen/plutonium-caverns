// Player object
// (c) 2019 Jani Nykänen

#ifndef __PLAYER__
#define __PLAYER__

#include "../../core/types.h"
#include "../../core/sprite.h"


// Player type
typedef struct
{
    Byte2 pos;
    Byte2 target;
    int8 moveTimer;
    boolean moving;
    Sprite spr;
    uint8 direction;

    // Rendering flags
    boolean redraw;
    boolean flip;
    // Other flags
    boolean itemsChanged;

    // Item info
    uint8 pickaxe;
    uint8 shovel;
    uint8 bombs;
    uint8 keys;
    uint8 gems;
    uint8 maxGems;

} Player;

// Initialize players
void init_players();

// Create player
Player create_player(uint8 x, uint8 y);

// Update player
void pl_update(Player* pl, void* s, int steps);

// Draw player
void pl_draw(Player* pl, void* s, int dx, int dy);

#endif // __PLAYER__
