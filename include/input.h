#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include "SDL2/SDL.h"

#define KEY_COUNT 353

typedef struct key_data_t
{
    bool isPressed, justPressed;
} key_data_t;

typedef struct keys_t
{
    bool anyKeyPressed;
    key_data_t keys[KEY_COUNT];
} keys_t;

key_data_t get_key(keys_t *keyboard, size_t keyCode);

size_t get_key_id(size_t keyCode);

void update_keys(keys_t *keys);

void read_keyboard(SDL_Event *, keys_t *);

void init_keys(keys_t *keys);

#endif
