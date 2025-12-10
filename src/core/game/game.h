#include <stdint.h>
#include "core/boards/board.h"

typedef enum game_state_t game_state_t;
typedef struct game_t game_t;

enum game_state_t {
	GAME_DEFAULT,
	GAME_UPDATING
};
struct game_t {
	game_state_t state;
	board_ptr_t board;
	char *command;
};

void game_update(game_t *game, const char command[]);
