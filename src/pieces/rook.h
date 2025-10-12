/**
 * @file rook.h
 * @brief provide rook piece
 * @author SOgz12Z3Ce
 * @date 2025-10-12
 * @version 1.0
 */

#ifndef ROOK_H_
#define ROOK_H_

#include "piece.h"

typedef struct rook_t rook_t;

/**
 * @brief create a rook piece
 * @param side the side of rook
 * @return `piece_ptr_t` of rook
 */
piece_ptr_t rook_create(side_t side);

#endif /* ROOK_H_ */
