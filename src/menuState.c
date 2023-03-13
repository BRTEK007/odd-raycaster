#include "menuState.h"
// #include <SDL2/SDL_ttf.h>
#include "draw.h"

void menuState_init(menuState_t *menuState, SDL_Renderer *renderer, TTF_Font *font)
{

    menuState->font = font;
    menuState->quit = false;
    menuState->selectedOption = intInRange_create(0, 0, 2 - 1);
}
void menuState_free(menuState_t *menuState)
{
}

void menuState_event(menuState_t *menuState, SDL_Event event)
{

    if (event.type == SDL_KEYDOWN)
    {
        // printf("key down %d ", event->key.keysym.sym);
        switch (event.key.keysym.sym)
        {
        case SDLK_RETURN:
            menuState->quit = true;
            break;
        case SDLK_UP:
            intInRange_decrement(&(menuState->selectedOption));
            break;
        case SDLK_DOWN:
            intInRange_increment(&(menuState->selectedOption));
            break;
        }
    }
}
void menuState_update(menuState_t *menuState, float delta)
{
}

void menuState_draw(menuState_t *menuState, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 64, 0, 25, 255);
    SDL_RenderClear(renderer);

    scaling_info_t scalingInfo = get_scaling_info(renderer);

    text_texture_t titleText = create_text_texture(renderer, menuState->font, "RAYCASTER", (SDL_Color){191, 255, 225});

    int textHeight = scalingInfo.rendererHeight / 8;
    int textWidth = textHeight * titleText.aspectRatio;

    SDL_Rect rect;
    rect.x = scalingInfo.rendererWidth / 2 - textWidth / 2;
    rect.y = scalingInfo.rendererHeight / 3 - textHeight / 2;
    rect.w = textWidth;
    rect.h = textHeight;

    SDL_RenderCopy(renderer, titleText.texture, NULL, &rect);

    // Don't forget to free your surface and texture
    text_texture_free(&titleText);

#define OPTIONS_COUNT 2
#define TEXT_MARGIN_RATIO 1.5

    const static char *options[OPTIONS_COUNT] = {"PLAY", "EXIT"};
    const static char *optionsSelected[OPTIONS_COUNT] = {">PLAY<", ">EXIT<"};

    for (int i = 0; i < OPTIONS_COUNT; i++)
    {
        const char *text = i == menuState->selectedOption.value ? optionsSelected[i] : options[i];

        text_texture_t textTexture = create_text_texture(renderer, menuState->font, text, (SDL_Color){128, 128, 128});

        textHeight = scalingInfo.rendererHeight / 18;
        textWidth = textHeight * textTexture.aspectRatio;

        rect.x = scalingInfo.rendererWidth / 2 - textWidth / 2;
        rect.y = scalingInfo.rendererHeight / 2 + i * textHeight * TEXT_MARGIN_RATIO;
        rect.w = textWidth;
        rect.h = textHeight;

        SDL_RenderCopy(renderer, textTexture.texture, NULL, &rect);
        text_texture_free(&textTexture);
    }

#undef TEXT_MARGIN_RATIO
#undef OPTIONS_COUNT
}