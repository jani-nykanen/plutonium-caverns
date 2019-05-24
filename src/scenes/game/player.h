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
    Sprite spr;
    uint8 direction;

} Player;


#endif // __PLAYER__
