/**
 * @file board.h
 * @brief provide board struct
 * @author SOgz12Z3Ce
 * @date 2025-10-11
 * @version 1.0
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <stddef.h>
#include "piece.h"

/** @brief macro for board_init() */
#define SET_EMPTY 0
#define SET_STANDARD 1

/** @brief board struct */
typedef struct Board Board;

/**
 * @brief create `Board`
 * @return [own] ptr of created `Board`
 */
Board *board_create();

/**
 * @brief free `Board`
 * @param p [own] ptr of `Board` to free
 */
void board_free(Board *b);

/**
 * @brief clear `Board`
 * @param b [in,out] ptr of `Board`
 * @return 0 for success
 */
int borad_clear(Board *b);

/**
 * @brief put `Piece` on given position of `Board`
 * @param b [in,out] ptr of `Board`
 * @param row row of position
 * @param col column of position
 * @param p [own] ptr of `Piece` to put
 * @return 0 for success
 */
int board_put(Board *b, size_t row, size_t col, Piece *p);

/**
 * @brief remove `Piece` on given position of `Board`
 * @param b [in,out] ptr of `Board`
 * @param row row of position
 * @param col column of position
 * @return 0 for success
 */
int board_remove(Board *b, size_t row, size_t col);

/**
 * @brief move a `Piece` on `Board`
 * @param b [in,out] ptr of `Board` to add on
 * @param s_row row of start position
 * @param s_col column of start position
 * @param e_row row of end position
 * @param e_col column of end position
 * @return 0 for success
 */
int board_move(Board *b, size_t s_row, size_t s_col, size_t e_row, size_t e_col);

/**
 * @brief initialize `Board` to standard opening status
 * @param b [out] ptr of `Board` to initialize
 */
void board_init(Board *b);

#endif /* BOARD_H_ */
