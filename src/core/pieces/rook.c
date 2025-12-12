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

#include "core/pieces/rook.h"
#include "core/pieces/piece.h"
#include "core/boards/board.h"
#include "core/position.h"
#include "core/mallocer.h"

static const char name[] = "king";
static const char white_image_path[] = "White_Rook.svg";
static const char black_image_path[] = "Black_Rook.svg";

static piece_ptr_t copy(piece_ptr_t p);
static bool is_moved(piece_ptr_t p);
static bool is(piece_ptr_t p, const char *const type);
static bool can_control(piece_ptr_t p, board_ptr_t b, pos_t pos);
static void get_name(piece_ptr_t p, char *output);
static side_t get_side(piece_ptr_t p);
static void get_image_path(piece_ptr_t p, char *output);
static void on_move(piece_ptr_t p, board_ptr_t b);

/** @brief the rook piece */
struct rook_t {
	piece_interface_t i;
	side_t side; /**< @brief `SIDE_WHITE` or `SIDE_BLACK` */
	bool moved; /**< @brief is this rook moved */
	const char *name;
	const char *image_path;
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
	.get_name = get_name,
	.get_side = get_side,
	.get_image_path = get_image_path,
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
		.moved = false,
		.name = name
	};
	if (side == SIDE_WHITE)
		ret->image_path = white_image_path;
	else
		ret->image_path = black_image_path;
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

static void get_name(piece_ptr_t p, char *output)
{
	rook_t *this = p.rook;

	strcpy(output, this->name);
}

static side_t get_side(piece_ptr_t p)
{
	rook_t *this = p.rook;

	return this->side;
}

static void get_image_path(piece_ptr_t p, char *output)
{
	rook_t *this = p.rook;

	strcpy(output, this->image_path);
}

static void on_move(piece_ptr_t p, board_ptr_t b)
{
	rook_t *this = p.rook;

	this->moved = true;
}
