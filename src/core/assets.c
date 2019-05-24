// Global assets manager
// (c) 2019 Jani Nykänen

#include "assets.h"

#include "bitmap.h"
#include "tilemap.h"
#include "err.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Maximum asset count
#define MAX_ASSETS 32
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

// Asset buffer
static Asset assBuffer [MAX_ASSETS];


// Find the next empty location
static int16 find_next_empty() {

    int16 i = 0;
    for(; i < MAX_ASSETS; ++ i) {

        if(assBuffer[i].isEmpty)
            return i;
    }

    return -1;
}


// Put a generic asset to the buffer
static bool put_asset(void* pointer, const char* name, int16 type) {

    Asset ass;

    // Find next empty index
    int16 index = find_next_empty();
    if(index == -1) {

        err_throw_no_param("No more room in the asset buffer!");
        return false;
    }

    // Put to the asset storage
    ass.pointer = pointer;
    snprintf(ass.name, ASSET_NAME_LENGTH, "%s", name);
    ass.type = type;
    ass.isEmpty = false;

    assBuffer[index] = ass;

    return true;
}



// Initialize
void init_assets() {

    int16 i = 0;
    for(; i < MAX_ASSETS; ++ i) {

        assBuffer[i].isEmpty = true;
    }
}


// Add a bitmap
bool ass_add_bitmap(const char* path, const char* name) {

    Bitmap* bmp;

    // Load bitmap
    bmp = load_bitmap(path);
    if(bmp == NULL)
        return false;

    // Put to the asset storage
    if(!put_asset((void*)bmp, name, TypeBitmap)) {

        destroy_bitmap(bmp);
        return false;
    }

    return true;
}


// Add a tilemap
bool ass_add_tilemap(const char* path, const char* name) {

    Tilemap* t;

    // Load tilemap
    t = load_tilemap(path);
    if(t == NULL)
        return false;

    // Put to the asset storage
    if(!put_asset((void*)t, name, TypeTilemap)) {

        destroy_tilemap(t);
        return false;
    }

    return true;
}


// Get an asset
void* get_asset(const char* name) {

    int16 i = 0;
    for(; i < MAX_ASSETS; ++ i) {

        if(assBuffer[i].isEmpty ||
           strcmp(name, assBuffer[i].name) != 0)
            continue;

        return assBuffer[i].pointer;    
    }
    return NULL;
}


// Remove an asset
void ass_remove(const char* name) {

    int16 i = 0;
    for(; i < MAX_ASSETS; ++ i) {

        if(assBuffer[i].isEmpty ||
           strcmp(name, assBuffer[i].name) != 0)
            continue; 

        // Destroy asset
        switch (assBuffer[i].type)
        {
            case TypeBitmap:
                
                destroy_bitmap(assBuffer[i].pointer);
                break;
        
            case TypeTilemap:

                destroy_tilemap(assBuffer[i].pointer);
                break;

            default:
                break;
        }

        // Set location to empty
        assBuffer[i].pointer = NULL;
        assBuffer[i].isEmpty = true;
    }
}


// Destroy assets
void destroy_assets() {

    int16 i = 0;

    for(; i < MAX_ASSETS; ++ i) {

        if(assBuffer[i].isEmpty)
            continue;

        // Destroy asset
        switch (assBuffer[i].type)
        {
            case TypeBitmap:
                
                destroy_bitmap(assBuffer[i].pointer);
                break;
        
            case TypeTilemap:

                destroy_tilemap(assBuffer[i].pointer);
                break;

            default:
                break;
        }
    }
}
