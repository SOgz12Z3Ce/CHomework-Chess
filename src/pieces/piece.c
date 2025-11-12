/**
 * @file king.c
 * @brief implement `piece.h`
 * @author SOgz12Z3Ce
 * @date 2025-10-12
 * @version 1.0
 */

#include <stddef.h>
#include <stdbool.h>

#include "piece.h"
#include "board.h"
#include "mallocer.h"

void piece_free(piece_ptr_t p)
{
	free(p.ptr);
}

bool piece_can_attack(piece_ptr_t p, board_ptr_t b, pos_t pos)
{
	if (!p.i->can_control(p, b, pos))
		return false;

	piece_ptr_t target = b.i->at(b, pos);
	if (!target.ptr)
		return false;
	if (is_same_side(p, target))
		return false;
	return true;
}

bool piece_can_move(piece_ptr_t p, board_ptr_t b, pos_t pos)
{
	pos_t cur_pos = b.i->find(b, p);

	int break_;

	/* check reachable */
	if (!p.i->can_walk(p, b, pos) && !p.i->can_attack(p, b, pos))
		return false;

	/* check attack same side piece */
	piece_ptr_t target = b.i->at(b, pos);
	if (target.ptr && is_same_side(p, target))
		return false;

	/* check is board valid */
	board_ptr_t tmp = b.i->copy(b);
	tmp.i->move(tmp, move_create(cur_pos, pos));
	if (tmp.i->get_checking(tmp) & p.i->get_side(p)) {
		/* cause checked */
		tmp.i->free(tmp);
		return false;
	}

	tmp.i->free(tmp);
	return true;
}

pos_t *piece_all(piece_ptr_t p, board_ptr_t b, size_t *size,
                 all_callback_t callback)
{
	size_t row_size = b.i->get_row_size(b);
	size_t col_size = b.i->get_col_size(b);

	pos_t *ret = new_array(pos_t, b.i->get_square_num(b));
	size_t ret_size = 0;
	for (size_t i = 0; i < row_size; i++) {
		for (size_t j = 0; j < col_size; j++) {
			pos_t pos = pos_create(i, j);
			if (callback(p, b, pos))
				ret[ret_size++] = pos;
		}
	}

	*size = ret_size;
	return ret;
}

void piece_on_move(piece_ptr_t p, board_ptr_t b)
{
	// do nothing
}

bool is_same_side(piece_ptr_t p1, piece_ptr_t p2)
{
	return p1.i->get_side(p1) == p2.i->get_side(p2);
}

size_t get_moveable_num(piece_ptr_t p, board_ptr_t b,
                        int next_col, int next_row)
{
	size_t col_size = b.i->get_col_size(b);
	size_t row_size = b.i->get_row_size(b);

	size_t ret = 0;
	pos_t pos = b.i->find(b, p);
	do {
		pos.col += next_col;
		pos.row += next_row;
		ret++;
	} while ((pos.col < col_size && pos.row < row_size)
	         && !b.i->at(b, pos).ptr);
	
	return ret - 1;
}
