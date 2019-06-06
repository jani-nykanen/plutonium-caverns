// Application core
// (c) 2019 Jani Nykäenn

#include "application.h"

#include "types.h"
#include "err.h"
#include "graphics.h"
#include "input.h"
#include "assets.h"
#include "transition.h"
#include "audio.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
static boolean running;


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

// Update
static void app_update(int16 steps) {
    
     // Update active scene
    if(activeScene->update != NULL) {

        activeScene->update(steps);
    }

    // Update transition
    tr_update(steps);
    // Update audio
    audio_update(steps);
}


// Draw
static void app_draw() {

    if(activeScene->draw != NULL) {

        // Draw active scene
        activeScene->draw();
    }

    // Draw transition
    tr_draw();
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
    init_audio();

    // Set defaults params
    frameSkip = 1;

    return 0;
}

// Run
void app_run() {

    boolean updateFrame =false;
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
            app_update(frameSkip +1);
            // Draw
            app_draw();
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


// Add a scene
void app_add_scene(Scene s, bool makeActive) {
    
    if(stepCount == MAX_SCENES)
        err_throw_no_param("Maximum scenes reached.");

    scenes[sceneCount ++] = s;
    if(makeActive)
        activeScene = &scenes[sceneCount-1];
}   


// Change scene
void app_change_scene(const char* name, void* param) {

    int16 i = 0;
    for(; i < sceneCount; ++ i) {

        if(strcmp(scenes[i].name, name) == 0) {

            activeScene = &scenes[i];
            if(activeScene->change != NULL) {

                activeScene->change(param);
            }

            return;
        }
    }
}


// Terminate
void app_terminate() {

    running = false;
}   

