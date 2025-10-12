/**
 * @file king.h
 * @brief provide king piece
 * @author SOgz12Z3Ce
 * @date 2025-10-12
 * @version 1.0
 */

#ifndef KING_H_
#define KING_H_

#include "piece.h"

typedef struct king_t king_t;

/**
 * @brief create a king piece
 * @param side the side of king
 * @return 
 */
piece_ptr_t king_create(side_t side);

#endif /* KING_H_ */
