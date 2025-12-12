/**
 * @file king.c
 * @brief implement `king.h`
 * @author SOgz12Z3Ce
 * @date 2025-10-12
 * @version 1.0
 */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "core/pieces/king.h"
#include "core/pieces/piece.h"
#include "core/boards/board.h"
#include "core/position.h"
#include "core/mallocer.h"
#include "core/state.h"
#include "core/move.h"
#include "core/pieces/distance.h"

#define A_COLUMN 0
#define C_COLUMN 2
#define G_COLUMN 6
#define H_COLUMN 7

static const char name[] = "king";
static const char white_image_path[] = "White_King.svg";
static const char black_image_path[] = "Black_King.svg";

static piece_ptr_t copy(piece_ptr_t p);
static bool is_moved(piece_ptr_t p);
static bool is(piece_ptr_t p, const char *const type);
static bool can_walk(piece_ptr_t p, board_ptr_t b, pos_t pos);
static bool can_control(piece_ptr_t p, board_ptr_t b, pos_t pos);
static void get_name(piece_ptr_t p, char *output);
static side_t get_side(piece_ptr_t p);
static void get_image_path(piece_ptr_t p, char *output);
static void on_move(piece_ptr_t p, board_ptr_t b);

/** @brief the classic king piece. can castle to c and g col.
 *         only use in stdboard
*/
struct king_t {
	piece_interface_t i;
	side_t side; /**< @brief `SIDE_WHITE` or `SIDE_BLACK` */
	bool moved; /**< @brief is this king moved */
	const char *name;
	const char *image_path;
};

static const piece_interface_t vtable = (piece_interface_t) {
	.copy = copy,
	.free = piece_free,
	.is_moved = is_moved,
	.is = is,
	.can_walk = can_walk,
	.can_control = can_control,
	.can_attack = piece_can_attack,
	.can_move = piece_can_move,
	.all = piece_all,
	.get_name = get_name,
	.get_side = get_side,
	.get_image_path = get_image_path,
	.on_move = on_move
};

piece_ptr_t king_create(side_t side)
{
	king_t *ret = new(king_t);
	if (!ret) {
		printf("fatal error at king_create: Not enough memory.\n");
		exit(1);
	}
	*ret = (king_t) {
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
		.king = ret
	};
}

static piece_ptr_t copy(piece_ptr_t p)
{
	king_t *this = p.king;
	king_t *ret = king_create(this->side).king;
	ret->moved = this->moved;

	return (piece_ptr_t) {
		.king = ret
	};
}

static bool is_moved(piece_ptr_t p)
{
	king_t *this = p.king;

	return this->moved;
}

static bool is(piece_ptr_t p, const char *const type)
{
	return strcmp(type, "king") == 0;
}

static bool can_walk(piece_ptr_t p, board_ptr_t b, pos_t pos)
{
	king_t *this = p.king;
	
	size_t row_size = b.i->get_row_size(b);
	size_t col_size = b.i->get_col_size(b);
	pos_t cur_pos = b.i->find(b, p);
	
	if (b.i->at(b, pos).ptr)
		return false;
	/* castle */
	if (cur_pos.row == pos.row && dist(cur_pos.col, pos.col) == 2) {
		/* king moved */
		if (this->moved)
			return false;

		/* is checked */
		if (b.i->get_checking(b) & this->side)
			return false;
		
		/* get rook */
		piece_ptr_t l_rook = b.i->at(b, pos_create(cur_pos.row, A_COLUMN));
		piece_ptr_t r_rook = b.i->at(b, pos_create(cur_pos.row, H_COLUMN));

		/* set target rook */
		piece_ptr_t rook;
		if (cur_pos.col < pos.col)
			rook = r_rook;
		else
			rook = l_rook;

		/* rook DNE */
		if (!rook.ptr)
			return false;

		/* path is blocked or attacked */
		int next = cur_pos.col > pos.col ? -1 : 1;
		for (size_t i = cur_pos.col; i != pos.col; i += next) {
			if (i == cur_pos.col)
				continue;
			pos_t ipos = pos_create(pos.row, i);
			if (b.i->at(b, ipos).ptr)
				return false;
			for (size_t j = 0; j < row_size; j++) {
				for (size_t k = 0; k < col_size; k++) {
					piece_ptr_t tmp = b.i->at(b, pos_create(j, k));
					if (!tmp.ptr)
						continue;
					if (tmp.i->can_control(tmp, b, ipos) && !(tmp.i->get_side(tmp) & this->side))
						return false;
				}
			}
		}

		return true;
	}

	/* normal */
	if (dist(cur_pos.row, pos.row) > 1 || dist(cur_pos.col, pos.col) > 1)
		return false;
	return true;
}

bool can_control(piece_ptr_t p, board_ptr_t b, pos_t pos)
{
	/* king_t *this = p.king; */
	pos_t cur_pos = b.i->find(b, p);

	if (pos.row == cur_pos.row && pos.col == cur_pos.col)
		return false;
	return dist(cur_pos.row, pos.row) <= 1 && dist(cur_pos.col, pos.col) <= 1;
}

static void get_name(piece_ptr_t p, char *output)
{
	king_t *this = p.king;

	strcpy(output, this->name);
}

static side_t get_side(piece_ptr_t p)
{
	king_t *this = p.king;

	return this->side;
}

static void get_image_path(piece_ptr_t p, char *output)
{
	king_t *this = p.king;

	strcpy(output, this->image_path);
}

static void on_move(piece_ptr_t p, board_ptr_t b)
{
	king_t *this = p.king;

	move_t last = b.i->get_last_move(b);

	this->moved = true;
	if (last.src.row == last.dest.row
	    && dist(last.src.col, last.dest.col) == 2) {
		pos_t rook_pos;
		pos_t dest_pos;
		if (last.src.col > last.dest.col) {
			rook_pos = pos_create(last.dest.row, A_COLUMN);
			dest_pos = pos_create(last.dest.row, last.dest.col + 1);
		} else {
			rook_pos = pos_create(last.dest.row, H_COLUMN);
			dest_pos = pos_create(last.dest.row, last.dest.col - 1);
		}
		b.i->move(b, move_create(rook_pos, dest_pos));
	}
}
