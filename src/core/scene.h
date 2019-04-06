// Scene
// (c) 2019 Jani Nykänen

#ifndef __SCENE_H__
#define __SCENE_H__

#include "types.h"

// Scene type
typedef struct {

    // Methods
    int16 (*init)    (void);
    void  (*update)  (int16);
    void  (*draw)    ();
    void  (*dispose) (void);

    // Name
    const char* name;

} Scene;

#endif // __SCENE_H__
