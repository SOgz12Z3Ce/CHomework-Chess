/**
 * @file piece_interface.h
 * @brief provide interface for boards
 * @author SOgz12Z3Ce
 * @date 2025-10-12
 * @version 1.0
 */

#ifndef BOARD_INTERFACE_H_
#define BOARD_INTERFACE_H_

#include <stddef.h>

#include "piece_interface.h"

typedef struct board_interface_t board_interface_t;

/** @brief abstract for boards */
typedef union __attribute__ ((__transparent_union__)) {
	board_interface_t *i;
} board_ptr_t;

/** @brief interface of boards */
struct board_interface_t {
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
	 * @brief put `piece_ptr_t` on given position of `board_ptr_t`
	 * @param b [in,out] `board_ptr_t`
	 * @param row row of position
	 * @param col column of position
	 * @param p [own] `piece_ptr_t` to put
	 */
	void (*put)(board_ptr_t b,
	            size_t row, size_t col,
	            piece_ptr_t p);

	/**
	 * @brief remove piece on given position of `board_ptr_t`
	 * @param b [in,out] `board_ptr_t`
	 * @param row row of position
	 * @param col column of position
	 */
	void (*remove)(board_ptr_t *b,
	               size_t row, size_t col);

	/**
	 * @brief move a piece on `board_ptr_t`
	 * @param b [in,out] `board_ptr_t` to add on
	 * @param s_row row of start position
	 * @param s_col column of start position
	 * @param e_row row of end position
	 * @param e_col column of end position
	 */
	void (*move)(board_ptr_t *b,
	             size_t s_row, size_t s_col,
	             size_t e_row, size_t e_col);
};

#endif /* BOARD_INTERFACE_H_ */
