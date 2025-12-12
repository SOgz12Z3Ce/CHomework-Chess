/**
 * @file state.h
 * @brief provide state method
 * @author SOgz12Z3Ce
 * @date 2025-10-15
 * @version 1.0
 */

#ifndef STATE_H_
#define STATE_H_

#include <stddef.h>
#include <stdlib.h>

#include "core/pieces/piece.h"

void state_free(piece_ptr_t **state, size_t row_size);
piece_ptr_t **state_create(size_t row_size, size_t col_size);
piece_ptr_t **state_copy(piece_ptr_t **state,
                         size_t row_size, size_t col_size);

#endif /* STATE_H_ */
