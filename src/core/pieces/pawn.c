/**
 * @file pawn.c
 * @brief implement `pawn.h`
 * @author SOgz12Z3Ce
 * @date 2025-11-12
 * @version 1.0
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "core/pieces/pawn.h"
#include "core/pieces/piece.h"
#include "core/boards/board.h"
#include "core/position.h"
#include "core/mallocer.h"
#include "core/pieces/distance.h"

static piece_ptr_t copy(piece_ptr_t p);
static bool is_moved(piece_ptr_t p);
static bool is(piece_ptr_t p, const char *const type);
static bool can_walk(piece_ptr_t p, board_ptr_t b, pos_t pos);
static bool can_control(piece_ptr_t p, board_ptr_t b, pos_t pos);
static side_t get_side(piece_ptr_t p);
static void on_move(piece_ptr_t p, board_ptr_t b);

static int get_forward(pawn_t *p);

/** @brief the pawn piece */
struct pawn_t {
	piece_interface_t i;
	side_t side; /**< @brief `SIDE_WHITE` or `SIDE_BLACK` */
	bool moved; /**< @brief is this pawn moved */
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
	.get_side = get_side,
	.on_move = piece_on_move,
};

piece_ptr_t pawn_create(side_t side)
{
	pawn_t *ret = new(pawn_t);
	if (!ret) {
		printf("fatal error at pawn_create: Not enough memory.\n");
		exit(1);
	}
	*ret = (pawn_t) {
		.i = vtable,
		.side = side
	};
	return (piece_ptr_t) {
		.pawn = ret
	};
}

static piece_ptr_t copy(piece_ptr_t p)
{
	pawn_t *this = p.pawn;

	return pawn_create(this->side);
}

static bool is_moved(piece_ptr_t p)
{
	pawn_t *this = p.pawn;

	return this->moved;
}

static bool is(piece_ptr_t p, const char *const type)
{
	return strcmp(type, "pawn") == 0;
}

static bool can_walk(piece_ptr_t p, board_ptr_t b, pos_t pos)
{
	pawn_t *this = p.pawn;

	if (b.i->at(b, pos).ptr)
		return false;

	pos_t cur_pos = b.i->find(b, p);

	/* the direction of "forward" for this pawn */
	int forward = get_forward(this);

	/* normal move */
	if (cur_pos.row + forward == pos.row && cur_pos.col == pos.col)
		return true;
	
	/* first move */
	if (cur_pos.row + 2 * forward == pos.row && cur_pos.col == pos.col)
		return true;
	
	return false;
}

static bool can_control(piece_ptr_t p, board_ptr_t b, pos_t pos)
{
	pawn_t *this = p.pawn;
	pos_t cur_pos = b.i->find(b, p);
	int forward = get_forward(this);
	
	return cur_pos.row + forward == pos.row && dist(cur_pos.col, pos.col) == 1;
}

static side_t get_side(piece_ptr_t p)
{
	pawn_t *this = p.pawn;

	return this->side;
}

static int get_forward(pawn_t *p)
{
	return p->side == SIDE_BLACK ? 1 : -1;
}

static void on_move(piece_ptr_t p, board_ptr_t b)
{
	pawn_t *this = p.pawn;

	move_t last_move = b.i->get_last_move(b);
	if (last_move.dest.row != 0)
		return;

	piece_ptr_t tmp_p;
	switch (last_move.disambiguation) {
		case 'q':
			tmp_p = queen_create(this->side);
			break;
		case 'r':
			tmp_p = rook_create(this->side);
			break;
		case 'b':
			tmp_p = bishop_create(this->side);
			break;
		case 'n':
			tmp_p = knight_create(this->side);
			break;
	}
	pos_t cur_pos = b.i->find(b, p);
	b.i->remove(b, cur_pos);
	b.i->put(b, tmp_p, cur_pos);
}
