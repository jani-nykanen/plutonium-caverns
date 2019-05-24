// Player object
// (c) 2019 Jani Nykänen

#ifndef __PLAYER__
#define __PLAYER__

#include "../../core/types.h"
#include "../../core/sprite.h"


// Player type
typedef struct
{
    uint8 x, y;
    uint8 moveTimer;
    boolean moving;
    Sprite spr;
    uint8 direction;

    

} Player;

// Initialize players
void init_players();

// Create player
Player create_player(uint8 x, uint8 y);

// Update player
void pl_update(Player* pl, int steps);

// Draw player
void pl_draw(Player* pl, int dx, int dy);

#endif // __PLAYER__
