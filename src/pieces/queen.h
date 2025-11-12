/**
 * @file queen.h
 * @brief provide queen piece
 * @author SOgz12Z3Ce
 * @date 2025-11-12
 * @version 1.0
 */

#ifndef QUEEN_H_
#define QUEEN_H_

typedef struct queen_t queen_t;

#include "piece.h"

/**
 * @brief create a queen piece
 * @param side the side of queen
 * @return `piece_ptr_t` of queen
 */
piece_ptr_t queen_create(side_t side);

#endif /* QUEEN_H_ */
