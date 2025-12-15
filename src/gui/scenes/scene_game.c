#include "gui/scenes/scene_game.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "gui/common.h"
#include "gui/utils.h"
#include "gui/button.h"
#include <ctype.h>
#include <string.h>
#include <stddef.h>
#include "stdlib.h"
#include <stdio.h>

#define CHESS_BOARD_W (2*WINDOW_W/3-200)
#define CHESS_BOARD_H (8*WINDOW_W/15+56)
#define CHESS_BOARD_X 0
#define CHESS_BOARD_Y 0
#define SQUARE_SIZE (CHESS_BOARD_W / 8.0f)

#define RIGHT_PANEL_X CHESS_BOARD_W
#define RIGHT_PANEL_W (WINDOW_W - CHESS_BOARD_W)
#define GAME_BUTTON_W (RIGHT_PANEL_W * 0.8f)
#define GAME_BUTTON_H 80
#define GAME_BUTTON_SPACING ((WINDOW_H - 5 * GAME_BUTTON_H) / 6.0f)
#define IMAGE_BOARD_PATH "Chessboard.svg"

static char images_full_dir[2600];

static void piece_on_press(button_t *self, appstate_t *as);
static void piece_on_release(button_t *self, appstate_t *as);
static void queen_on_click(button_t *self, appstate_t *as);
static void rook_on_click(button_t *self, appstate_t *as);
static void bishop_on_click(button_t *self, appstate_t *as);
static void knight_on_click(button_t *self, appstate_t *as);

void get_game_buttons(button_t **dest, size_t *buttons_index, SDL_Renderer *renderer, TTF_Font *font, appstate_t *as) {
    char board_path[2600];

 	const char *base_path = SDL_GetBasePath();
    if (base_path == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "获取程序目录失败：%s", SDL_GetError());
        return;
	}

	strcpy(images_full_dir, base_path); 
    strcat(images_full_dir, IMAGES_DIR);     

	strcpy(board_path, images_full_dir);      
    strcat(board_path, IMAGE_BOARD_PATH);
	
	SDL_Texture *board_tex = texture_from_svg(renderer, board_path, (int)CHESS_BOARD_W, (int)CHESS_BOARD_H);
	button_t *board_b = create_button(
		(SDL_FRect){
			.x = CHESS_BOARD_X,
			.y = CHESS_BOARD_Y,
			.w = CHESS_BOARD_W,
			.h = CHESS_BOARD_H
		},
		(SDL_Color){0, 0, 0, 0}, 
		board_tex,
		false,
		nop,
		nop,
		nop,
		nop,
		nop
	);

	button_t *piece_buttons[320];
	int piece_count = 0;
    piece_ptr_t **state = as->game.board.i->get_state(as->game.board);
	for (size_t i = 0; i < 8; i++) {
	    for (size_t j = 0; j < 8; j++) {
	        if (!state[i][j].ptr)
	            continue;
	        piece_ptr_t p = state[i][j];
	        char buffer1[2560];
	        char buffer2[2560];
	        p.i->get_name(p, buffer1);
	        p.i->get_image_path(p, buffer2);

			float px = CHESS_BOARD_X + j * SQUARE_SIZE;
			float py = CHESS_BOARD_Y + i * SQUARE_SIZE;


			char piece_path[2600];
			strcpy(piece_path, images_full_dir);
			strcat(piece_path, buffer2);

			SDL_Texture *piece_tex = texture_from_svg(renderer, piece_path, (int)SQUARE_SIZE, (int)SQUARE_SIZE);
			piece_buttons[piece_count] = create_button(
				(SDL_FRect){
					.x = px,
					.y = py,
					.w = SQUARE_SIZE,
					.h = SQUARE_SIZE
				},
				(SDL_Color){0, 0, 0, 0}, 
				piece_tex,
				true,
				nop,
				piece_on_press,
				piece_on_release,
				nop,
				nop
			);
			piece_count++;
			}
	}

	for (size_t i = 0; i < 8; i++)
		free(state[i]);
	free(state);

	const char *right_button_texts[5] = {
    "升变_车",
    "升变_后",
    "升变_象",
    "升变_马",
    "保存当前进度"
};
SDL_Color text_color = {0, 0, 0, 255}; 

    button_t *right_buttons[5];
	for(int i = 0; i < 5; i++) {
		float bx = RIGHT_PANEL_X + (RIGHT_PANEL_W - GAME_BUTTON_W) / 2;
		float by = GAME_BUTTON_SPACING + i * (GAME_BUTTON_H + GAME_BUTTON_SPACING);
        
		SDL_Texture *btn_tex = texture_from_str(renderer, font, right_button_texts[i], &text_color);
	
		if (i == 0) {
			right_buttons[i] = create_button(
			(SDL_FRect){
				.x = bx,
				.y = by,
				.w = GAME_BUTTON_W,
				.h = GAME_BUTTON_H
			},
			color_white,
			btn_tex,
			false,
			nop,
			nop,
			nop,
			rook_on_click,
			nop
			);
		} else if (i == 1) {
			right_buttons[i] = create_button(
			(SDL_FRect){
				.x = bx,
				.y = by,
				.w = GAME_BUTTON_W,
				.h = GAME_BUTTON_H
			},
			color_white,
			btn_tex,
			false,
			nop,
			nop,
			nop,
			queen_on_click,
			nop
			);
		} else if (i == 2) {
			right_buttons[i] = create_button(
			(SDL_FRect){
				.x = bx,
				.y = by,
				.w = GAME_BUTTON_W,
				.h = GAME_BUTTON_H
			},
			color_white,
			btn_tex,
			false,
			nop,
			nop,
			nop,
			bishop_on_click,
			nop
			);
		} else if (i == 3) {
			right_buttons[i] = create_button(
			(SDL_FRect){
				.x = bx,
				.y = by,
				.w = GAME_BUTTON_W,
				.h = GAME_BUTTON_H
			},
			color_white,
			btn_tex,
			false,
			nop,
			nop,
			nop,
			knight_on_click,
			nop
		);
		} else {
			right_buttons[i] = create_button(
			(SDL_FRect){
				.x = bx,
				.y = by,
				.w = GAME_BUTTON_W,
				.h = GAME_BUTTON_H
			},
			color_white,
			btn_tex,
			false,
			nop,
			nop,
			nop,
			nop,
			nop
		);
		}
	}

	*buttons_index = 0;
	
    dest[(*buttons_index)++] = board_b;
	for (int i = 0; i < piece_count; i++) {
		dest[(*buttons_index)++] = piece_buttons[i];
	}
	for (int i = 0; i < 5; i++) {
		dest[(*buttons_index)++] = right_buttons[i];
	}
}

