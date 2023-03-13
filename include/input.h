#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include "SDL2/SDL.h"

#define KEY_COUNT 353

typedef struct KeyData
{
    bool isPressed, isJustPressed;
} KeyData;

typedef struct KeysArray
{
    bool isAnyKeyPressed;
    KeyData keys[KEY_COUNT];
} KeysArray;

KeyData getKeyData(KeysArray *keyboard, size_t keyCode);

size_t getKeyId(size_t keyCode);

void KeysArray_update(KeysArray *keys);

void read_keyboard(SDL_Event *, KeysArray *);

void KeysArray_init(KeysArray *keys);

#endif
