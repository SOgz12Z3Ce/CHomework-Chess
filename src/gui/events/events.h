#ifndef EVENTS_H_
#define EVENTS_H_

#include <SDL3/SDL.h>
#include "gui/common.h"
#include "gui/button.h"

void try_press_button(appstate_t *as, const SDL_Event *event, button_t *button);
void try_release_button(appstate_t *as, const SDL_Event *event, button_t *button);
void try_drag_button(appstate_t *as, const SDL_Event *event, button_t *button);

#endif /* EVENTS_H_ */
