#ifndef COMMON_H_
#define COMMON_H_

#include <SDL3/SDL.h>
#include "core/game/game.h"
#include "core/position.h"

#define WINDOW_W 1920
#define WINDOW_H 1080
#define FONTS_DIR "assets/fonts/"
#define FONT_MAIN_PATH "SourceHanSansSC-VF.otf"
#define IMAGES_DIR "assets/images/"

typedef struct appstate_t appstate_t;

struct appstate_t {
	bool running;
	enum {
		SCENE_MAINMENU,
		SCENE_GAME
	} scene;
	bool chaging_scene;
	game_t game;
	pos_t moving_piece;
	SDL_Event *event;
	char command_buffer[10];
	bool ready_to_promote;
	bool white_turn;
};

#endif /* COMMON_H_ */
