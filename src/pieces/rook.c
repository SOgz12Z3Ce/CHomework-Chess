/**
 * @file rook.c
 * @brief implement `rook.h`
 * @author SOgz12Z3Ce
 * @date 2025-10-12
 * @version 1.0
 */

#include <stddef.h>
#include <stdbool.h>

#include "rook.h"
#include "piece.h"
#include "board.h"
#include "position.h"

#define MAX_REACHABLE_POS_SIZE 256

static piece_ptr_t copy(piece_ptr_t p);
static void free(piece_ptr_t p);
static bool is_king(piece_ptr_t p);
static bool can_reach(piece_ptr_t p, board_ptr_t b, pos_t pos);
static side_t get_side(piece_ptr_t p);
static void on_move(piece_ptr_t p, board_ptr_t b);

// TODO: castle

/** @brief the rook piece */
struct rook_t {
	piece_interface_t *i;
	side_t side; /**< @brief `SIDE_WHITE` or `SIDE_BLACK` */
	bool moved; /**< @brief is this rook moved */
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

piece_ptr_t rook_create(side_t side)
{
	rook_t *ret = (rook_t *)malloc(sizeof(rook_t));
	if (!ret) {
		printf("fatal error at rook_create: Not enough memory.\n");
		exit(1);
	}
	*ret = (rook_t) {
		.i = &vtable,
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
	rook_t ret = rook_create(this->side).rook;
	ret.moved = this->moved;

	return (piece_ptr_t) {
		.rook = ret
	};
}

static void free(piece_ptr_t p)
{
	king_t *this = p.rook;

	free(this);
}

static bool can_reach(piece_ptr_t p, board_ptr_t b, pos_t pos)
{
	rook_t *this = p.rook;

	pos_t cur_pos = b.i->find(b, p);
	if (cur_pos.row != pos.row ^ cur_pos.col != pos.col)
		return false;

	piece_ptr_t **state = b.i->get_state(b);
	if (cur_pos.row == pos.row) {
		int next = pos.col - cur_pos.col > 0 ? 1 : -1;
		for (int i = cur_pos.col + next; i != pos.col; i += next) {
			if (state[pos.row][i].i != NULL) {
				free(state);
				return false;
			}
		}
	} else {
		int next = pos.row - cur_pos.row > 0 ? 1 : -1;
		for (int i = cur_pos.row + next; i != pos.row; i += next) {
			if (state[i][pos.col].i != NULL) {
				free(state);
				return false;
			}
		}
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
