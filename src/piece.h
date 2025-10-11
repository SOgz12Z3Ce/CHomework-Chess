/**
 * @file piece.h
 * @brief provide struct for pieces
 * @author SOgz12Z3Ce
 * @date 2025-10-11
 * @version 1.0
 */

#ifndef PIECE_H_
#define PIECE_H_

/** @brief rename struct */
typedef struct Piece Piece;

/** 
 * @brief create `Piece`
 * @param id [in] the unique identifier of `Piece` to create
 * @return [own] ptr of created `Piece`
 */
Piece *piece_create(const char *id);

/**
 * @brief free `Piece`
 * @param p [own] ptr of `Piece` to free
 */
void piece_free(Piece *p);

#endif /* PIECE_H_ */
