// Bitmap
// (c) 2019 Jani Nykänen

#ifndef __BITMAP__
#define __BITMAP__

#include "types.h"

// Bitmap type
typedef struct {

    // Dimensions
    uint8 width;
    uint8 height;

    // Pixels
    uint8* data;

} Bitmap;

// Create a bitmap
Bitmap* create_bitmap(uint8 w, uint8 h, uint8* data);

// Load a bitmap
Bitmap* load_bitmap(const char* path);

// Destroy a bitmap
void destroy_bitmap(Bitmap* bmp);

#endif // __BITMAP__
