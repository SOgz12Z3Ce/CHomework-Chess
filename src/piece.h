/**
 * @file piece.h
 * @brief provide struct for pieces
 * @author Wu HaoYu
 * @date 2025-10-11
 * @version 1.0
 */

#ifndef PIECE_H_
#define PIECE_H_

/** @brief rename struct */
typedef struct Piece Piece;

/** 
 * @brief create `Piece`
 * @param id the unique identifier of `Piece`
 */
Piece *piece_create(const char *id);

/**
 * @brief free `Piece`
 * @param p the `Piece`
 */
void piece_free(Piece *p);

#endif /* PIECE_H_ */
