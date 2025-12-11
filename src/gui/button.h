#ifndef BUTTON_H_
#define BUTTON_H_

#include <SDL3/SDL.h>
#include "common.h"

typedef enum button_state_t button_state_t;
typedef struct button_t button_t;
typedef void (*button_callback_t)(button_t *, appstate_t *);

enum button_state_t {
	BUTTON_DEFAULT,
	BUTTON_PRESSED,
	BUTTON_DRAGGING
};
struct button_t {
	SDL_FRect rect;
	button_state_t state;
	SDL_Color color;
	SDL_Texture *content;
	bool movetable;
	button_callback_t init;
	button_callback_t on_press;
	button_callback_t on_release;
	button_callback_t on_click;
	button_callback_t on_dragging;
};

button_t *create_button(
	SDL_FRect rect,
	SDL_Color color,
	SDL_Texture *content,
	bool movetable,
	button_callback_t init,
	button_callback_t on_press,
	button_callback_t on_release,
	button_callback_t on_click,
	button_callback_t on_dragging
);
void nop(button_t *self, appstate_t *as);
void button_update(SDL_Renderer *renderer, button_t *self, appstate_t *as);

#endif /* BUTTON_H_ */
