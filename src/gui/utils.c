#include "utils.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

bool in(float x, float y, SDL_FRect *rect)
{
	return (x >= rect->x && x <= rect->x + rect->w)
	       && (y >= rect->y && y <= rect->y + rect->h);
}

SDL_Texture *texture_from_str(SDL_Renderer *renderer, const TTF_Font *font,
                              const char *str, const SDL_Color *color)
{
	SDL_Surface *text = TTF_RenderText_Blended(font, s, 0, *color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text);
	SDL_DestroySurface(text);
	
	return texture;
}
