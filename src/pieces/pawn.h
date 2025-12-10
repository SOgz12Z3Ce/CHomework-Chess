/**
 * @file pawn.h
 * @brief provide pawn piece
 * @author SOgz12Z3Ce
 * @date 2025-11-12
 * @version 1.0
 */

#ifndef PAWN_H_
#define PAWN_H_

typedef struct pawn_t pawn_t;

#include "piece.h"

/**
 * @brief create a pawn piece
 * @param side the side of pawn
 * @return `piece_ptr_t` of pawn
 */
piece_ptr_t pawn_create(side_t side);

#endif /* PAWN_H_ */
