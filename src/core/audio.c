// Simple audio player
// (c) 2019 Jani Nykänen

#include "audio.h"

#include <i86.h>

#include <stdio.h>
#include <stdlib.h>

// Macro for laziness
#define SOUND(s) case S_##s: audioBuffer = s; break;

// Include data
#define SOUND_DATA_INCLUDED
#include "sounddata.h"

// Audio buffer
static const int16* audioBuffer;
// Audio pointer
static uint16 audioPointer;
// Audio timer
static uint16 audioTimer;
// Audio length
static uint16 audioLength;

// Audio playing
static boolean audioPlaying;
// Audio enabled
static boolean audioEnabled;


// Initialize
void init_audio() {

    audioEnabled = true;
    audioPlaying = false;
    audioBuffer = NULL;

    audioTimer = 0;
    audioLength = 0;
}


// Play sound
void audio_play(uint8 s) {

    if(!audioEnabled) return;

    switch (s)
    {
    SOUND(BEEP1);
    SOUND(BEEP2);
    SOUND(BEEP3);
    SOUND(BEEP4);
    default:
        break;
    }
    audioPlaying = true;
    audioTimer = 0;
    audioPointer = 0;

    // Play the first sound
    nosound();
    sound((uint16)audioBuffer[0]);
    audioLength = (uint16)audioBuffer[1];
}


// Update audio
void audio_update(int16 steps) {

    if(!audioEnabled || !audioPlaying) return;

    // Update audio timer
    audioTimer += 1 * (uint16)steps;
    if(audioTimer >= audioLength) {

        audioTimer -= audioLength;
        ++ audioPointer;

        // Stop the current audio
        nosound();
        // If no more sounds, stop
        if(audioBuffer[audioPointer *2] == -1) {

            audioPlaying = false;
            return;
        }

        // Play the next sound
        sound((uint16)audioBuffer[audioPointer *2]);
        audioLength = (uint16)audioBuffer[audioPointer *2 +1];
    }
}


// Toggle audio
void audio_toggle() {

    audioEnabled = !audioEnabled;
    if(!audioEnabled && audioPlaying) {

        audioPlaying = false;
        nosound();
    }
}


// Is audio enabled
boolean audio_enabled() {

    return audioEnabled;
}
