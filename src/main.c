// Main
// (c) 2019 Jani Nykänen

#include "core/application.h"
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

    // Run application
    app_run();
}
