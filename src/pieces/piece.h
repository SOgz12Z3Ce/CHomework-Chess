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

#include "piece_base.h"
#include "board.h"
#include "position.h"
#include "king.h"
#include "rook.h"
#include "queen.h"
#include "bishop.h"
#include "knight.h"
#include "pawn.h"

/** @brief abstract for pieces */
union __attribute__ ((__transparent_union__)) piece_ptr_t {
	void *ptr;
	piece_interface_t *i;
	king_t *king;
	queen_t *queen;
	rook_t *rook;
	bishop_t *bishop;
	knight_t *knight;
	pawn_t *pawn;
};

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
	 * @brief check if the `piece_ptr_t` moved
	 * @param p [in] `piece_ptr_t` to check
	 */
	bool (*is_moved)(piece_ptr_t p);

	/**
	 * @brief check the type of piece
	 * @param p [in] piece to check
	 * @param type [in] type
	 * @return check if the `p` is `type`
	 */
	bool (*is)(piece_ptr_t p, const char *const type);

	/**
	 * @brief check if the `piece_ptr_t` can walk(no capture) to the given
	 *        `pos_t`
	 * @param p [in] `piece_ptr_t` to check
	 * @param b [in] `board_ptr_t` to check (must contain `p`)
	 * @param pos the `pos_t` (must be at `b` and blank)
	 */
	bool (*can_walk)(piece_ptr_t p, board_ptr_t b, pos_t pos);

	/**
	 * @brief check if the `piece_ptr_t` can control the given `pos_t`
	 * @param p [in] `piece_ptr_t` to check
	 * @param b [in] `board_ptr_t` to check (must contain `p`)
	 * @param pos the `pos_t` (must be at `b`)
	 */
	bool (*can_control)(piece_ptr_t p, board_ptr_t b, pos_t pos);

	/**
	 * @brief check if the `piece_ptr_t` can capture the given `pos_t`
	 * @param p [in] `piece_ptr_t` to check
	 * @param b [in] `board_ptr_t` to check (must contain `p`)
	 * @param pos the `pos_t` (must be at `b` and a enemy piece)
	 */
	bool (*can_attack)(piece_ptr_t p, board_ptr_t b, pos_t pos);

	/**
	 * @brief check if the `piece_ptr_t` can move to the given `pos_t`
	 * @param p [in] `piece_ptr_t` to check
	 * @param b [in] `board_ptr_t` to check (must contain `p`)
	 * @param pos the `pos_t` (must be at `b`)
	 */
	bool (*can_move)(piece_ptr_t p, board_ptr_t b, pos_t pos);

	/**
	 * @brief get all positions that make callback return true as array
	 * @param p [in] `piece_ptr_t`
	 * @param b [in] `board_ptr_t` (must contain `p`)
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
	 * @param b [in,out] `board_ptr_t` (must contain `p`)
	 */
	void (*on_move)(piece_ptr_t p, board_ptr_t b);
};

/* base methods */

void piece_free(piece_ptr_t p);
bool piece_is_moved(piece_ptr_t p);
bool piece_can_walk(piece_ptr_t p, board_ptr_t b, pos_t pos);
bool piece_can_attack(piece_ptr_t p, board_ptr_t b, pos_t pos);
bool piece_can_move(piece_ptr_t p, board_ptr_t b, pos_t pos);
pos_t *piece_all(piece_ptr_t p, board_ptr_t b, size_t *size,
                 all_callback_t callback);
void piece_on_move(piece_ptr_t p, board_ptr_t b);

/* other methods */

bool is_same_side(piece_ptr_t p1, piece_ptr_t p2);
size_t get_moveable_num(piece_ptr_t p, board_ptr_t b,
                        int next_col, int next_row);

#endif /* PIECE_INTERFACE_H_ */
