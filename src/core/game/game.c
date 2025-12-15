#include "core/game/game.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "core/move.h"
#include "core/position.h"
#include "core/pieces/piece.h"
#include "core/boards/board.h"

void game_update(game_t *game, const char command[])
{
	if (strlen(command) == 0)
		return;

	if (game->state == GAME_DEFAULT) {
		board_ptr_t b = game->board;

		char src_col;
		size_t src_row;
		char dest_col;
		size_t dest_row;
		char disambiguation;
		sscanf(command, "%c%llu %c%llu %c", &src_col, &src_row, &dest_col, &dest_row, &disambiguation);
		
		pos_t src_pos = pos_create(game->board.i->get_row_size(game->board) - src_row, src_col - 'a');
		pos_t dest_pos = pos_create(game->board.i->get_row_size(game->board) - dest_row, dest_col - 'a');
		piece_ptr_t p = b.i->at(b, src_pos);
		if (!p.i->can_move(p, b, dest_pos))
			return;
		if (p.i->get_side(p) == SIDE_WHITE && !game->white_turn)
			return;
		if (p.i->get_side(p) == SIDE_BLACK && game->white_turn)
			return;

		move_t move = (move_t){
			.src = src_pos,
			.dest = dest_pos,
			.disambiguation = disambiguation
		};

		game->board.i->move(game->board, move);
		game->white_turn = !game->white_turn;
		return;
	}
}
