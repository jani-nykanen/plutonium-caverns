// Application core
// (c) 2019 Jani Nykäenn

#ifndef __APP_CORE__
#define __APP_CORE__

#include "scene.h"

#include <stdbool.h>

// Initialize
int16 init_application();

// Add a scene
void app_add_scene(Scene s, bool makeActive);

// Run
void app_run();

#endif // __APP_CORE__
