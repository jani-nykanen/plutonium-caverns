// Graphics
// (c) 2019 Jani Nykänen

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "types.h"
#include "bitmap.h"

// Initialize graphics
int16 init_graphics();

// Destroy graphics
void destroy_graphics();

// Wait for vblank
void vblank();

// Draw frame to the screen
void draw_frame();

// Clear screen
void clear_screen(uint8 color);

// Clear viewport
void clear_view(uint8 color);

// Set viewport
void set_viewport(short x, short y, 
    short w, short h);
// Reset viewport
void reset_viewport();

// Toggle clipping
void toggle_clipping(bool state);

// Translate
void translate(int16 x, int16 y);
// "Additive translation"
void move(int16 x, int16 y);

// Draw a line
void draw_line(int16 x1, int16 y1, 
    int16 x2, int16 y2, uint8 color);

// Fill a rectangle
void fill_rect(int16 x, int16 y, 
    int16 w, int16 h, uint8 col);

// Draw a rectangle that is not filled
void draw_rect(int16 x, int16 y, 
    int16 w, int16 h, uint8 col);

// Draw a bitmap fast (= ignoring alpha)
void draw_bitmap_fast(Bitmap* bmp, int16 x, int16 y);

// Draw a bitmap region fast (= ignoring alpha)
void draw_bitmap_region_fast(Bitmap* bmp, 
    int16 sx, int16 sy, int16 sw, int16 sh, int16 dx, int16 dy);

// Draw text fast (ignoring alpha)
void draw_text_fast(Bitmap* font, const char* text, 
    int16 x, int16 y, int16 xoff, int16 yoff, bool center);

// Draw a bitmap
void draw_bitmap(Bitmap* bmp, int16 x, int16 y,
    bool flip);    

// Draw a bitmap region
void draw_bitmap_region(Bitmap* bmp, 
    int16 sx, int16 sy, int16 sw, int16 sh, int16 dx, int16 dy,
    bool flip);    

// Draw a bitmap region, but skip some pixels
void draw_bitmap_region_skip(Bitmap* bmp, 
    int16 sx, int16 sy, int16 sw, int16 sh, int16 dx, int16 dy,
    int16 skip, bool flip);    

// Set palette darkness
void set_palette_darkness(uint8 d);

#endif // __GRAPHICS_H__
