// Sound data
// (c) 2019 Jani Nykänen

// INCLUDE ONLY ONCE!

#ifdef SOUND_DATA_INCLUDED

// Beep 1
static const int16 BEEP1[] = {
    125, 20, -1,
};
// Beep 2
static const int16 BEEP2[] = {
    200, 8, -1,
};
// Beep 3
static const int16 BEEP3[] = {
    150, 15, -1,
};
// Beep 4
static const int16 BEEP4[] = {
    100, 15, -1,
};
// Beep 5
static const int16 BEEP5[] = {
    200, 5, -1
};

// Break
static const int16 BREAK[] = {

    100, 5, 0, 4, 150, 10, -1
};
// Move
static const int16 MOVE[] = {

    200, 5, -1
};
// Activate
static const int16 ACTIVATE[] = {

    200, 5, 0, 4, 150, 5, -1
};
// Disappear
static const int16 DISAPPEAR[] = {

    200, 4, 150, 5, 100, 10, -1
};

// Item
static const int16 ITEM[] = {

    200, 4, 250, 5, 300, 10, -1
};

// Victory
static const int16 VICTORY[] = {

    200, 20, 150, 5, 250, 5, 175, 5, 300, 30, -1
};

// Explosion
static const int16 EXPLOSION[] = {
    100, 4, 125, 4, 150, 4, 125, 4, 100, 4, 75, 4, 50, 20, -1
};

#undef SOUND_DATA_INCLUDED
#endif
