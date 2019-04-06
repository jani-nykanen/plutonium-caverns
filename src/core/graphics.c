// Graphics
// (c) 2019 Jani Nykänen

#include "graphics.h"

#include <dos.h>
#include <conio.h>
#include <graph.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "err.h"

#define PALETTE_INCLUDED
#include "palette.h"

// Framebuffer size (we can asssume
// no other size is wanted, or even
// possible, really)
#define FB_WIDTH 320
#define FB_HEIGHT 200

// Framebuffer
static uint8* frame;

// Framebuffer size
static Vector2 frameDim;
static uint16 frameSize; // Size in bytes

// Viewport size
static Rect viewport;
// Is clipping enabled
static bool clipping;
// Translation 
static Vector2 tr;

// Global alpha (TODO: Make not constant?)
static const uint8 ALPHA = 170;

// VGA position
static const long VGA_POS = 0xA0000000;

// Palette constants
static const long PALETTE_INDEX = 0x03c8;
static const long PALETTE_DATA = 0x03c9;



// Clip a rectangle
static bool clip_rect(short* x, short* y, 
    short* w, short* h) {

    // Left
    if(*x < viewport.x) {

        *w -= viewport.x - (*x);
        *x = viewport.x;
    }
    // Right
    if(*x+*w >= viewport.x+viewport.w) {

        *w -= (*x+*w) - (viewport.x + viewport.w);
    }

    // Top
    if(*y < viewport.y) {

        *h -= viewport.y - (*y);
        *y = viewport.y;
    }
    // Bottom
    if(*y+*h >= viewport.y+viewport.h) {

        *h -= (*y+*h) - (viewport.y + viewport.h);
    }

    return *w > 0 && *h > 0;
}


// Clip (general)
static bool clip(int16* sx, int16* sy, int16* sw, int16* sh, 
    int16* dx, int16* dy, bool flip) {

    int16 ow, oh;

    // Left
    ow = *sw;
    if(*dx < viewport.x) {

        *sw -= viewport.x - (*dx);
        if(!flip)
            *sx += ow-*sw;

        *dx = viewport.x;
    }
    // Right
    if(*dx+*sw >= viewport.x+viewport.w) {

         *sw -= (*dx+*sw) - (viewport.x + viewport.w); 
         if(flip)
            *sx += ow-*sw;
    }

    // Top
    oh = *sh;
    if(*dy < viewport.y) {

        *sh -= viewport.y - (*dy);
        *sy += oh-*sh;
        *dy = viewport.y;
    }
    // Bottom
    if(*dy+*sh >= viewport.y+viewport.h) {

        *sh -= (*dy+*sh) - (viewport.y + viewport.h);
    }

    return *sw > 0 && *sh > 0;
}



// Set palette
static void set_palette() {

    int16 i = 0;

    // Generate simple palette
    outp(PALETTE_INDEX,0);
    for(i = 0; i < 256*3 ;  ++ i) {

        outp(PALETTE_DATA, PALETTE[i]/4);
    }
}


// Initialize graphics
int16 init_graphics() {

    // Create a framebuffer
    frame = (uint8*)malloc(sizeof(uint8)*FB_WIDTH*FB_HEIGHT);
    if(frame == NULL) {

        err_throw_no_param("Memory allocation error!");
        return 1;
    }

    // Set defaults
    frameDim.x = FB_WIDTH;
    frameDim.y = FB_HEIGHT;
    frameSize = frameDim.x*frameDim.y;
    tr.x = 0;
    tr.y = 0;

    // Clear to black
    clear_screen(0);

    // Set video mode to 320x200 256 colors
    _setvideomode(_MRES256COLOR);
    // Set palette
    set_palette();

    // Set default viewport
    viewport.x = 0;
    viewport.y = 0;
    viewport.w = FB_WIDTH;
    viewport.h = FB_HEIGHT;
    
    // Set other default values
    clipping = true;

    return 0;
}


