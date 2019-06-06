// Simple audio player
// (c) 2019 Jani Nykänen

#ifndef __AUDIO__
#define __AUDIO__

#include "types.h"

// Sounds
enum {

    S_BEEP1 = 0,
    S_BEEP2 = 1,
    S_BEEP3 = 2,
    S_BEEP4 = 3,
    S_BEEP5 = 4,
    S_BREAK = 5,
    S_MOVE = 6,
    S_ACTIVATE = 7,
    S_DISAPPEAR = 8,
    S_ITEM = 9,
    S_VICTORY = 10,
    S_EXPLOSION = 11,
};

// Initialize
void init_audio();

// Play sound
void audio_play(uint8 sound);

// Update audio
void audio_update(int16 steps);

// Toggle audio
void audio_toggle();

// Is audio enabled
boolean audio_enabled();

#endif // __AUDIO__
