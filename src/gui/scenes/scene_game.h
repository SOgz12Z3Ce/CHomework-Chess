#ifndef SCENE_GAME_H_
#define SCENE_GAME_H_

#include <stddef.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "gui/button.h"
#include "gui/common.h"

void get_game_buttons(button_t **dest, size_t *buttons_index, SDL_Renderer *renderer, TTF_Font *font, appstate_t *as);

#endif /* SCENE_GAME_H_ */
