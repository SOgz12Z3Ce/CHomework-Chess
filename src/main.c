#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "gui/common.h"
#include "gui/utils.h"
#include "core/game/game.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *font = NULL;
static char *const fonts_full_dir = "";
static char *const images_full_dir = "";

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
