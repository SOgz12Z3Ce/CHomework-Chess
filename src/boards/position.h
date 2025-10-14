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

typedef struct pos {
	size_t row;
	size_t col;
} pos_t;

inline pos_t pos_create(size_t row, size_t col)
{
	return (pos_t) {
		.row = row,
		.col = col
	};
}

#endif /* POSITION_H_ */
