/**
 * @file knight.h
 * @brief provide knight piece
 * @author SOgz12Z3Ce
 * @date 2025-11-12
 * @version 1.0
 */

#ifndef KNIGHT_H_
#define KNIGHT_H_

typedef struct knight_t knight_t;

#include "core/pieces/piece.h"

/**
 * @brief create a knight piece
 * @param side the side of knight
 * @return `piece_ptr_t` of knight
 */
piece_ptr_t knight_create(side_t side);

#endif /* KNIGHT_H_ */
