// An animated sprite
// (c) 2019 Jani Nykänen

#include "sprite.h"

#include <stdio.h>


// Create a sprite
Sprite createSprite(int16 w, int16 h) {

    Sprite s;

    s.width = w;
    s.height = h;
    s.frame = 0;
    s.count = 0;
    s.row = 0;

    return s;
}


// Animate a sprite
void sprAnimate(Sprite* s, int16 row, int16 start, int16 end, 
    int16 speed, int16 steps) {

    if(s == NULL) return;

    // If nothing to animate
    if(start == end) {
    
		s->count = 0;
		s->frame = start;
		s->row = row;
		return;
	}

    // If row changed
	if(s->row != row) {
	
		s->count = 0;
		s->frame = end > start ? start : end;
		s->row = row;
	}

	if(start < end && s->frame < start) {
	
		s->frame = start;
    }
    else if(end < start && s->frame < end) {
    
        s->frame = end;
    }

    // Animate
	s->count += steps;
	if(s->count > speed) {
	
        if(start < end) {
        
            if(++ s->frame > end) {
                s->frame = start;
            }
        }
        else {
        
            if(-- s->frame < end) {
            
                s->frame = start;
            }
        }

		s->count -= speed;
	}
}


// Draw a sprite frame with skipped pixels
void spr_draw_frame_skip(Sprite* s, Bitmap* bmp, 
    int16 frame, int16 row, 
    int16 x, int16 y, int16 skip, bool flip) {

    draw_bitmap_region_skip(bmp, s->width*frame, s->height*row,
        s->width, s->height, x, y, skip, flip);
}


// Draw a sprite with skipped pixels
void spr_draw_skip(Sprite* s,  Bitmap* bmp, 
    int16 x, int16 y, int16 skip, bool flip) {

    spr_draw_frame_skip(s, bmp, s->frame, s->row, x, y, skip, flip);
}


// Draw a sprite frame
void spr_draw_frame(Sprite* s, Bitmap* bmp, 
    int16 frame, int16 row, 
    int16 x, int16 y, bool flip) {

    draw_bitmap_region(bmp, s->width*frame, s->height*row,
        s->width, s->height, x, y, flip);
}


// Draw a sprite
void spr_draw(Sprite* s, Bitmap* bmp, 
    int16 x, int16 y, bool flip) {

    spr_draw_frame(s, bmp, s->frame, s->row, x, y, flip);
}


// Draw a sprite frame
void spr_draw_frame_fast(Sprite* s, Bitmap* bmp, 
    int16 frame, int16 row, 
    int16 x, int16 y) {

    draw_bitmap_region_fast( bmp, s->width*frame, s->height*row,
        s->width, s->height, x, y);
}


// Draw a sprite
void spr_drawFast(Sprite* s, Bitmap* bmp, 
    int16 x, int16 y) {

    spr_draw_frame_fast(s, bmp, s->frame, s->row, x, y);
}