static void piece_on_press(button_t *self, appstate_t *as)
{
	float x = as->event->button.x;
	float y = as->event->button.y;
	size_t j = (size_t)(x / SQUARE_SIZE);
	size_t i = (size_t)(y / SQUARE_SIZE);
	as->moving_piece = pos_create(i, j);
}

static void piece_on_release(button_t *self, appstate_t *as)
{
	float x = as->event->button.x;
	float y = as->event->button.y;
	size_t j = (size_t)(x / SQUARE_SIZE);
	size_t i = (size_t)(y / SQUARE_SIZE);
	if (i >= 8 || j >= 8)
		return;
	if (as->moving_piece.row == i && as->moving_piece.col == j)
		return;

	board_ptr_t board = as->game.board;
	piece_ptr_t moving_piece_ptr = board.i->at(board, as->moving_piece);

	char buffer[8] = "";
	buffer[0] = as->moving_piece.col + 'a';
	buffer[1] = '8' - as->moving_piece.row;
	buffer[2] = ' ';
	buffer[3] = j + 'a';
	buffer[4] = '8' - i;
	buffer[5] = ' ';
	buffer[6] = '0';

	if ((i == 0 || i == 7) && moving_piece_ptr.i->is(moving_piece_ptr, "pawn")) {
		as->ready_to_promote = true;
		strcpy(as->command_buffer, buffer);
		return;
	}
	
	game_update(&as->game, buffer);
	as->chaging_scene = true;
}

static void queen_on_click(button_t *self, appstate_t *as)
{
	if (!as->ready_to_promote)
		return;
	as->command_buffer[6] = 'q';
	game_update(&as->game, as->command_buffer);
	as->chaging_scene = true;
	as->ready_to_promote = false;
}

static void rook_on_click(button_t *self, appstate_t *as)
{
	if (!as->ready_to_promote)
		return;
	as->command_buffer[6] = 'r';
	game_update(&as->game, as->command_buffer);
	as->chaging_scene = true;
	as->ready_to_promote = false;
}

static void bishop_on_click(button_t *self, appstate_t *as)
{
	if (!as->ready_to_promote)
		return;
	as->command_buffer[6] = 'b';
	game_update(&as->game, as->command_buffer);
	as->chaging_scene = true;
	as->ready_to_promote = false;
}

static void knight_on_click(button_t *self, appstate_t *as)
{
	if (!as->ready_to_promote)
		return;
	as->command_buffer[6] = 'k';
	game_update(&as->game, as->command_buffer);
	as->chaging_scene = true;
	as->ready_to_promote = false;
}
