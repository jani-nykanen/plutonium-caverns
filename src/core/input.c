// Input handling
// (c) 2019 Jani Nykänen

#include "input.h"

#include "types.h"

#include <dos.h>
#include <conio.h>
#include <bios.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Key buffer size
#define KEY_BUFFER_SIZE 0x60

// Arrow keycodes
static const short ARROW_KEY_CODES[] = {
    77, 72, 75, 80,
};

// Normal keys
static uint8 normalKeys[KEY_BUFFER_SIZE];
// Extended keys
static uint8 extKeys[KEY_BUFFER_SIZE];

// Old key states (normal)
static uint8 oldNormals[KEY_BUFFER_SIZE];
// Old key states (extended)
static uint8 oldExt[KEY_BUFFER_SIZE];
// Read states (normal)
static bool normalRead[KEY_BUFFER_SIZE];
// Read states (extended)
static bool extRead[KEY_BUFFER_SIZE];

// Handlers
static void far interrupt (*oldHandler)(void);


// Keyboard interruption
static void far interrupt handler() {

    static uint8 buffer;
    uint8 rawcode;
    uint8 makeBreak;
    int16 scancode;

    rawcode = inp(0x60); 
    makeBreak = !(rawcode & 0x80); 
    scancode = rawcode & 0x7F;

    if(buffer == 0xE0) { 

        if(scancode < 0x60) {

            oldExt[scancode] = extKeys[scancode];
            extKeys[scancode] = makeBreak;
            
            if(extKeys[scancode] != oldExt[scancode])
                extRead[scancode] = false;
        }
        buffer = 0;

    } 
    else if (buffer >= 0xE1 && buffer <= 0xE2) {

        buffer = 0;

    } 
    else if (rawcode >= 0xE0 && rawcode <= 0xE2) {

        buffer = rawcode; 

    } 
    else if (scancode < 0x60) {

        oldNormals[scancode] = normalKeys[scancode];
        normalKeys[scancode] = makeBreak;

        if(normalKeys[scancode] != oldNormals[scancode])
            normalRead[scancode] = false;
    }

    outp(0x20, 0x20);
}


// Get button value from an array
static char get_value_from_array(uint8* arr, bool* readArr, int16 id) {

    uint8 state = arr[id];
    bool read = readArr[id];
    uint8 ret = 0;

    if(state == Down) {

        ret = read ? Down : Pressed;
    }
    else if(state == Up) {

        ret = read ? Up : Released;
    }
    readArr[id] = true;

    return ret;
}


// Initialize
void init_input() {

    int16 i;

    // Set defaults
    for(i=0; i < KEY_BUFFER_SIZE; ++ i) {

        oldNormals[i] = 255;
        oldExt[i] = 255;
        normalRead[i] = false;
        extRead[i] = false;
    }

    // Hook handlers
    oldHandler = _dos_getvect(0x09);
    _dos_setvect(0x09, handler);
}


// Destroy
void destroy_input() {

    _dos_setvect(0x09, oldHandler);
    oldHandler = NULL;
}


// Get key state
int16 input_get_key(int16 id) {

    if(id > KEY_BUFFER_SIZE)
        return Up;

    // Get normal key
    return get_value_from_array(
        normalKeys, 
        normalRead, id);
}


// Get arrow key state
int16 input_get_arrow_key(int16 id) {

    if(id > 4)
        return Up;

    // Get arrow key
    return get_value_from_array(
        extKeys, 
        extRead, ARROW_KEY_CODES[id]);
}
