#include "gui/scenes/scene_mainmenu.h"
#include <SDL3/SDL.h>
#include "gui/common.h"
#include "gui/utils.h"
#include "gui/button.h"

#define MAINMENU_BUTTON_W 500
#define MAINMENU_BUTTON_H 100
#define START_W MAINMENU_BUTTON_W
#define START_H MAINMENU_BUTTON_H
#define START_X (WINDOW_W / 2 - START_W / 2)
#define START_Y (WINDOW_H / 2 - START_H / 2)
#define LOAD_W MAINMENU_BUTTON_W
#define LOAD_H MAINMENU_BUTTON_H
#define LOAD_X (WINDOW_W / 2 - LOAD_W / 2)
#define LOAD_Y (WINDOW_H / 2 - LOAD_H / 2 + 2 * MAINMENU_BUTTON_H)
#define EXIT_W 50
#define EXIT_H 50
#define EXIT_X (WINDOW_W - EXIT_W)
#define EXIT_Y 0

void get_mainmenu_buttons(button_t **dest, size_t *buttons_index, SDL_Renderer *renderer, TTF_Font *font)
{
	button_t *start_b = create_button(
		(SDL_FRect){
			.x = START_X,
			.y = START_Y,
			.w = START_W,
			.h = START_H
		},
		color_blue,
		texture_from_str(renderer, font, u8"开始游戏", &color_white),
		false,
		nop,
		nop,
		nop,
		nop,
		nop
	);
	button_t *load_b = create_button(
		(SDL_FRect){
			.x = LOAD_X,
			.y = LOAD_Y,
			.w = LOAD_W,
			.h = LOAD_H
		},
		color_blue,
		texture_from_str(renderer, font, u8"继续游戏", &color_white),
		false,
		nop,
		nop,
		nop,
		nop,
		nop
	);
	button_t *exit_b = create_button(
		(SDL_FRect){
			.x = EXIT_X,
			.y = EXIT_Y,
			.w = EXIT_W,
			.h = EXIT_H
		},
		color_red,
		texture_from_str(renderer, font, u8"×", &color_white),
		false,
		nop,
		nop,
		nop,
		nop,
		nop
	);

	*buttons_index = 0;
	dest[(*buttons_index)++] = start_b;
	dest[(*buttons_index)++] = load_b;
	dest[(*buttons_index)++] = exit_b;
}
