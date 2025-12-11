#ifndef COMMON_H_
#define COMMON_H_

#include "core/game/game.h"

#define WINDOW_W 1920
#define WINDOW_H 1080
#define FONTS_DIR "assets/fonts/"
#define FONT_MAIN_PATH "SourceHanSansSC-VF.otf"
#define IMAGES_DIR "assets/images/"

typedef struct appstate_t appstate_t;

struct appstate_t {
	bool running;
	game_t game;
};

#endif /* COMMON_H_ */
