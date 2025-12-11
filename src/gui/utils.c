#include "utils.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

bool in(float x, float y, SDL_FRect *rect)
{
	return (x >= rect->x && x <= rect->x + rect->w)
	       && (y >= rect->y && y <= rect->y + rect->h);
}

SDL_Texture *texture_from_str(SDL_Renderer *renderer, TTF_Font *font,
                              const char *str, const SDL_Color *color)
{
	SDL_Surface *text = TTF_RenderText_Blended(font, str, 0, *color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text);
	SDL_DestroySurface(text);
	
	return texture;
}

SDL_Texture *texture_from_svg(SDL_Renderer *renderer, const char *path, size_t w, size_t h)
{
	SDL_IOStream *ios = SDL_IOFromFile(path, "r");
	SDL_Surface *piece = IMG_LoadSizedSVG_IO(ios, w, h);
	SDL_CloseIO(ios);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, piece);
	
	return texture;
}
