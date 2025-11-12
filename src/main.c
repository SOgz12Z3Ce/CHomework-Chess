#include <stdio.h>

#include "board.h"
#include "stdboard.h"
#include "piece.h"
#include "king.h"

int main(void)
{
	board_ptr_t board = stdboard_create();
	board.i->put(board, king_create(SIDE_BLACK), pos_create(0, 2));
	board.i->put(board, king_create(SIDE_WHITE), pos_create(7, 4));
	board.i->put(board, rook_create(SIDE_BLACK), pos_create(0, 4));
	board.i->put(board, rook_create(SIDE_WHITE), pos_create(2, 4));
	board.i->put(board, rook_create(SIDE_WHITE), pos_create(7, 0));
	board.i->put(board, rook_create(SIDE_WHITE), pos_create(7, 7));
	board.i->put(board, rook_create(SIDE_BLACK), pos_create(2, 0));
	piece_ptr_t the_rook = board.i->at(board, pos_create(7, 4));
	size_t size[4];
	pos_t *walkable = the_rook.i->all(the_rook, board, &size[0], the_rook.i->can_walk);
	pos_t *attackable = the_rook.i->all(the_rook, board, &size[1], the_rook.i->can_attack);
	pos_t *controlable = the_rook.i->all(the_rook, board, &size[2], the_rook.i->can_control);
	pos_t *moveable = the_rook.i->all(the_rook, board, &size[3], the_rook.i->can_move);
	printf("walkable\n");
	for (size_t i = 0; i < size[0]; i++) {
		printf("row: %zu, col: %zu\n", walkable[i].row, walkable[i].col);
	}
	printf("attackable\n");
	for (size_t i = 0; i < size[1]; i++) {
		printf("row: %zu, col: %zu\n", attackable[i].row, attackable[i].col);
	}
	printf("controlable\n");
	for (size_t i = 0; i < size[2]; i++) {
		printf("row: %zu, col: %zu\n", controlable[i].row, controlable[i].col);
	}
	printf("moveable\n");
	for (size_t i = 0; i < size[3]; i++) {
		printf("row: %zu, col: %zu\n", moveable[i].row, moveable[i].col);
	}
	board.i->move(board, move_create(pos_create(7, 4), pos_create(7, 2)));
	printf("-----------");
	piece_ptr_t tmp1 = board.i->at(board, pos_create(7, 2));
	piece_ptr_t tmp2 = board.i->at(board, pos_create(7, 3));
	printf("%d", tmp1.i->is(tmp1, "king"));
	printf("%d", tmp2.i->is(tmp2, "rook"));

	return 0;
}
