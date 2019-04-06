// An animated sprite
// (c) 2019 Jani Nykänen

#ifndef __SPRITE__
#define __SPRITE__

#include "types.h"
#include "bitmap.h"
#include "graphics.h"

// Sprite type
typedef struct {

    int16 width;
    int16 height; 
    int16 frame;
    int16 row;
    int16 count;

} Sprite;

// Create a sprite
Sprite createSprite(int16 w, int16 h);

// Animate a sprite
void sprAnimate(Sprite* s, int16 row, int16 start, int16 end, 
    int16 speed, int16 steps);

// Draw a sprite frame with skipped pixels
void spr_draw_frame_skip(Sprite* s, Bitmap* bmp, 
    int16 frame, int16 row, 
    int16 x, int16 y, int16 skip, bool flip);

// Draw a sprite with skipped pixels
void spr_draw_skip(Sprite* s, Bitmap* bmp, 
    int16 x, int16 y, int16 skip, bool flip);

// Draw a sprite frame
void spr_draw_frame(Sprite* s, Bitmap* bmp, 
    int16 frame, int16 row, 
    int16 x, int16 y, bool flip);

// Draw a sprite
void spr_draw(Sprite* s, Bitmap* bmp, 
    int16 x, int16 y, bool flip);

// Draw a sprite frame fast
void spr_draw_frame_fast(Sprite* s, Bitmap* bmp, 
    int16 frame, int16 row, 
    int16 x, int16 y);

// Draw a sprite fast
void spr_drawFast(Sprite* s, Bitmap* bmp, 
    int16 x, int16 y);


#endif // __SPRITE__
