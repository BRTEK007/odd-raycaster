#include "input.h"

void KeysArray_init(KeysArray *keys)
{
    keys->isAnyKeyPressed = false;
    for (int i = 0; i < KEY_COUNT; i++)
    {
        keys->keys[i].isPressed = false;
        keys->keys[i].isJustPressed = false;
    }
}

void KeysArray_update(KeysArray *keys)
{
    keys->isAnyKeyPressed = false;
    for (int i = 0; i < KEY_COUNT; i++)
    {
        keys->keys[i].isJustPressed = false;
    }
}

void read_keyboard(SDL_Event *event, KeysArray *keyboard)
{
    if (event->type == SDL_KEYDOWN)
    {
        keyboard->isAnyKeyPressed = true;

        size_t key_id = getKeyId(event->key.keysym.sym);
        if (keyboard->keys[key_id].isPressed == false)
            keyboard->keys[key_id].isJustPressed = true;
        keyboard->keys[key_id].isPressed = true;
    }
    else if (event->type == SDL_KEYUP)
    {

        size_t key_id = getKeyId(event->key.keysym.sym);
        keyboard->keys[key_id].isPressed = false;
    }
}

size_t getKeyId(size_t keyCode)
{
    if (keyCode > 127)
    {
        keyCode -= 1 << 30;
        keyCode += 128;
    }
    return keyCode;
}

KeyData getKeyData(KeysArray *keyboard, size_t keyCode)
{
    return keyboard->keys[getKeyId(keyCode)];
}
