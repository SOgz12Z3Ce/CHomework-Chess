/**
 * @file bishop.h
 * @brief provide bishop piece
 * @author SOgz12Z3Ce
 * @date 2025-11-12
 * @version 1.0
 */

#ifndef BISHOP_H_
#define BISHOP_H_

typedef struct bishop_t bishop_t;

#include "piece.h"

/**
 * @brief create a bishop piece
 * @param side the side of bishop
 * @return `piece_ptr_t` of bishop
 */
piece_ptr_t bishop_create(side_t side);

#endif /* BISHOP_H_ */
