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

#include "piece.h"

typedef struct board_interface_t board_interface_t;

/** @brief abstract for boards */
typedef union __attribute__ ((__transparent_union__)) {
	board_interface_t *i;
} board_ptr_t;

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
	 * @return [own] a two dimensional array which repersents current
	 *               board state
	 */
	piece_ptr_t **(*get_state)(board_ptr_t b);

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
	 * @param p [in] `piece_ptr_t` to find
	 * @return `pos_t` of `piece_ptr_t`
	 */
	pos_t (*find)(board_ptr_t b, piece_ptr_t p);

	/**
	 * @brief put `piece_ptr_t` on given position of `board_ptr_t`
	 * @param b [in,out] `board_ptr_t`
	 * @param p [own] `piece_ptr_t` to put
	 * @param pos the position
	 */
	void (*put)(board_ptr_t b, piece_ptr_t p, pos_t pos);

	/**
	 * @brief remove piece on given position of `board_ptr_t`
	 * @param b [in,out] `board_ptr_t`
	 * @param pos the position
	 */
	void (*remove)(board_ptr_t b, pos_t pos);

	/**
	 * @brief move a piece on `board_ptr_t`
	 * @param b [in,out] `board_ptr_t` to add on
	 * @param cur_pos current position
	 * @param dest_pos dest position
	 */
	void (*move)(board_ptr_t b, pos_t cur_pos, pos_t dest_pos);

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
};

#endif /* BOARD_INTERFACE_H_ */
