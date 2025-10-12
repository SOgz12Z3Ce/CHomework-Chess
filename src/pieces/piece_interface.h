/**
 * @file piece_interface.h
 * @brief provide interface for pieces
 * @author SOgz12Z3Ce
 * @date 2025-10-12
 * @version 1.0
 */

#ifndef PIECE_INTERFACE_H_
#define PIECE_INTERFACE_H_

#include <stddef.h>
#include <stdbool.h>

#include "board_interface.h"
#include "king.h"

typedef struct piece_interface_t piece_interface_t;

/** @brief abstract for pieces */
typedef union __attribute__ ((__transparent_union__)) {
	piece_interface_t *i;
	// king_t *king;
	// queen_t *queen;
	// rook_t *rook;
	// bishop_t *bishop;
	// knight_t *knight;
	// pawn_t *pawn;
} piece_ptr_t;

/** @brief interface of pieces */
struct piece_interface_t {
	/**
	 * @brief free `piece_ptr_t`
	 * @param p [own] `piece_ptr_t` to free
	 */
	void (*free)(piece_ptr_t p);

	/**
	 * @brief check if the `piece_ptr_t` is counted as king
	 * @param p [in] `piece_ptr_t` to check
	 */
	bool (*is_king)(piece_ptr_t p);

	/**
	 * @brief check if the `piece_ptr_t` can move to the given position
	 * @param p [in] `piece_ptr_t` to check
	 * @param b [in] `board_ptr_t` to check
	 * @param row row of the position
	 * @param col column of the position
	 */
	bool (*can_move_to)(piece_ptr_t p,
	                    board_ptr_t b,
	                    size_t row, size_t col);
};

#endif /* PIECE_INTERFACE_H_ */
