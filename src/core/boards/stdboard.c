/**
 * @file stdboard.c
 * @brief implement `stdboard.h`
 * @author SOgz12Z3Ce
 * @date 2025-10-12
 * @version 1.0
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "core/boards/stdboard.h"
#include "core/boards/board.h"
#include "core/pieces/piece.h"
#include "core/mallocer.h"
#include "core/position.h"
#include "core/move.h"
#include "core/state.h"
#include "core/pieces/king.h"

#define STDBOARD_ROW_SIZE 8
#define STDBOARD_COL_SIZE 8

static board_ptr_t copy(board_ptr_t b);
static void _free(board_ptr_t b);
static void clear(board_ptr_t b);
static piece_ptr_t **get_state(board_ptr_t b);
static move_t get_last_move(board_ptr_t b);
static size_t get_row_size(board_ptr_t b);
static size_t get_col_size(board_ptr_t b);
static pos_t find(board_ptr_t b, piece_ptr_t p);
static void put(board_ptr_t b, piece_ptr_t p, pos_t pos);
static void _remove(board_ptr_t b, pos_t pos);
static piece_ptr_t at(board_ptr_t, pos_t pos);
static piece_ptr_t pickup(board_ptr_t b, pos_t pos);
static void move(board_ptr_t b, move_t m);
static side_t get_checking(board_ptr_t b);
static size_t get_square_num(board_ptr_t b);

/** @brief standard board */
struct stdboard_t {
	board_interface_t i;
	piece_ptr_t **state;
	move_t last;
};

static const board_interface_t vtable = (board_interface_t) {
	.copy = copy,
	.free = _free,
	.clear = clear,
	.get_state = get_state,
	.get_last_move = get_last_move,
	.get_row_size = get_row_size,
	.get_col_size = get_col_size,
	.find = find,
	.put = put,
	.remove = _remove,
	.at = at,
	.pickup = pickup,
	.move = move,
	.get_checking = get_checking,
	.get_square_num = get_square_num
};

board_ptr_t stdboard_create()
{
	stdboard_t *ret = new(stdboard_t);
	if (!ret) {
		printf("fatal error at stdboard_create: Not enough memory.\n");
		exit(1);
	}
	*ret = (stdboard_t) {
		.i = vtable,
		.state = state_create(STDBOARD_ROW_SIZE, STDBOARD_COL_SIZE),
		.last = move_null
	};
	return (board_ptr_t) {
		.stdboard = ret
	};
}

static board_ptr_t copy(board_ptr_t b)
{
	stdboard_t *this = b.stdboard;

	stdboard_t *ret = stdboard_create().stdboard;
	state_free(ret->state, STDBOARD_ROW_SIZE);
	ret->state = state_copy(this->state,
	                        STDBOARD_ROW_SIZE, STDBOARD_COL_SIZE);
	
	return (board_ptr_t) {
		.stdboard = ret
	};
}

static void _free(board_ptr_t b)
{
	stdboard_t *this = b.stdboard;
	
	state_free(this->state, STDBOARD_ROW_SIZE);
	free(this);
}

static void clear(board_ptr_t b)
{
	stdboard_t *this = b.stdboard;

	for (size_t i = 0; i < STDBOARD_ROW_SIZE; i++) {
		for (size_t j = 0; j < STDBOARD_COL_SIZE; j++) {
			if (!this->state[i][j].ptr)
				continue;
			_remove(this, pos_create(i, j));
		}
	}
}

static piece_ptr_t **get_state(board_ptr_t b)
{
	stdboard_t *this = b.stdboard;

	return state_copy(this->state,
	                  STDBOARD_ROW_SIZE, STDBOARD_COL_SIZE);
}

move_t get_last_move(board_ptr_t b)
{
	stdboard_t *this = b.stdboard;

	return this->last;
}

static size_t get_row_size(board_ptr_t b)
{
	/* stdboard_t *this = b.stdboard; */

	return STDBOARD_ROW_SIZE;
}

static size_t get_col_size(board_ptr_t b)
{
	/* stdboard_t *this = b.stdboard; */

	return STDBOARD_COL_SIZE;
}

static pos_t find(board_ptr_t b, piece_ptr_t p)
{
	stdboard_t *this = b.stdboard;

	for (int i = 0; i < STDBOARD_ROW_SIZE; i++) {
		for (int j = 0; j < STDBOARD_COL_SIZE; j++) {
			if (this->state[i][j].ptr == p.ptr)
				return pos_create(i, j);
		}
	}
	printf("fatal error at find: Not found piece.");
	exit(1);

	return pos_create(-1, -1); /* garbage value for complier to check */
}

static void put(board_ptr_t b, piece_ptr_t p, pos_t pos)
{
	stdboard_t *this = b.stdboard;

	this->state[pos.row][pos.col] = p;
}

static void _remove(board_ptr_t b, pos_t pos)
{
	stdboard_t *this = b.stdboard;

	piece_ptr_t p = this->state[pos.row][pos.col];
	p.i->free(p);
	this->state[pos.row][pos.col].ptr = NULL;
}

static piece_ptr_t at(board_ptr_t b, pos_t pos)
{
	stdboard_t *this = b.stdboard;

	return this->state[pos.row][pos.col];
}

static piece_ptr_t pickup(board_ptr_t b, pos_t pos)
{
	stdboard_t *this = b.stdboard;

	piece_ptr_t ret = this->state[pos.row][pos.col];
	this->state[pos.row][pos.col].ptr = NULL;

	return ret;
}

static void move(board_ptr_t b, move_t m)
{
	stdboard_t *this = b.stdboard;

	piece_ptr_t cur_p = pickup(this, m.src);
	piece_ptr_t dest_p = at(this, m.dest);

	this->last = m;
	cur_p.i->on_move(cur_p, b);

	if (dest_p.ptr)
		_remove(this, m.dest);
	put(this, cur_p, m.dest);
}

static side_t get_checking(board_ptr_t b)
{
	stdboard_t *this = b.stdboard;

	/* find white and black kings */
	piece_ptr_t white_king;
	piece_ptr_t black_king;
	pos_t white_pos;
	pos_t black_pos;
	for (size_t i = 0; i < STDBOARD_ROW_SIZE; i++) {
		for (size_t j = 0; j < STDBOARD_COL_SIZE; j++) {
			piece_ptr_t tmp = this->state[i][j];
			if (!tmp.ptr)
				continue;
			if (tmp.i->is(tmp, "king")) {
				if (tmp.i->get_side(tmp) == SIDE_WHITE) {
					white_king = tmp;
					white_pos = pos_create(i, j);
				} else {
					black_king = tmp;
					black_pos = pos_create(i, j);
				}
			}
		}
	}

	/* check if checked */
	bool is_white_checked;
	bool is_black_checked;
	for (size_t i = 0; i < STDBOARD_ROW_SIZE; i++) {
		for (size_t j = 0; j < STDBOARD_COL_SIZE; j++) {
			piece_ptr_t tmp = this->state[i][j];
			if (!tmp.ptr)
				continue;
			if (tmp.i->can_attack(tmp, b, white_pos))
				is_white_checked = true;
			if (tmp.i->can_attack(tmp, b, black_pos))
				is_black_checked = true;
		}
	}

	return (is_white_checked ? SIDE_WHITE : 0)
	       | (is_black_checked ? SIDE_BLACK : 0);
}

static size_t get_square_num(board_ptr_t b)
{
	return STDBOARD_COL_SIZE * STDBOARD_ROW_SIZE;
}