// Destroy graphics
void destroy_graphics() {

    // Reset graphics mode
    _setvideomode( _DEFAULTMODE );

    // Free allocated data
    free(frame);
}


// Vertical sync
void vblank() {

    while(inp(0x3DA) & 8);
    while(!(inp(0x3DA) & 8));
}


// Draw frame to the screen
void draw_frame() {

    memcpy((void*)VGA_POS, (const void*)frame, frameSize);
}


// Clear screen
void clear_screen(uint8 color) {

    memset(frame, color, frameSize);
}


// Clear viewport
void clear_view(uint8 color) {

    fill_rect(viewport.x, viewport.y,
        viewport.w, viewport.h, color);
}


// Set viewport
void set_viewport(short x, short y, 
    short w, short h) {

    viewport.x = x;
    viewport.y = y;
    viewport.w = w;
    viewport.h = h;
}


// Reset viewport
void reset_viewport() {

    viewport.x = 0;
    viewport.y = 0;
    viewport.w = FB_WIDTH;
    viewport.h = FB_HEIGHT;
}


// Toggle clipping
void toggle_clipping(bool state) {

    clipping = state;
}


// Translate
void translate(int16 x, int16 y) {

    tr = vec2(x, y);
}


// "Additive translation"
void move(int16 x, int16 y) {

    tr.x += x;
    tr.y += y;
}


// Draw a line
void draw_line(int16 x1, int16 y1, 
    int16 x2, int16 y2, uint8 color) {

    int16 endx = viewport.x + viewport.w;
    int16 endy = viewport.y + viewport.h;

    int16 dx, dy;
    int16 sx = x1 < x2 ? 1 : -1;
    int16 sy = y1 < y2 ? 1 : -1; 

    int16 err;
    int16 e2;

    // Check if outside the screen
    if((x1 < 0 && x2 < 0) ||
       (y1 < 0 && y2 < 0) ||
       (x1 >= frameDim.x && x2 >= frameDim.x) ||
       (y1 >= frameDim.y && y2 >= frameDim.y)) {

        return;
    }

    // Compute error
    dx = abs(x2-x1);
    dy = abs(y2-y1);
    err = (dx > dy ? dx : -dy) / 2;
     
    while(true) {

        // Put pixel
        if(y1 < endy && y1 >= viewport.y &&
            x1 < endx && x1 >= viewport.x)
            frame[y1 * frameDim.x + x1] = color;
        
        // Goal reached
        if (x1==x2 && y1==y2) 
            break;

        e2 = err;
        if (e2 >-dx) { err -= dy; x1 += sx; }
        if (e2 < dy) { err += dx; y1 += sy; }
    }
}


// Fill a rectangle
void fill_rect(int16 dx, int16 dy, 
    int16 w, int16 h, uint8 col) {

    int16 y;
    uint16 offset;

    dx += tr.x;
    dy += tr.y;

    // Clip
    if(clipping && !clip_rect( &dx, &dy, &w, &h))
        return;

    // Draw
    offset = frameDim.x*dy + dx;
    for(y = dy; y < dy+h; ++ y) {

        memset(frame + offset, col, w);
        offset += frameDim.x;
    }

}



// Draw a rectangle that is not filled
void draw_rect(int16 dx, int16 dy, 
    int16 w, int16 h, uint8 col) {

    int16 ox = dx;
    int16 oy = dy;
    uint16 offset;
    int16 y;

    // Clip
    if(clipping && !clip_rect( &dx, &dy, &w, &h))
        return;

    // Top line
    if(dy == oy) {

        offset = frameDim.x*dy + dx;
        memset(frame + offset, col, w);
    }
    // Bottom line
    if(dy+h >= 0) {

        offset = frameDim.x*(dy+h-1) + dx;
        memset(frame + offset, col, w);
    }

    // Left colum
    if(dx == ox) {
        for(y = dy+1; y < dy+h-1; ++ y) {

            frame[frameDim.x*y + dx] = col;
        }
    }
    // Right colum
    if(dx+w >= 0) {
        for(y = dy+1; y < dy+h-1; ++ y) {

            frame[frameDim.x*y + dx+w-1] = col;
        }
    }
}


