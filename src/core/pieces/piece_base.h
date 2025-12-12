#ifndef PIECE_BASE_H_
#define PIECE_BASE_H_

#include <stdint.h>
#include <stdbool.h>

#include "core/pieces/piece_base.h"
#include "core/boards/board_base.h"
#include "core/position.h"

/** @brief represents white and black side */
typedef uint64_t side_t;
#define SIDE_WHITE 0b01
#define SIDE_BLACK 0b10
#define SIDE_WHITE_AND_BLACK 0b11

typedef union piece_ptr_t piece_ptr_t;
typedef bool (*all_callback_t)(piece_ptr_t, board_ptr_t, pos_t);
typedef struct piece_interface_t piece_interface_t;

#endif /* PIECE_BASE_H_ */
