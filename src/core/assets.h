// Global assets manager
// (c) 2019 Jani Nykänen

#ifndef __ASSETS__
#define __ASSETS__

#include "types.h"

#include <stdbool.h>

// Asset name length
#define ASSET_NAME_LENGTH 16

// Asset type
enum {

    TypeBitmap = 0,
    TypeTilemap = 1,
};

// Asset container
typedef struct {

    void* pointer;
    char name [ASSET_NAME_LENGTH];
    int16 type;
    bool isEmpty;

} Asset;

// Initialize
void init_assets();

// Add a bitmap
bool ass_add_bitmap(const char* path, const char* name);
// Add a tilemap
bool ass_add_tilemap(const char* path, const char* name);

// Get an asset
void* get_asset(const char* name);
// Remove an asset
void ass_remove(const char* name);


// Destroy assets
void destroy_assets();

#endif // __ASSETS__
