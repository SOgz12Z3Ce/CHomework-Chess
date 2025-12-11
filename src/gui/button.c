#include "gui/button.h"
#include <stdlib.h>
#include <SDL3/SDL.h>
#include "gui/common.h"

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
)
{
	button_t *ret = (button_t *)malloc(sizeof(button_t));
	(*ret) = (button_t){
		.rect = rect,
		.state = BUTTON_DEFAULT,
		.content = content,
		.movetable = movetable,
		.init = init,
		.on_press = on_press,
		.on_release = on_release,
		.on_click = on_click,
		.on_dragging = on_dragging
	};

	return ret;
}

void nop(button_t *self, appstate_t *as)
{
	/* do nothing */
}

void button_update(SDL_Renderer *renderer, button_t *self, appstate_t *as)
{
	/* main */
	SDL_Color color = self->color;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &self->rect);

	/* content */
	if (!self->content)
		return;
	SDL_FRect dest = {
		.x = self->rect.x + (self->rect.w - self->content->w) / 2,
		.y = self->rect.y + (self->rect.h - self->content->h) / 2,
		.w = self->content->w,
		.h = self->content->h
	};
	SDL_RenderTexture(renderer, self->content, NULL, &dest);
}
