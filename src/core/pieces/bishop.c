/**
 * @file bishop.c
 * @brief implement `bishop.h`
 * @author SOgz12Z3Ce
 * @date 2025-11-12
 * @version 1.0
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "core/pieces/bishop.h"
#include "core/pieces/piece.h"
#include "core/boards/board.h"
#include "core/position.h"
#include "core/mallocer.h"
#include "core/pieces/distance.h"

static piece_ptr_t copy(piece_ptr_t p);
static bool is(piece_ptr_t p, const char *const type);
static bool can_control(piece_ptr_t p, board_ptr_t b, pos_t pos);
static side_t get_side(piece_ptr_t p);

/** @brief the bishop piece */
struct bishop_t {
	piece_interface_t i;
	side_t side; /**< @brief `SIDE_WHITE` or `SIDE_BLACK` */
};

static const piece_interface_t vtable = (piece_interface_t) {
	.copy = copy,
	.free = piece_free,
	.is_moved = piece_is_moved,
	.is = is,
	.can_walk = piece_can_walk,
	.can_control = can_control,
	.can_attack = piece_can_attack,
	.can_move = piece_can_move,
	.all = piece_all,
	.get_side = get_side,
	.on_move = piece_on_move,
};

piece_ptr_t bishop_create(side_t side)
{
	bishop_t *ret = new(bishop_t);
	if (!ret) {
		printf("fatal error at bishop_create: Not enough memory.\n");
		exit(1);
	}
	*ret = (bishop_t) {
		.i = vtable,
		.side = side
	};
	return (piece_ptr_t) {
		.bishop = ret
	};
}

static piece_ptr_t copy(piece_ptr_t p)
{
	bishop_t *this = p.bishop;

	return bishop_create(this->side);
}

static bool is(piece_ptr_t p, const char *const type)
{
	return strcmp(type, "bishop") == 0;
}

static bool can_control(piece_ptr_t p, board_ptr_t b, pos_t pos)
{
	/* bishop_t *this = p.bishop; */

	pos_t cur_pos = b.i->find(b, p);
	if (cur_pos.row == pos.row && cur_pos.col == pos.col)
		return false;
	if (dist(cur_pos.row, pos.row) != dist(cur_pos.col, pos.col))
		return false;

	int next_col = 0;
	int next_row = 0;
	if (cur_pos.row != pos.row)
		next_row = cur_pos.row < pos.row ? 1 : -1;
	if (cur_pos.col != pos.col)
		next_col = cur_pos.col < pos.col ? 1 : -1;

	while (true) {
		cur_pos.col += next_col;
		cur_pos.row += next_row;
		if (cur_pos.col == pos.col && cur_pos.row == pos.row)
			return true;
		if (b.i->at(b, cur_pos).ptr)
			return false;
	}

	return true;
}

static side_t get_side(piece_ptr_t p)
{
	bishop_t *this = p.bishop;

	return this->side;
}
