/**
 * @file state.c
 * @brief implement `state.h`
 * @author SOgz12Z3Ce
 * @date 2025-10-22
 * @version 1.0
 */

#include <stddef.h>
#include <string.h>

#include "core/state.h"
#include "core/pieces/piece.h"
#include "core/mallocer.h"

void state_free(piece_ptr_t **state, size_t row_size)
{
	for (size_t i = 0; i < row_size; i++)
		free(state[i]);
	free(state);
}

piece_ptr_t **state_create(size_t row_size, size_t col_size)
{
	piece_ptr_t **state = new_array(piece_ptr_t *, row_size);
	for (size_t i = 0; i < row_size; i++) {
		state[i] = new_array(piece_ptr_t, col_size);
		memset(state[i], 0, sizeof(piece_ptr_t) * col_size);
	}
	return state;
}

piece_ptr_t **state_copy(piece_ptr_t **state,
                        size_t row_size, size_t col_size)
{
	piece_ptr_t **ret = state_create(row_size, col_size);
	for (size_t i = 0; i < row_size; i++) {
		for (size_t j = 0; j < col_size; j++) {
			piece_ptr_t tmp = state[i][j];
			if (!tmp.ptr)
				continue;
			ret[i][j] = tmp.i->copy(tmp);
		}
	}
	return ret;
}