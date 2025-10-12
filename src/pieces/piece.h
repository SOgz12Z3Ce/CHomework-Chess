/**
 * @file piece.h
 * @brief provide interface for pieces
 * @author SOgz12Z3Ce
 * @date 2025-10-12
 * @version 1.0
 */

#ifndef PIECE_INTERFACE_H_
#define PIECE_INTERFACE_H_

#include <stddef.h>
#include <stdbool.h>

#include "board.h"
#include "king.h"
#include "rook.h"
#include "position.h"

/** @brief represents white and black side */
typedef int side_t;
#define SIDE_WHITE 0b01
#define SIDE_BLACK 0b10
#define SIDE_WHITE_AND_BLACK 0b11

typedef bool (*all_callback_t)(piece_ptr_t, board_ptr_t, pos_t);
typedef struct piece_interface_t piece_interface_t;

/** @brief abstract for pieces */
typedef union __attribute__ ((__transparent_union__)) {
	piece_interface_t *i;
	king_t *king;
	// queen_t *queen;
	rook_t *rook;
	// bishop_t *bishop;
	// knight_t *knight;
	// pawn_t *pawn;
} piece_ptr_t;

/** @brief interface of pieces */
struct piece_interface_t {
	/**
	 * @brief deep copy `piece_ptr_t`
	 * @param p [in] `piece_ptr_t` to copy
	 * @return [own] copy of `piece_ptr_t`
	 */
	piece_ptr_t (*copy)(piece_ptr_t p);

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
	 * @brief check if the `piece_ptr_t` can reach the given `pos_t`
	 * @param p [in] `piece_ptr_t` to check
	 * @param b [in] `board_ptr_t` to check
	 * @param pos the `pos_t`
	 */
	bool (*can_reach)(piece_ptr_t p, board_ptr_t b, pos_t pos);

	/**
	 * @brief get all positions that make callback return true as array
	 * @param p [in] `piece_ptr_t`
	 * @param b [in] `board_ptr_t`
	 * @param size [out] returned array size
	 * @param callback callback func
	 * @return [own] reachable `pos_t` array
	 */
	pos_t *(*all)(piece_ptr_t p, board_ptr_t b, size_t *size,
	              all_callback_t callback);

	/**
	 * @brief get `piece_ptr_t` side
	 * @param p [in] `piece_ptr_t`
	 * @return side of `piece_ptr_t`:
	 *         - `SIDE_WHITE`: white side
	 *         - `SIDE_BLACK`: black side
	 */
	side_t (*get_side)(piece_ptr_t p);

	/**
	 * @brief take actions when `piece_ptr_t` is moved
	 * @param p [in,out] `piece_ptr_t` to take action
	 * @param b [in,out] `board_ptr_t`
	 */
	void (*on_move)(piece_ptr_t p, board_ptr_t b);
};

/* base methods */

void piece_free(piece_ptr_t p);
bool piece_is_king(piece_ptr_t p);
bool piece_can_move_to(piece_ptr_t p, board_ptr_t b, pos_t pos);
pos_t *piece_all(piece_ptr_t p, board_ptr_t b, size_t *size,
                 all_callback_t callback);
void piece_on_move(piece_ptr_t p, board_ptr_t b);

inline bool is_same_side(piece_ptr_t p1, piece_ptr_t p2)
{
	return p1.i->get_side(p1) == p2.i->get_side(p2);
}

#endif /* PIECE_INTERFACE_H_ */
