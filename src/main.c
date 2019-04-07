// Main
// (c) 2019 Jani Nykänen

#include "core/application.h"
#include "core/input.h"
#include "core/err.h"

#include "scenes/game/game.h"

#include <stdio.h>


// Main function
void main() {

    // Initialize application
    if(init_application() == 1) {

        printf("ERROR: %s\n", get_error());
        return;
    }

    // Add scenes
    app_add_scene(game_get_scene(), true);

    // Add buttons
    input_add_button(0, 44);
    input_add_button(1, 45);
    input_add_button(2, 19);
    input_add_button(3, 0x1C);
    input_add_button(4, 1);

    // Run application
    app_run();
}
