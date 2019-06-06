// A generic (pause) menu
// (c) 2019 Jani Nykänen

#include "menu.h"

#include "core/graphics.h"
#include "core/assets.h"
#include "core/input.h"
#include "core/mathext.h"
#include "core/audio.h"

#include <stdio.h>
#include <string.h>

// Bitmaps
static Bitmap* bmpFont;


// Initialize menus
void init_menus() {

    bmpFont = (Bitmap*)get_asset("font");
}


// Create a menu
Menu create_menu(uint8 escAction) {

    Menu m;
    m.buttonCount = 0;
    m.cpos = 0;
    m.active = false;
    m.width = 0;
    m.redraw = true;
    m.escAction = escAction;

    return m;
}


// Add a button
void menu_add_button(Menu* m, const char* text, void (*cb) (void)) {

    m->cbs[m->buttonCount] = cb;
    snprintf(m->text[m->buttonCount], BUTTON_NAME_LENGTH, " %s", text);

    ++ m->buttonCount;
    if(strlen(text) > m->width)
        m->width = strlen(text);
}


// Update menu
void menu_update(Menu* m, int16 steps) {

    int8 opos = m->cpos;

    if(!m->active) return;

    // Check escape
    if(input_get_button(3) == StatePressed) {

        m->cbs[m->escAction] ();
        return;
    }

    // Check if enter pressed
    if(input_get_button(2) == StatePressed) {

        m->cbs[m->cpos]();

        audio_play(S_BEEP3);
    }

    // Check cursor movement
    if(input_get_arrow_key(ArrowDown) == StatePressed)
        ++ m->cpos;
    else if(input_get_arrow_key(ArrowUp) == StatePressed)
        -- m->cpos;

    m->cpos = neg_mod(m->cpos, m->buttonCount);
    if(m->cpos != opos) {

        m->redraw = true;
        audio_play(S_BEEP2);
    }
}


// Draw menu
void menu_draw(Menu* m, int16 dx, int16 dy) {

    const int16 XOFF = 4;
    const int16 YOFF = 6;

    int16 x, y, w, h;
    int16 i;

    if(!m->active || !m->redraw) return;

    w = (m->width+2)*8;
    h = (m->buttonCount+1)*12;

    x = dx - w/2;
    y = dy - h/2;

    // Draw box
    fill_rect(x-2, y-2, w+4, h+4, 146);
    fill_rect(x-1, y-1, w+2, h+2, 255);
    fill_rect(x, y, w, h, 0);

    // Draw text
    for(i = 0; i < m->buttonCount; ++ i) {

        m->text[i][0] = m->cpos == i ? '\6' : ' ';

        draw_text_fast(bmpFont, m->text[i],
            x+XOFF, y+YOFF+i*12+3, 0, 0, false);
    }

    m->redraw = false;
}


// Activate
void menu_activate(Menu* m, int16 cpos) {

    m->active = true;
    m->redraw = true;
    m->cpos = cpos < 0 ? m->cpos : cpos;
}
