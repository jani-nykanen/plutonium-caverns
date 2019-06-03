// Bitmap
// (c) 2019 Jani Nykänen

#include "bitmap.h"

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#include "err.h"


// Create a bitmap
Bitmap* create_bitmap(uint16 w, uint16 h, uint8* data) {

    uint16 i = 0;

    // Allocate memory
    Bitmap* bmp = (Bitmap*)malloc(sizeof(Bitmap));
    if(bmp == NULL) {

        printf("Memory allocation error!\n");
        return NULL;
    }
    bmp->data = (uint8*)malloc(sizeof(uint8) * w * h);
    if(bmp->data == NULL) {

        printf("Memory allocation error!\n");
        return NULL;
    }

    // Copy data (if any)
    if(data != NULL) {

        for(i=0; i < w*h; ++ i) {

            bmp->data[i] = data[i];
        }
    }

    // Store size
    bmp->width = w;
    bmp->height = h;

    return bmp;
}


// Load a bitmap
Bitmap* load_bitmap(const char* path) {

    uint16 w, h;
    Bitmap* bmp;

    // Open file
    FILE* f = fopen(path, "rb");
    if(f == NULL) {

        err_throw_param_1("Could not load a file in: ", path);
        return NULL;
    }

    // Read size
    fread(&w, sizeof(uint16), 1, f);
    fread(&h, sizeof(uint16), 1, f);

    // Allocate memory
    bmp = create_bitmap((uint8)w, (uint8)h, NULL);
    if(bmp == NULL) {

        err_throw_no_param("Failed to create a bitmap!");
        return NULL;
    }

    // Read data
    fread(bmp->data, sizeof(uint8), w*h, f);

    // Close file
    fclose(f);

    return bmp;
}


// Destroy a bitmap
void destroy_bitmap(Bitmap* bmp) {

    if(bmp == NULL)
        return ;

    // Free data
    if(bmp->data != NULL) {
        
        free(bmp->data);
    }
    // Free bitmap
    free(bmp);
}
