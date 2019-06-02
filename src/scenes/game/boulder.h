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
    Byte2 target;

    // Flags
    boolean exist;
    boolean redraw;
    boolean oldPlayerMoveState;

    // Movement
    uint8 moveTimer;
    boolean moving;
    int8 bombTimer;
    uint8 animTimer;

    // Type
    uint8 type;

} Boulder;

// Initialize boulders
void init_boulders();

// Create a boulder
Boulder create_boulder(uint8 x, uint8 y, uint8 type);

// Update boulder
void boulder_update(Boulder* b, void* pl, void* s, int steps);

// Draw
void boulder_draw(Boulder* b, int16 dx, int16 dy);

// Check detonation
void boulder_check_detonation(Boulder* b, void* _s, uint8 dx, uint8 dy);

#endif // __BOULDER__
