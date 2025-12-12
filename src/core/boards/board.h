/**
 * @file piece.h
 * @brief provide interface for boards
 * @author SOgz12Z3Ce
 * @date 2025-10-12
 * @version 1.0
 */

#ifndef BOARD_INTERFACE_H_
#define BOARD_INTERFACE_H_

#include <stddef.h>

#include "core/pieces/piece_base.h"
#include "core/pieces/piece.h"
#include "core/boards/stdboard.h"
#include "core/move.h"

/** @brief abstract for boards */
union __attribute__ ((__transparent_union__)) board_ptr_t {
	void *ptr;
	board_interface_t *i;
	stdboard_t *stdboard;
};

/** @brief interface of boards */
struct board_interface_t {
	/**
	 * @brief deep copy `board_ptr_t`
	 * @param p [in] `board_ptr_t` to copy
	 * @return [own] copy of `board_ptr_t`
	 */
	board_ptr_t (*copy)(board_ptr_t b);

	/**
	 * @brief free `board_ptr_t`
	 * @param p [own] `board_ptr_t` to free
	 */
	void (*free)(board_ptr_t b);

	/**
	 * @brief clear `board_ptr_t`
	 * @param b [in,out] `board_ptr_t`
	 */
	void (*clear)(board_ptr_t b);

	/**
	 * @brief get board state
	 * @param b [in] `board_ptr_t`
	 * @return [own] a two dimensional `this.row_size` * `this.col_size`
	 *               array which repersents current board state
	 */
	piece_ptr_t **(*get_state)(board_ptr_t b);

	/**
	 * @brief get last move
	 * @param b [in] the board
	 * @return last move
	 */
	move_t (*get_last_move)(board_ptr_t b);

	/**
	 * @brief get board row size (height)
	 * @param b [in] `board_ptr_t`
	 * @return row size
	 */
	size_t (*get_row_size)(board_ptr_t b);
	
	/**
	 * @brief get board column size (height)
	 * @param b [in] `board_ptr_t`
	 * @return column size
	 */
	size_t (*get_col_size)(board_ptr_t b);

	/**
	 * @brief find the `pos_t` of a `piece_ptr_t`
	 * @param b [in] `board_ptr_t`
	 * @param p [in] `piece_ptr_t` to find (must exist)
	 * @return `pos_t` of `piece_ptr_t`
	 */
	pos_t (*find)(board_ptr_t b, piece_ptr_t p);

	/**
	 * @brief put `piece_ptr_t` on given position of `board_ptr_t`
	 * @param b [in,out] `board_ptr_t`
	 * @param p [own] `piece_ptr_t` to put
	 * @param pos the position (must be a blank)
	 */
	void (*put)(board_ptr_t b, piece_ptr_t p, pos_t pos);

	/**
	 * @brief remove piece on given position of `board_ptr_t`
	 * @param b [in,out] `board_ptr_t`
	 * @param pos the position (must be a piece)
	 */
	void (*remove)(board_ptr_t b, pos_t pos);

	/**
	 * @brief get the piece ptr at position
	 * @param b [in] board
	 * @param pos the position
	 * @return [borrow] the piece
	 */
	piece_ptr_t (*at)(board_ptr_t b, pos_t pos);

	/**
	 * @brief clean and get the `piece_ptr_t` at `pos`
	 * @param b [in,out] `board_ptr_t`
	 * @param pos the position (must be a piece)
	 * @return [own] the `piece_ptr_t`
	 */
	piece_ptr_t (*pickup)(board_ptr_t b, pos_t pos);

	/**
	 * @brief move a piece on `board_ptr_t`
	 * @param b [in,out] `board_ptr_t` to add on
	 * @param m the move
	 */
	void (*move)(board_ptr_t b, move_t m);

	/**
	 * @brief check if the board is checking
	 * @param b [in] `board_ptr_t` to check
	 * @return checking status:
	 *         - `SIDE_WHITE`: white side is being checked
	 *         - `SIDE_BLACK`: black side is being checked
	 *         - `SIDE_WHITE_AND_BLACK` : white side and black side is being
	 *                                  checked
	 */
	side_t (*get_checking)(board_ptr_t b);

	/**
	 * @brief get number of squares on the board
	 * @param b[in] `board_ptr_t` to count
	 * @return number of squares
	 */
	size_t (*get_square_num)(board_ptr_t b);
};

#endif /* BOARD_INTERFACE_H_ */
