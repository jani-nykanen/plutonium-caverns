// Stage menu scene
// (c) 2019 Jani Nykänen

#ifndef __STAGE_MENU__
#define __STAGE_MENU__

#include "../../core/scene.h"

// Get stage menu scene
Scene smenu_get_scene();

// Initialize assets
void smenu_init_assets();

// Load the save data
boolean smenu_load_file();

// Clear progress
void smenu_clear_progress();

#endif // __STAGE_MENU__
