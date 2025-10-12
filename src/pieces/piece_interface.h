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
#include "position.h"

/** @brief macros for piece_interface_t.get_side() */
typedef int side_t;
#define SIDE_WHITE 0
#define SIDE_BLACK 1

typedef struct piece_interface_t piece_interface_t;

/** @brief abstract for pieces */
typedef union __attribute__ ((__transparent_union__)) {
	piece_interface_t *i;
	king_t *king;
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
	 * @brief check if the `piece_ptr_t` can move to the given `pos_t`
	 * @param p [in] `piece_ptr_t` to check
	 * @param b [in] `board_ptr_t` to check
	 * @param pos the `pos_t`
	 */
	bool (*can_move_to)(piece_ptr_t p, board_ptr_t b, pos_t pos);
	
	/**
	 * @brief get moveable positions array
	 * @param p [in] `piece_ptr_t`
	 * @param b [in] `board_ptr_t`
	 * @param size [out] returned array size
	 * @return [own] moveable `pos_t` array
	 */
	pos_t *(*get_moveable)(piece_ptr_t p, board_ptr_t b, size_t *size);

	/**
	 * @brief check if the `piece_ptr_t` can reach the given `pos_t`
	 * @param p [in] `piece_ptr_t` to check
	 * @param b [in] `board_ptr_t` to check
	 * @param pos the `pos_t`
	 */
	bool (*can_reach)(piece_ptr_t p, board_ptr_t b, pos_t pos);
	
	/**
	 * @brief get reachable positions array
	 * @param p [in] `piece_ptr_t`
	 * @param b [in] `board_ptr_t`
	 * @param cur_pos current `pos_t`
	 * @param size [out] returned array size
	 * @return [own] reachable `pos_t` array
	 */
	pos_t *(*get_reachable)(piece_ptr_t p, board_ptr_t b, size_t *size);
	
	/**
	 * @brief get `piece_ptr_t` side
	 * @param p [in] `piece_ptr_t`
	 * @return side of `piece_ptr_t`:
	 *         - SIDE_WHITE: white side
	 *         - SIDE_BLACK: black side
	 */
	side_t (*get_side)(piece_ptr_t p);
};

#endif /* PIECE_INTERFACE_H_ */
