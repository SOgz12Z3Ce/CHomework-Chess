#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "piece.h"

#define MAX_ID_LENGTH 256

/**
 * @brief check if piece id is valid
 * @return true if valid; false if invalid
 */
static bool check_id_valid(const char *id);

/**
 * @brief check if piece id represents a king
 * @return true if represents king; false if doesn't represent king
 */
static bool is_king(const char *id);

/**
 * @brief a piece
 */
struct Piece {
	char id[MAX_ID_LENGTH]; /**< unique identifier of piece */
	bool is_king;
	bool is_moved;
};

static bool check_id_valid(const char *id)
{
	static const char* valid[] = {
		"white_king",
		"white_queen",
		"white_rook",
		"white_bishop",
		"white_knight",
		"white_pawn",
		"black_king",
		"black_queen",
		"black_rook",
		"black_bishop",
		"black_knight",
		"black_pawn"
	};
	for (int i = 0; i < sizeof(valid) / sizeof(const char*); i++)
		if (strcmp(valid[i], id) == 0)
			return true;
	return false;
}

static bool is_king(const char *id)
{
	static const char* kings[] = {
		"white_king",
		"black_king"
	};
	for (int i = 0; i < sizeof(kings) / sizeof(const char*); i++)
		if (strcmp(kings[i], id) == 0)
			return true;
	return false;
}

Piece *piece_create(const char *id)
{
	if (!id) {
		printf("fatal error at piece_create():\n"
		       "\tInvaild argument(s):\n"
		       "\t\tid: NULL pointer.\n");
		exit(1);
	}
	if (!check_id_valid(id)) {
		printf("fatal error at piece_create():\n"
		       "\tInvaild argument(s):\n"
		       "\t\tid: invalid piece id(%s).\n", id);
		exit(1);
	}
	Piece *ret = (Piece*)malloc(sizeof(Piece));
	if (!ret) {
		printf("fatal error at piece_create():\n"
		       "\tNot enough memory.\n");
		exit(1);
	}
	*ret = (Piece) {
		.is_king = is_king(id),
		.is_moved = false
	};
	strcpy(ret->id, id);
	return ret;
}

void piece_free(Piece *p)
{
	if(!p) {
		printf("fatal error at piece_free():\n"
		       "\tInvaild argument(s):\n"
		       "\t\tp: NULL pointer.\n");
		exit(1);
	}
	free(p);
}