// Draw a bitmap fast (= ignoring alpha)
void draw_bitmap_fast(Bitmap* bmp, int16 dx, int16 dy) {

    draw_bitmap_region_fast( bmp, 0, 0, bmp->width, bmp->height,
        dx, dy);
}


// Draw a bitmap region fast (= ignoring alpha)
void draw_bitmap_region_fast(Bitmap* bmp, 
    int16 sx, int16 sy, int16 sw, int16 sh, int16 dx, int16 dy) {

    int16 y;
    uint16 offset;
    uint16 boff;

    if(bmp == NULL) return;

    // Translate
    dx += tr.x;
    dy += tr.y;

    // Clip
    if(clipping && !clip(&sx, &sy, &sw, &sh, &dx, &dy, false))
        return;

    // Copy horizontal lines
    offset = frameDim.x*dy + dx;
    boff = bmp->width*sy + sx;
    for(y = dy; y < dy+sh; ++ y) {

        memcpy(frame + offset, bmp->data + boff, sw);
        offset += frameDim.x;
        boff += bmp->width;
    }
}


// Draw text fast (ignoring alpha)
void draw_text_fast(Bitmap* font, const char* text, 
    int16 dx, int16 dy, int16 xoff, int16 yoff, bool center) {

    uint8 len = strlen((const char*)text);

    int16 x = dx;
    int16 y = dy;
    uint16 cw = font->width / 16;
    uint16 ch = cw;
    int16 i;
    uint8 c;
    int16 sx, sy;

    // Center
    if(center) {

        dx -= (cw+xoff)*len/2;
        x = dx;
    }

    // Draw characters
    for(i = 0; i < len; ++ i) {

        c = text[i];

        // Newline
        if(c == '\n') {

            x = dx;
            y += ch + yoff;
            continue;
        }

        sx = c % 16;
        sy = c / 16;

        // Draw char
        draw_bitmap_region_fast(font, sx*cw, sy*ch, 
            cw, ch, x, y);

        x += cw + xoff;
    }
}


// Draw a bitmap
void draw_bitmap(Bitmap* bmp, int16 x, int16 y,
    bool flip) {

    draw_bitmap_region(bmp, 0, 0, bmp->width, bmp->height, x, y, flip);
}


// Draw a bitmap region
void draw_bitmap_region(Bitmap* bmp, 
    int16 sx, int16 sy, int16 sw, int16 sh, int16 dx, int16 dy,
    bool flip) {

    draw_bitmap_region_skip( bmp, sx, sy, sw, sh, dx, dy, 0, flip);
}


// Draw a bitmap region, but skip some pixels
void draw_bitmap_region_skip(Bitmap* bmp, 
    int16 sx, int16 sy, int16 sw, int16 sh, int16 dx, int16 dy,
    int16 skip, bool flip) {

    int16 x, y;
    uint16 offset;
    uint16 boff;
    uint8 pixel;
    int16 dir = flip ? -1 : 1;

    if(bmp == NULL) return;

    // Translate
    dx += tr.x;
    dy += tr.y;

    // Clip
    if(clipping && !clip(&sx, &sy, &sw, &sh, &dx, &dy, flip))
        return;

    // Draw pixels
    offset = frameDim.x*dy + dx;
    boff = bmp->width*sy + sx + (flip ? (sw-1) : 0);
    for(y = 0; y < sh; ++ y) {

        for(x = 0; x < sw; ++ x) {

            pixel = bmp->data[boff];
            // Check if not alpha pixel
            // (i.e not transparent)
            if(pixel != ALPHA &&
              (skip == 0 || (x % skip != 0 && y % skip != 0) )) {

                frame[offset] = pixel;
            }

            boff += dir;
            ++ offset;
        }
        boff += bmp->width-sw*dir;
        offset += frameDim.x-sw;
    }
}
