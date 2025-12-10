
#ifndef UTILS_H_
#define UTILS_H_

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

bool in(float x, float y, SDL_FRect *rect);
SDL_Texture *texture_from_str(SDL_Renderer *renderer, const TTF_Font *font,
                              const char *str, const SDL_Color *color);

#endif /* UTILS_H_ */
