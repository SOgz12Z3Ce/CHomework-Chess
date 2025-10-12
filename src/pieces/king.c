/**
 * @file king.c
 * @brief implement `king.h`
 * @author SOgz12Z3Ce
 * @date 2025-10-12
 * @version 1.0
 */

#include <stddef.h>
#include <stdbool.h>

#include "king.h"
#include "piece.h"
#include "board.h"
#include "position.h"

#define MAX_REACHABLE_POS_SIZE 10

#define abs(x) ((x) > 0 ? (x) : (-(x)))

static piece_ptr_t copy(piece_ptr_t p);
static void free(piece_ptr_t p);
static bool is_king(piece_ptr_t p);
static bool can_move_to(piece_ptr_t p, board_ptr_t b, pos_t pos);
static pos_t *get_moveable(piece_ptr_t p, board_ptr_t b, size_t *size);
static bool can_reach(piece_ptr_t p, board_ptr_t b, pos_t pos);
static pos_t *get_reachable(piece_ptr_t p, board_ptr_t b, size_t *size);
static side_t get_side(piece_ptr_t p);

// TODO: castle

/** @brief the king piece */
struct king_t {
	piece_interface_t *i;
	bool moved; /**< @brief is this king moved */
	side_t side; /**< @brief `SIDE_WHITE` or `SIDE_BLACK` */
};

piece_ptr_t king_create(side_t side)
{
	king_t *ret = (king_t *)malloc(sizeof(king_t));
	if (!ret) {
		printf("fatal error at king_create: Not enough memory.\n");
		exit(1);
	}
	piece_interface_t *i = (piece_interface_t *)malloc(sizeof(piece_interface_t));
	if (!i) {
		printf("fatal error at king_create: Not enough memory.\n");
		exit(1);
	}
	*i = (piece_interface_t) {
		.copy = copy,
		.free = free,
		.is_king = is_king,
		.can_move_to = can_move_to,
		.get_moveable = get_moveable,
		.can_reach = can_reach,
		.get_reachable = get_reachable,
		.get_side = get_side
	};
	*ret = (king_t) {
		.i = i,
		.moved = false,
		.side = side
	};
	return (piece_ptr_t) {
		.king = ret
	};
}

static piece_ptr_t copy(piece_ptr_t p)
{
	king_t *this = p.king;

	return (piece_ptr_t) {
		.king = king_create(get_side(this))
	};
}

static void free(piece_ptr_t p)
{
	king_t *this = p.king;

	free(this);
}

static bool is_king(piece_ptr_t p)
{
	// king_t *this = p.king;

	return true;
}

static bool can_move_to(piece_ptr_t p, board_ptr_t b, pos_t pos)
{
	king_t *this = p.king;

	piece_ptr_t **state = b.i->get_state(b);
	pos_t cur_pos = b.i->find(b, p);

	bool ret;
	if (!can_reach(p, b, pos)) {
		ret = false;
	}
	else if (is_same_side(p, state[pos.row][pos.col])) {
		ret = false;
	}
	else {
		/** create and check the post-move board */
		board_ptr_t tmp = b.i->copy(b);
		tmp.i->remove(tmp, cur_pos);
		tmp.i->put(tmp, copy(this), pos);

		if (tmp.i->get_checking(tmp) & get_side(this))
			ret = false;
		else
			ret = true;
		tmp.i->free(tmp);
	}
	
	free(state);
	return ret;
}

static pos_t *get_moveable(piece_ptr_t p, board_ptr_t b, size_t *size)
{
	king_t *this = p.king;

	size_t row_size = b.i->get_row_size(b);
	size_t col_size = b.i->get_col_size(b);

	pos_t *ret = (pos_t *)malloc(sizeof(pos_t) * MAX_REACHABLE_POS_SIZE);
	size_t ret_size = 0;
	for (size_t i = 0; i < row_size; i++) {
		for (size_t j = 0; j < col_size; j++) {
			pos_t pos = (pos_t) {
				.row = i,
				.col = j
			};
			if (can_move_to(this, b, pos))
				ret[ret_size++] = pos;
		}
	}

	*size = ret_size;
	return ret;
}

static bool can_reach(piece_ptr_t p, board_ptr_t b, pos_t pos)
{
	king_t *this = p.king;

	pos_t cur_pos = b.i->find(b, p);
	// TODO: castle
	if (abs(cur_pos.row - pos.row) > 1 || abs(cur_pos.col - pos.col) > 1)
		return false;
	return true;	
}

static pos_t *get_reachable(piece_ptr_t p, board_ptr_t b, size_t *size)
{
	king_t *this = p.king;

	size_t row_size = b.i->get_row_size(b);
	size_t col_size = b.i->get_col_size(b);

	pos_t *ret = (pos_t *)malloc(sizeof(pos_t) * MAX_REACHABLE_POS_SIZE);
	size_t ret_size = 0;
	// TODO: castle
	for (size_t i = 0; i < row_size; i++) {
		for (size_t j = 0; j < col_size; j++) {
			pos_t pos = (pos_t) {
				.row = i,
				.col = j
			};
			if (can_reach(this, b, pos))
				ret[ret_size++] = pos;
		}
	}

	*size = ret_size;
	return ret;
}

static side_t get_side(piece_ptr_t p)
{
	king_t *this = p.king;

	return this->side;
}
