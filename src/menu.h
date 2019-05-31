// A generic (pause) menu
// (c) 2019 Jani Nykänen

#ifndef __MENU__
#define __MENU__

#include "core/types.h"
#include "core/bitmap.h"

// Button name length
#define BUTTON_NAME_LENGTH 12
#define MAX_BUTTON_COUNT 8


// Menu type
typedef struct {

    int8 cpos;
    uint8 escAction;
    uint8 buttonCount;
    // Button info (no need to add an own
    // struct for these, really)
    void (*cbs [MAX_BUTTON_COUNT])(void);
    char text[MAX_BUTTON_COUNT] [BUTTON_NAME_LENGTH];

    uint8 width;

    boolean active;
    boolean redraw;

    // Bitmaps
    Bitmap* bmpFont;

} Menu;

// Create a menu
Menu create_menu(uint8 escAction);

// Add a button
void menu_add_button(Menu* m, const char* text, void (*cb) (void));

// Update menu
void menu_update(Menu* m, int16 steps);

// Draw menu
void menu_draw(Menu* m, int16 dx, int16 dy);

// Activate
void menu_activate(Menu* m, int16 cpos);

#endif // __MENU__
