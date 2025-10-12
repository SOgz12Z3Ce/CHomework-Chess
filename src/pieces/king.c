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

#define abs(x) ((x) > 0 ? (x) : (-(x)))

static piece_ptr_t copy(piece_ptr_t p);
static void free(piece_ptr_t p);
static bool is_king(piece_ptr_t p);
static bool can_reach(piece_ptr_t p, board_ptr_t b, pos_t pos);
static side_t get_side(piece_ptr_t p);
static void on_move(piece_ptr_t p, board_ptr_t b);

// TODO: castle

/** @brief the king piece */
struct king_t {
	piece_interface_t *i;
	side_t side; /**< @brief `SIDE_WHITE` or `SIDE_BLACK` */
	bool moved; /**< @brief is this king moved */
};

static const piece_interface_t vtable = (piece_interface_t) {
	.copy = copy,
	.free = free,
	.is_king = is_king,
	.can_move_to = piece_can_move_to,
	.can_reach = can_reach,
	.all = piece_all,
	.get_side = get_side
};

piece_ptr_t king_create(side_t side)
{
	king_t *ret = (king_t *)malloc(sizeof(king_t));
	if (!ret) {
		printf("fatal error at king_create: Not enough memory.\n");
		exit(1);
	}
	*ret = (king_t) {
		.i = &vtable,
		.side = side,
		.moved = false
	};
	return (piece_ptr_t) {
		.king = ret
	};
}

static piece_ptr_t copy(piece_ptr_t p)
{
	king_t *this = p.king;
	king_t ret = king_create(this->side).king;
	ret.moved = this->moved;

	return (piece_ptr_t) {
		.king = ret
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

static bool can_reach(piece_ptr_t p, board_ptr_t b, pos_t pos)
{
	king_t *this = p.king;

	pos_t cur_pos = b.i->find(b, p);
	if (cur_pos.row == pos.row && cur_pos.col == pos.col)
		return false;
	// TODO: castle
	if (abs(cur_pos.row - pos.row) > 1 || abs(cur_pos.col - pos.col) > 1)
		return false;
	return true;
}

static side_t get_side(piece_ptr_t p)
{
	king_t *this = p.king;

	return this->side;
}

static void on_move(piece_ptr_t p, board_ptr_t b)
{
	king_t *this = p.king;

	this->moved = true;
}
