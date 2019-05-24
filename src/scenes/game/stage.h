// Game stage
// (c) 2019 Jani Nykänen

#ifndef __STAGE__
#define __STAGE__

#include "../../core/bitmap.h"
#include "../../core/tilemap.h"

#include <stdbool.h>

// Stage type
typedef struct {

    // Bitmaps
    Bitmap* bmpFrame;
    Bitmap* bmpTileset;

    // Map
    Tilemap* tmap;
    // Active map data
    int* data;
    int* solid;
    int width, height;

    // Lava timers
    int lavaTimer;
    int lavaGlowTimer;

    // Rendering flags
    bool frameDrawn;
    bool staticDrawn;

} Stage;

// Create a stage object
Stage* create_stage();

// Initialize a stage
int stage_init(Stage* s, const char* mapPath);

// Update stage
void stage_update(Stage* s, int step);

// Draw stage
void stage_draw(Stage* s);

#endif // __STAGE__
