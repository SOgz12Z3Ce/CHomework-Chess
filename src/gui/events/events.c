#include "gui/events/events.h"
#include <SDL3/SDL.h>
#include "gui/common.h"
#include "gui/utils.h"
#include "gui/button.h"

void try_press_button(appstate_t *as, const SDL_Event *event, button_t *button)
{
	if (event->button.button != SDL_BUTTON_LEFT)
		return;
	if (button->state != BUTTON_DEFAULT)
		return;

	float x = event->button.x;
	float y = event->button.y;

	if (in(x, y, &button->rect)) {
		button->on_press(button, as);
		button->state = BUTTON_PRESSED;
	}
}

void try_release_button(appstate_t *as, const SDL_Event *event, button_t *button)
{
	if (event->button.button != SDL_BUTTON_LEFT)
		return;

	switch (button->state) {
		case BUTTON_DEFAULT:
			break;
		case BUTTON_PRESSED:
			button->on_release(button, as);
			float x = event->button.x;
			float y = event->button.y;
			if (in(x, y, &button->rect)) {
				button->on_click(button, as);
			}
			break;
		case BUTTON_DRAGGING:
			button->on_release(button, as);
			break;
	}
	button->state = BUTTON_DEFAULT;
}

void try_drag_button(appstate_t *as, const SDL_Event *event, button_t *button)
{
	if (!(event->button.button & SDL_BUTTON_LMASK))
		return;

	if (button->state == BUTTON_PRESSED && button->moveable)
		button->state = BUTTON_DRAGGING;
	if (button->state != BUTTON_DRAGGING)
		return;

	button->on_dragging(button, as);
	button->rect.x += event->motion.xrel;
	button->rect.y += event->motion.yrel;
}
