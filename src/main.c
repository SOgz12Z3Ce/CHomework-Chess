#define SDL_MAIN_USE_CALLBACKS 1

#include <stdlib.h>
#include <limits.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "gui/common.h"
#include "gui/utils.h"
#include "gui/button.h"
#include "core/game/game.h"

// #define START_W 500
// #define START_H 100
// #define START_X (WINDOW_W / 2 - START_W / 2)
// #define START_Y (WINDOW_H / 2 - START_H / 2)
// #define EXIT_W 50
// #define EXIT_H 50
// #define EXIT_X (WINDOW_W - EXIT_W)
// #define EXIT_Y 0

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *font = NULL;
static char fonts_full_dir[PATH_MAX] = "";
static char images_full_dir[PATH_MAX] = "";

// static void start_on_press(button_t *this, appstate_t *as);
// static void start_on_release(button_t *this, appstate_t *as);
// static void start_on_click(button_t *this, appstate_t *as);

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
	/* env */
	strcpy(fonts_full_dir, SDL_GetBasePath());
	strcat(fonts_full_dir, FONTS_DIR);

	strcpy(images_full_dir, SDL_GetBasePath());
	strcat(images_full_dir, IMAGES_DIR);

	/* basic */
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("app", WINDOW_W, WINDOW_H, SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(window, NULL);
	SDL_SetRenderLogicalPresentation(renderer, WINDOW_W, WINDOW_H, SDL_LOGICAL_PRESENTATION_LETTERBOX);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	/* font */
	TTF_Init();
	char main_font_full_path[PATH_MAX] = "";
	strcpy(main_font_full_path, fonts_full_dir);
	strcat(main_font_full_path, FONT_MAIN_PATH);
	font = TTF_OpenFont(main_font_full_path, 48);

	/* appstate */
	appstate_t *as = (appstate_t *)malloc(sizeof(appstate_t));
	as->running = true;
	*appstate = as;

	/* buttons */
	// button_t *start_b = create_button(
	// 	(SDL_FRect){
	// 		.x = START_X,
	// 		.y = START_Y,
	// 		.w = START_W,
	// 		.h = START_H
	// 	},
	// 	(SDL_Color){
	// 		.r = 0x00,
	// 		.g = 0x00,
	// 		.b = 0xFF,
	// 		.a = 0x80
	// 	},
	// 	texture_from_str(renderer, font, u8"开始游戏", &color_white),
	// 	false,
	// 	nop,
	// 	start_on_press,
	// 	start_on_release,
	// 	start_on_click,
	// 	nop
	// );

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
	appstate_t *as = (appstate_t *)appstate;

	switch (event->type)
	{
	case SDL_EVENT_QUIT:
		return SDL_APP_SUCCESS;
	default:
		break;
	}

	if (!as->running)
		return SDL_APP_SUCCESS;
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
	appstate_t *as = (appstate_t *)appstate;

	if (as->game.state == GAME_UPDATING)
		game_update(&as->game, as->game.command);

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{

}

// static void start_on_press(button_t *this, appstate_t *as)
// {
// 	this->color = (SDL_Color) {
// 		.r = 0x00,
// 		.g = 0xFF,
// 		.b = 0x00,
// 		.a = SDL_ALPHA_OPAQUE
// 	};
// }

// static void start_on_release(button_t *this, appstate_t *as)
// {
// 	this->color = (SDL_Color) {
// 		.r = 0x00,
// 		.g = 0x00,
// 		.b = 0xFF,
// 		.a = SDL_ALPHA_OPAQUE
// 	};
// }

// static void start_on_click(button_t *this, appstate_t *as)
// {
// 	/* TODO: add game scene */
// }
