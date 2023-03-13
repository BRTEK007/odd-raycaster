#ifndef GLOBAL_H
#define GLOBAL_H

//[480,270,3] [320,180,4], [640, 310, 2], [320, 200, 4, (original WF3D)], [400, 225, 4]

#define GAME_SCREEN_OFFSET_X 2
#define GAME_SCREEN_OFFSET_Y 2
#define BAR_SCREEN_OFFSET_X 2
#define BAR_SCREEN_OFFSET_Y 206
#define BAR_SCREEN_WIDTH 400
#define BAR_SCREEN_HEIGHT 32
#define FRAME_WIDTH 404
#define FRAME_HEIGHT 240
#define HEARTS_SLOT_X 2
#define HEARTS_SLOT_Y 206
#define HEAD_SLOT_X 186
#define HEAD_SLOT_Y 206
#define WEAPON_SLOT_X 338
#define WEAPON_SLOT_Y 206
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 200

#define TEXTURE_SIZE 32
#define THREADED_FLOOR_RENDERING
#define THREAD_COUNT 4
#define PLAYER_LIGHT 7.0f

typedef enum state_t
{
    GAME_STATE,
    MENU_STATE
} state_t;

#endif