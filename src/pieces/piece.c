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

bool piece_is_king(piece_ptr_t p)
{
	return false;
}

bool piece_can_move_to(piece_ptr_t p, board_ptr_t b, pos_t pos)
{
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
		piece_ptr_t target = state[pos.row][pos.col];
		if (target.ptr)
			target.i->free(target);
		tmp.i->put(tmp, p.i->copy(p), pos);

		if (tmp.i->get_checking(tmp) & p.i->get_side(p))
			ret = false;
		else
			ret = true;
		tmp.i->free(tmp);
	}
	
	free(state);
	return ret;
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
			pos_t pos = (pos_t) {
				.row = i,
				.col = j
			};
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
