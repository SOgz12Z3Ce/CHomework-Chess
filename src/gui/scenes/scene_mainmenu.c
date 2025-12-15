#include "gui/scenes/scene_mainmenu.h"
#include <SDL3/SDL.h>
#include "gui/common.h"
#include "gui/utils.h"
#include "gui/button.h"
#include "core/boards/board.h"

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

static void mainmenu_button_on_press(button_t *this, appstate_t *as);
static void mainmenu_button_on_release(button_t *this, appstate_t *as);
static void start_on_click(button_t *this, appstate_t *as);
static void load_on_click(button_t *this, appstate_t *as);
static void exit_on_release(button_t *this, appstate_t *as);
static void exit_on_click(button_t *this, appstate_t *as);

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
		mainmenu_button_on_press,
		mainmenu_button_on_release,
		start_on_click,
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
		mainmenu_button_on_press,
		mainmenu_button_on_release,
		load_on_click,
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
		mainmenu_button_on_press,
		exit_on_release,
		exit_on_click,
		nop
	);

	*buttons_index = 0;
	dest[(*buttons_index)++] = start_b;
	dest[(*buttons_index)++] = load_b;
	dest[(*buttons_index)++] = exit_b;
}

static void mainmenu_button_on_press(button_t *this, appstate_t *as)
{
	this->color = color_green;
}

static void mainmenu_button_on_release(button_t *this, appstate_t *as)
{
	this->color = color_blue;
}

static void start_on_click(button_t *this, appstate_t *as)
{
	as->scene = SCENE_GAME;
	as->chaging_scene = true;

	board_ptr_t board = as->game.board;

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

	as->game.white_turn = true;
}

static void load_on_click(button_t *this, appstate_t *as)
{
	/* TODO: load & save */
	as->scene = SCENE_GAME;
	as->chaging_scene = true;
}

static void exit_on_release(button_t *this, appstate_t *as)
{
	this->color = color_red;
}

static void exit_on_click(button_t *this, appstate_t *as)
{
	as->running = false;
}
