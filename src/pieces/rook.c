/**
 * @file rook.c
 * @brief implement `rook.h`
 * @author SOgz12Z3Ce
 * @date 2025-10-12
 * @version 1.0
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "rook.h"
#include "piece.h"
#include "board.h"
#include "position.h"
#include "mallocer.h"

static piece_ptr_t copy(piece_ptr_t p);
static bool is_moved(piece_ptr_t p);
static bool is(piece_ptr_t p, const char *const type);
static bool can_control(piece_ptr_t p, board_ptr_t b, pos_t pos);
static side_t get_side(piece_ptr_t p);
static void on_move(piece_ptr_t p, board_ptr_t b);

/** @brief the rook piece */
struct rook_t {
	piece_interface_t i;
	side_t side; /**< @brief `SIDE_WHITE` or `SIDE_BLACK` */
	bool moved; /**< @brief is this rook moved */
};

static const piece_interface_t vtable = (piece_interface_t) {
	.copy = copy,
	.free = piece_free,
	.is_moved = is_moved,
	.is = is,
	.can_walk = piece_can_walk,
	.can_control = can_control,
	.can_attack = piece_can_attack,
	.can_move = piece_can_move,
	.all = piece_all,
	.get_side = get_side,
	.on_move = on_move,
};

piece_ptr_t rook_create(side_t side)
{
	rook_t *ret = new(rook_t);
	if (!ret) {
		printf("fatal error at rook_create: Not enough memory.\n");
		exit(1);
	}
	*ret = (rook_t) {
		.i = vtable,
		.side = side,
		.moved = false
	};
	return (piece_ptr_t) {
		.rook = ret
	};
}

static piece_ptr_t copy(piece_ptr_t p)
{
	rook_t *this = p.rook;
	rook_t *ret = rook_create(this->side).rook;
	ret->moved = this->moved;

	return (piece_ptr_t) {
		.rook = ret
	};
}

static bool is_moved(piece_ptr_t p)
{
	rook_t *this = p.rook;

	return this->moved;
}

static bool is(piece_ptr_t p, const char *const type)
{
	return strcmp(type, "rook") == 0;
}

static bool can_control(piece_ptr_t p, board_ptr_t b, pos_t pos)
{
	/* rook_t *this = p.rook; */

	pos_t cur_pos = b.i->find(b, p);

	if (!(cur_pos.col == pos.col ^ cur_pos.row == pos.row))
		return false;
	int next_col;
	int next_row;
	if (cur_pos.col == pos.col) {
		next_col = 0;
		next_row = cur_pos.row < pos.row ? 1 : -1;
	} else {
		next_row = 0;
		next_col = cur_pos.col < pos.col ? 1 : -1;
	}

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
	rook_t *this = p.rook;

	return this->side;
}

static void on_move(piece_ptr_t p, board_ptr_t b)
{
	rook_t *this = p.rook;

	this->moved = true;
}
