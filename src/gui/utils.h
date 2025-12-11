
#ifndef UTILS_H_
#define UTILS_H_

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

static const SDL_Color color_white = {
	.r = 0xFF,
	.g = 0xFF,
	.b = 0xFF,
	.a = SDL_ALPHA_OPAQUE
};
static const SDL_Color color_black = {
	.r = 0x00,
	.g = 0x00,
	.b = 0x00,
	.a = SDL_ALPHA_OPAQUE
};

bool in(float x, float y, SDL_FRect *rect);
SDL_Texture *texture_from_str(SDL_Renderer *renderer, TTF_Font *font,
                              const char *str, const SDL_Color *color);

#endif /* UTILS_H_ */
