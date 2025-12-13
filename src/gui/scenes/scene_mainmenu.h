#ifndef SCENE_MAINMENU_H_
#define SCENE_MAINMENU_H_

#include <stddef.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "gui/button.h"

void get_mainmenu_buttons(button_t **dest, size_t *buttons_index, SDL_Renderer *renderer, TTF_Font *font);

#endif /* SCENE_MAINMENU_H_ */
