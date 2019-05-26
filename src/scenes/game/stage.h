// Game stage
// (c) 2019 Jani Nykänen

#ifndef __STAGE__
#define __STAGE__

#include "../../core/bitmap.h"
#include "../../core/tilemap.h"

#include <stdbool.h>

#include "boulder.h"
#include "player.h"

// Stage type
typedef struct {

    // Bitmaps
    Bitmap* bmpFrame;
    Bitmap* bmpTileset;
    Bitmap* bmpItems;

    // Map
    Tilemap* tmap;
    // Active map data
    uint8* data;
    uint8* solid;
    uint8 width, height;

    // Lava timers
    uint16 lavaTimer;
    uint16 lavaGlowTimer;

    // Rendering flags
    boolean frameDrawn;
    boolean staticDrawn;

    // Objects
    Boulder* boulders;
    Player pl;
    uint8 bcount;

} Stage;

// Create a stage object
Stage* create_stage();

// Initialize a stage
int stage_init(Stage* s, const char* mapPath);

// Update stage
void stage_update(Stage* s, int steps);

// Draw stage
void stage_draw(Stage* s);

// Draw static tiles
void stage_draw_static(Stage* s, 
    uint8 startx, uint8 starty, uint8 ex, uint8 ey,
    int dx, int dy);

// Update solid data
void stage_update_solid(Stage* s, uint8 x, uint8 y, uint8 value);

// Get solid tile data
uint8 stage_get_solid_data(Stage* s, uint8 x, uint8 y);

#endif // __STAGE__
