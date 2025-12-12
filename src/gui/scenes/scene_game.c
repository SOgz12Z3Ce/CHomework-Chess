#include "gui/scenes/scene_game.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "gui/common.h"
#include "gui/utils.h"
#include "gui/button.h"
#include <ctype.h>
#include <string.h>

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
				false,
				nop,
				nop,
				nop,
				nop,
				nop
			);
			piece_count++;
			}
	}
	
    const char *right_button_images[5] = {
		"Promotation_Rook.svg",
		"Promotation_Queen.svg",
		"Promotation_Bishop.svg",
		"Promotation_Knight.svg",
		"save.svg"
	};

    button_t *right_buttons[5];
	for(int i = 0; i < 5; i++) {
		float bx = RIGHT_PANEL_X + (RIGHT_PANEL_W - GAME_BUTTON_W) / 2;
		float by = GAME_BUTTON_SPACING + i * (GAME_BUTTON_H + GAME_BUTTON_SPACING);
		
        char btn_path[2600];
		strcpy(btn_path, images_full_dir);
		strcat(btn_path, right_button_images[i]);

        SDL_Texture *btn_tex = texture_from_svg(renderer, btn_path, GAME_BUTTON_W, GAME_BUTTON_H);
        
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

	*buttons_index = 0;
	
    dest[(*buttons_index)++] = board_b;
	for (int i = 0; i < piece_count; i++) {
		dest[(*buttons_index)++] = piece_buttons[i];
	}
	for (int i = 0; i < 5; i++) {
		dest[(*buttons_index)++] = right_buttons[i];
	}
}
