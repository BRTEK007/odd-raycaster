#include "input.h"

void init_keys(keys_t *keys)
{
    keys->anyKeyPressed = false;
    for (int i = 0; i < KEY_COUNT; i++)
    {
        keys->keys[i].isPressed = false;
        keys->keys[i].justPressed = false;
    }
}

void update_keys(keys_t *keys)
{
    keys->anyKeyPressed = false;
    for (int i = 0; i < KEY_COUNT; i++)
    {
        keys->keys[i].justPressed = false;
    }
}

void read_keyboard(SDL_Event *event, keys_t *keyboard)
{
    if (event->type == SDL_KEYDOWN)
    {
        keyboard->anyKeyPressed = true;

        size_t key_id = get_key_id(event->key.keysym.sym);
        if (keyboard->keys[key_id].isPressed == false)
            keyboard->keys[key_id].justPressed = true;
        keyboard->keys[key_id].isPressed = true;
    }
    else if (event->type == SDL_KEYUP)
    {

        size_t key_id = get_key_id(event->key.keysym.sym);
        keyboard->keys[key_id].isPressed = false;
    }
}

size_t get_key_id(size_t keyCode)
{
    if (keyCode > 127)
    {
        keyCode -= 1 << 30;
        keyCode += 128;
    }
    return keyCode;
}

key_data_t get_key(keys_t *keyboard, size_t keyCode)
{
    return keyboard->keys[get_key_id(keyCode)];
}
