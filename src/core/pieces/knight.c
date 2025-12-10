/**
 * @file knight.c
 * @brief implement `knight.h`
 * @author SOgz12Z3Ce
 * @date 2025-11-12
 * @version 1.0
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "core/pieces/knight.h"
#include "core/pieces/piece.h"
#include "core/boards/board.h"
#include "core/position.h"
#include "core/mallocer.h"
#include "core/pieces/distance.h"

static piece_ptr_t copy(piece_ptr_t p);
static bool is(piece_ptr_t p, const char *const type);
static bool can_control(piece_ptr_t p, board_ptr_t b, pos_t pos);
static side_t get_side(piece_ptr_t p);

/** @brief the knight piece */
struct knight_t {
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

piece_ptr_t knight_create(side_t side)
{
	knight_t *ret = new(knight_t);
	if (!ret) {
		printf("fatal error at knight_create: Not enough memory.\n");
		exit(1);
	}
	*ret = (knight_t) {
		.i = vtable,
		.side = side
	};
	return (piece_ptr_t) {
		.knight = ret
	};
}

static piece_ptr_t copy(piece_ptr_t p)
{
	knight_t *this = p.knight;

	return knight_create(this->side);
}

static bool is(piece_ptr_t p, const char *const type)
{
	return strcmp(type, "knight") == 0;
}

static bool can_control(piece_ptr_t p, board_ptr_t b, pos_t pos)
{
	/* knight_t *this = p.knight; */

	pos_t cur_pos = b.i->find(b, p);

	size_t dist_row = dist(cur_pos.row, pos.row);
	size_t dist_col = dist(cur_pos.col, pos.col);

	return (dist_row == 2 && dist_col == 1)
	       || (dist_row == 1 && dist_col == 2);
}

static side_t get_side(piece_ptr_t p)
{
	knight_t *this = p.knight;

	return this->side;
}
