// Tilemap
// (c) 2019 Jani Nykänen

#ifndef __TILEMAP__
#define __TILEMAP__

#include "types.h"

// Tilemap type
typedef struct {

    // Dimensions
    uint16 width;
    uint16 height;
    // Layer count
    uint8 layerCount;

    // Layers
    uint8** layers;

} Tilemap;

// Load a tilemap
Tilemap* load_tilemap(const char* path);

// Get a tile
uint8 map_get_tile(Tilemap* t, uint8 layer, int16 x, int16 y);

// Destroy a tilemap
void destroy_tilemap(Tilemap* t);

#endif // __TILEMAP__

