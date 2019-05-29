// Application core
// (c) 2019 Jani Nykäenn

#include "application.h"

#include "types.h"
#include "err.h"
#include "graphics.h"
#include "input.h"
#include "assets.h"
#include "transition.h"

#include <stdlib.h>
#include <stdio.h>

// Maximum amount of scenes
#define MAX_SCENES 16

// Scenes
static Scene scenes[MAX_SCENES];
// Scene count
static uint8 sceneCount;
// Active scene
static Scene* activeScene;

// Frame skip
static int16 frameSkip;
// Step count
static int16 stepCount;

// Is running
static bool running;


// Dispose
static void dispose() {

    int16 i;

    // Dispose scenes
    for(i = 0; i < sceneCount; ++ i) {

        scenes[i].dispose();
    }

    // Destroy components
    destroy_graphics();
    destroy_input();
}


// Initialize
int16 init_application() {

    // Initialize components
    if(init_graphics() == 1) {

        return 1;
    }
    init_input();
    init_assets();
    init_transition();

    // Set defaults params
    frameSkip = 1;

    return 0;
}


// Add a scene
void app_add_scene(Scene s, bool makeActive) {
    
    if(stepCount == MAX_SCENES)
        err_throw_no_param("Maximum scenes reached.");

    scenes[sceneCount ++] = s;
    if(makeActive)
        activeScene = &scenes[sceneCount-1];
}   


// Run
void app_run() {

    bool updateFrame =false;
    int16 i = 0;

    // Initialize scenes
    running = true;
    for(i = 0; i < sceneCount; ++ i) {

        if(scenes[i].init() == 1) {

            running = false;
            break;
        }
    }

    // Start the main loop
    stepCount = 0;
    while(running) {

        // Check frame skipping
        updateFrame = false;
        if(frameSkip == 0 || 
           ++ stepCount > frameSkip) {

            stepCount = 0;
            updateFrame = true;
        }

        // Update & render the active scene
        if(updateFrame 
            && activeScene != NULL) {

            // Update
            if(activeScene->update != NULL) {

                activeScene->update(frameSkip +1);

                // Update transition
                tr_update(frameSkip +1);
            }

            // Draw
            if(activeScene->draw != NULL) {

                activeScene->draw();

                // Draw transition
                tr_draw();
            }
        }

        // Wait for the vertical sync
        vblank();

        // Draw frame
        draw_frame();
    }

    // Dispose application
    dispose();

    // Write error, if any
    if(get_error() != NULL) {

        printf("ERROR: %s\n", get_error());
    }
}


// Terminate
void app_terminate() {

    running = false;
}   
