#include "menuState.h"
// #include <SDL2/SDL_ttf.h>
#include "draw.h"

void MenuState_init(MenuState *menuState, SDL_Renderer *renderer, TTF_Font *font)
{

    menuState->font = font;
    menuState->quit = false;
    menuState->selectedOption = IntInRange_create(0, 0, 2 - 1);
}
void MenuState_free(MenuState *menuState)
{
}

void MenuState_event(MenuState *menuState, SDL_Event event)
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
            IntInRange_decrement(&(menuState->selectedOption));
            break;
        case SDLK_DOWN:
            IntInRange_increment(&(menuState->selectedOption));
            break;
        }
    }
}
void MenuState_update(MenuState *menuState, float delta)
{
}

void MenuState_draw(MenuState *menuState, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 64, 0, 25, 255);
    SDL_RenderClear(renderer);

    ScalingData scalingInfo = ScalingData_create(renderer);

    TextureXAspectRatio titleText = TextureXAspectRation_create(renderer, menuState->font, "RAYCASTER", (SDL_Color){191, 255, 225});

    int textHeight = scalingInfo.rendererHeight / 8;
    int textWidth = textHeight * titleText.aspectRatio;

    SDL_Rect rect;
    rect.x = scalingInfo.rendererWidth / 2 - textWidth / 2;
    rect.y = scalingInfo.rendererHeight / 3 - textHeight / 2;
    rect.w = textWidth;
    rect.h = textHeight;

    SDL_RenderCopy(renderer, titleText.texture, NULL, &rect);

    // Don't forget to free your surface and texture
    TextureXAspectRation_free(&titleText);

#define OPTIONS_COUNT 2
#define TEXT_MARGIN_RATIO 1.5

    const static char *options[OPTIONS_COUNT] = {"PLAY", "EXIT"};
    const static char *optionsSelected[OPTIONS_COUNT] = {">PLAY<", ">EXIT<"};

    for (int i = 0; i < OPTIONS_COUNT; i++)
    {
        const char *text = i == menuState->selectedOption.value ? optionsSelected[i] : options[i];

        TextureXAspectRatio textTexture = TextureXAspectRation_create(renderer, menuState->font, text, (SDL_Color){128, 128, 128});

        textHeight = scalingInfo.rendererHeight / 18;
        textWidth = textHeight * textTexture.aspectRatio;

        rect.x = scalingInfo.rendererWidth / 2 - textWidth / 2;
        rect.y = scalingInfo.rendererHeight / 2 + i * textHeight * TEXT_MARGIN_RATIO;
        rect.w = textWidth;
        rect.h = textHeight;

        SDL_RenderCopy(renderer, textTexture.texture, NULL, &rect);
        TextureXAspectRation_free(&textTexture);
    }

#undef TEXT_MARGIN_RATIO
#undef OPTIONS_COUNT
}