// Game scene
// (c) 2019 Jani Nykänen

#ifndef __GAME__
#define __GAME__

#include "../../core/scene.h"

#include "player.h"

// Get game scene
Scene game_get_scene();

// (Re)draw info
void game_redraw_info(Player* pl);

// Clear game assets
void game_clear_assets();

// Load assets
int16 game_load_assets();

#endif // __GAME__
