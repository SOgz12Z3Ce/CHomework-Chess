/**
 * @file board.h
 * @brief provide board struct
 * @author Wu HaoYu
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
 * @brief initialize the board
 * @param b ptr of `Board` to initialize
 */
Board *board_get();

/**
 * @brief clear the board
 * @param b ptr of `Board`
 * @return 0 for success
 */
int borad_clear(Board *b);

/**
 * @brief put a piece on given position of the board
 * @param b ptr of `Board`
 * @param row row of position
 * @param col column of position
 * @param p ptr of piexe to add
 * @return 0 for success
 */
int board_put(Board *b, size_t row, size_t col, Piece *p);

/**
 * @brief remove a piece on given position of the board
 * @param b ptr of `Board`
 * @param row row of position
 * @param col column of position
 * @return 0 for success
 */
int board_remove(Board *b, size_t row, size_t col);

/**
 * @brief move a piece on the board
 * @param b ptr of `Board` to add on
 * @param s_row row of start position
 * @param s_col column of start position
 * @param e_row row of end position
 * @param e_col column of end position
 * @return 0 for success
 */
int board_move(Board *b, size_t s_row, size_t s_col, size_t e_row, size_t e_col);

/**
 * @brief initialize the board to standard opening status
 * @param b ptr of `Board` to initialize
 */
void board_init(Board *b, int status);

#endif /* BOARD_H_ */
