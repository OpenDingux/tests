#include "SDL.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool running = true;

void handleEvents()
{
	SDL_Event event;
	if (SDL_WaitEvent(&event)) {
		switch(event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_SPACE:
					case SDLK_RETURN:
					case SDLK_ESCAPE:
						running = false;
						break;
					default:
						break;
				}
				break;
			case SDL_QUIT:
				running = false;
				break;
			default:
				break;
		}
	} else {
		running = false;
	}
}

void paint(SDL_Surface *screen, SDL_Surface *image)
{
	SDL_BlitSurface(image, NULL, screen, NULL);
}

int main(int argc, char **argv)
{
	(void)argv; (void)argc;

	SDL_Init(SDL_INIT_VIDEO);
	atexit(SDL_Quit);

	SDL_ShowCursor(SDL_DISABLE);
	SDL_Surface *screen = SDL_SetVideoMode(240, 160, 16, SDL_HWSURFACE);
	if (!screen) {
		fprintf(stderr, "ERROR: Failed to set video mode.\n");
		exit(1);
	}
	if (screen->format->BitsPerPixel != 16) {
		fprintf(stderr, "ERROR: Did not get 16 bpp, got %u bpp instead.\n",
				screen->format->BitsPerPixel);
		exit(1);
	}
	if (!(screen->flags & SDL_HWSURFACE)) {
		fprintf(stderr, "WARNING: Did not get a hardware surface.\n");
	}

	SDL_Surface *org_image = SDL_LoadBMP("test-pattern-240-160.bmp");
	if (!org_image) {
		fprintf(stderr, "ERROR: Failed to load test image.\n");
		exit(1);
	}
	SDL_Surface *image = SDL_DisplayFormat(org_image);
	SDL_FreeSurface(org_image);

	while (running) {
		paint(screen, image);
		SDL_Flip(screen);
		handleEvents();
	}

	SDL_FreeSurface(image);

	return 0;
}
