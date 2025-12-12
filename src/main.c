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
#include "core/pieces/piece.h"
#include "core/pieces/pawn.h"
#include "core/pieces/rook.h"
#include "core/pieces/knight.h"
#include "core/pieces/bishop.h"
#include "core/pieces/queen.h"
#include "core/pieces/king.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *font = NULL;
static char fonts_full_dir[PATH_MAX] = "";
static char images_full_dir[PATH_MAX] = "";
static button_t *buttons[BUTTONS_MAX];
static size_t buttons_index = 0;

static void on_change_scene();

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
	as->scene = SCENE_MAINMENU;
	as->chaging_scene = true;
	board_ptr_t board = stdboard_create();
	board.i->put(board, pawn_create(SIDE_BLACK), pos_create(1, 0));
	board.i->put(board, pawn_create(SIDE_BLACK), pos_create(1, 1));
	board.i->put(board, pawn_create(SIDE_BLACK), pos_create(1, 2));
	board.i->put(board, pawn_create(SIDE_BLACK), pos_create(1, 3));
	board.i->put(board, pawn_create(SIDE_BLACK), pos_create(1, 4));
	board.i->put(board, pawn_create(SIDE_BLACK), pos_create(1, 5));
	board.i->put(board, pawn_create(SIDE_BLACK), pos_create(1, 6));
	board.i->put(board, pawn_create(SIDE_BLACK), pos_create(1, 7));
	board.i->put(board, pawn_create(SIDE_WHITE), pos_create(6, 0));
	board.i->put(board, pawn_create(SIDE_WHITE), pos_create(6, 1));
	board.i->put(board, pawn_create(SIDE_WHITE), pos_create(6, 2));
	board.i->put(board, pawn_create(SIDE_WHITE), pos_create(6, 3));
	board.i->put(board, pawn_create(SIDE_WHITE), pos_create(6, 4));
	board.i->put(board, pawn_create(SIDE_WHITE), pos_create(6, 5));
	board.i->put(board, pawn_create(SIDE_WHITE), pos_create(6, 6));
	board.i->put(board, pawn_create(SIDE_WHITE), pos_create(6, 7));

	board.i->put(board, rook_create(SIDE_BLACK), pos_create(0, 0));
	board.i->put(board, rook_create(SIDE_BLACK), pos_create(0, 7));
	board.i->put(board, rook_create(SIDE_WHITE), pos_create(7, 0));
	board.i->put(board, rook_create(SIDE_WHITE), pos_create(7, 7));

	board.i->put(board, knight_create(SIDE_BLACK), pos_create(0, 1));
	board.i->put(board, knight_create(SIDE_BLACK), pos_create(0, 6));
	board.i->put(board, knight_create(SIDE_WHITE), pos_create(7, 1));
	board.i->put(board, knight_create(SIDE_WHITE), pos_create(7, 6));

	board.i->put(board, bishop_create(SIDE_BLACK), pos_create(0, 2));
	board.i->put(board, bishop_create(SIDE_BLACK), pos_create(0, 5));
	board.i->put(board, bishop_create(SIDE_WHITE), pos_create(7, 2));
	board.i->put(board, bishop_create(SIDE_WHITE), pos_create(7, 5));

	board.i->put(board, queen_create(SIDE_BLACK), pos_create(0, 3));
	board.i->put(board, queen_create(SIDE_WHITE), pos_create(7, 3));

	board.i->put(board, king_create(SIDE_BLACK), pos_create(0, 4));
	board.i->put(board, king_create(SIDE_WHITE), pos_create(7, 4));

	as->game = (game_t){
		.state = GAME_DEFAULT,
		.board = board
	};
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
	
	/* clear */
	SDL_SetRenderDrawColor(renderer, 0x35, 0x35, 0x35, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	if (as->chaging_scene) {
		on_change_scene();
		switch (as->scene)
		{
		case SCENE_MAINMENU:
			/* TODO: add mainmenu scenes */
			break;
		case SCENE_GAME:
			/* TODO: add game scenes */
			break;
		}
		as->chaging_scene = false;
	}

	if (as->scene == SCENE_GAME && as->game.state == GAME_UPDATING)
		game_update(&as->game, as->game.command);

	for (size_t i = 0; i < buttons_index; i++)
		button_update(renderer, buttons[i], as);

	SDL_RenderPresent(renderer);
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{

}

static void on_change_scene()
{
	for (size_t i = 0; i < buttons_index; i++)
		button_free(buttons[i]);

	buttons_index = 0;
}
