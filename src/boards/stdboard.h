/**
 * @file stdboard.h
 * @brief provide standard board
 * @author SOgz12Z3Ce
 * @date 2025-10-12
 * @version 1.0
 */

#ifndef STANDARD_BOARD_H_
#define STANDARD_BOARD_H_

typedef struct stdboard_t stdboard_t;

#include "board.h"

/**
 * @brief create a standard board
 * @return `board_ptr_t` of the standard board
 */
board_ptr_t stdboard_create();

#endif /* STANDARD_BOARD_H_ */
