#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_W 1920
#define WINDOW_H 1080
#define START_W 500
#define START_H 100
#define START_X (WINDOW_W / 2 - START_W / 2)
#define START_Y (WINDOW_H / 2 - START_H / 2)
#define EXIT_W 50
#define EXIT_H 50
#define EXIT_X (WINDOW_W - EXIT_W)
#define EXIT_Y 0

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef struct button_t {
	SDL_FRect rect;
	bool pressing;
} button_t;

static bool try_press_button(const SDL_Event *event, button_t *button);
static bool try_release_button(const SDL_Event *event, button_t *button);

static const SDL_Color color_white = {
	.r = 0xFF,
	.g = 0xFF,
	.b = 0xFF,
	.a = SDL_ALPHA_OPAQUE
};
static button_t start_b = {
	.rect = {
		.x = START_X,
		.y = START_Y,
		.w = START_W,
		.h = START_H
	},
	.pressing = false
};
static button_t exit_b = {
	.rect = {
		.x = EXIT_X,
		.y = EXIT_Y,
		.w = EXIT_W,
		.h = EXIT_H
	},
	.pressing = false
};
static button_t *buttons[] = {
	&start_b,
	&exit_b
};
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *font = NULL;
static size_t count = 0;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
	/* env */
	char font_path[256];
	strcpy(font_path, SDL_GetBasePath());
	strcat(font_path, "assets/font/SourceHanSansSC-VF.otf");

	/* basic */
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(u8"游戏", WINDOW_W, WINDOW_H, SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(window, NULL);
	SDL_SetRenderLogicalPresentation(renderer, WINDOW_W, WINDOW_H, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	/* front */
	TTF_Init();
	font = TTF_OpenFont(font_path, 48);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
	if (event->type == SDL_EVENT_QUIT)
		return SDL_APP_SUCCESS;
	if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		for (size_t i = 0; i < ARRAY_SIZE(buttons); i++)
			try_press_button(event, buttons[i]);
		return SDL_APP_CONTINUE;
	}
	if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
		if (try_release_button(event, &exit_b))
			return SDL_APP_SUCCESS;
		if (try_release_button(event, &start_b))
			count++;
		start_b.pressing = false;
		exit_b.pressing = false;
		return SDL_APP_CONTINUE;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
	/* clear */
	SDL_SetRenderDrawColor(renderer, 0x35, 0x35, 0x35, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	/* start button */
	if (start_b.pressing)
		SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, SDL_ALPHA_OPAQUE);
	else
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &start_b.rect);

	/* exit button */
	if (exit_b.pressing)
		SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, SDL_ALPHA_OPAQUE);
	else
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &exit_b.rect);

	/* debug text */
	char buffer[256];
	sprintf(buffer, "%llu", count);
	SDL_Surface *text = TTF_RenderText_Blended(font, buffer, 0, color_white);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text);
	SDL_FRect dest = {
		.x = 0,
		.y = 0,
		.w = text->w,
		.h = text->h
	};
	SDL_RenderTexture(renderer, texture, NULL, &dest);
	SDL_DestroySurface(text);
	SDL_DestroyTexture(texture);

	SDL_RenderPresent(renderer);
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{

}

static bool try_press_button(const SDL_Event *event, button_t *button)
{
	if (event->type != SDL_EVENT_MOUSE_BUTTON_DOWN)
		return false;
	if (event->button.button != SDL_BUTTON_LEFT)
		return false;

	float x = event->button.x;
	float y = event->button.y;

	if (x >= button->rect.x && x <= button->rect.x + button->rect.w
	    && y >= button->rect.y && y <= button->rect.y + button->rect.h) {
		button->pressing = true;
		return true;
	}

	return false;
}

static bool try_release_button(const SDL_Event *event, button_t *button)
{
	if (event->type != SDL_EVENT_MOUSE_BUTTON_UP)
		return false;
	if (event->button.button != SDL_BUTTON_LEFT)
		return false;

	float x = event->button.x;
	float y = event->button.y;

	if (x >= button->rect.x && x <= button->rect.x + button->rect.w
	    && y >= button->rect.y && y <= button->rect.y + button->rect.h
	    && button->pressing) {
		button->pressing = false;
		return true;
	}

	return false;
}
