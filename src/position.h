/**
 * @file position.h
 * @brief provide position structure
 * @author SOgz12Z3Ce
 * @date 2025-10-12
 * @version 1.0
 */

#ifndef POSITION_H_
#define POSITION_H_

#include <stddef.h>

#define pos_create(r, c) ((pos_t){.row = (r), .col = (c)})

typedef struct pos {
	size_t row;
	size_t col;
} pos_t;

#endif /* POSITION_H_ */
