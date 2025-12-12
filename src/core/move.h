/**
 * @file move.h
 * @brief provide move structure
 * @author SOgz12Z3Ce
 * @date 2025-10-22
 * @version 1.0
 */

#ifndef MOVE_H_
#define MOVE_H_

#include <stdint.h>

#include "core/position.h"

/* struct of a move */
typedef struct move
{
	pos_t src; /*> the source position of a move */
	pos_t dest; /*> the destination position of a move */
	char disambiguation;
} move_t;

const static move_t move_null = {
	.src = 0,
	.dest = 0
};

#define move_create(s, d) ((move_t){.src = (s), .dest = (d), .disambiguation = '\0'})

#endif /* MOVE_H_ */
