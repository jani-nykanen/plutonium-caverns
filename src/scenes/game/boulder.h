// A movable boulder, also used for
// bombs.
// (c) 2019 Jani Nykänen

#ifndef __BOULDER__
#define __BOULDER__

#include "../../core/types.h"

#include <stdbool.h>

// Boulder (& bomb) type
typedef struct
{
    Byte2 pos;

    // Flags
    boolean isBomb;
    boolean exist;
    boolean redraw;

    // Movement
    uint8 moveTimer;
    boolean moving;

} Boulder;

// Initialize boulders
void init_boulders();

// Create a boulder
Boulder create_boulder(uint8 x, uint8 y, bool makeBomb);

// Update boulder
// TODO: (void*) => Player*
void boulder_update(Boulder* b, void* pl, int steps);

// Draw
void boulder_draw(Boulder* b, int dx, int dy);

#endif // __BOULDER__
