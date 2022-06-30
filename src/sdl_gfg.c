#include <stdio.h>

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

/*
void createVulkanInstance(SDL_Window *window) {
	if (SDL_Vulkan_LoadLibrary("") != 0) {
		fprintf(stderr, "SDL failed to load vulkan library with error: %s\n", SDL_GetError());
		// Need to exit program if vulkan fails to load.
	}

	unsigned int numExtensions;
	
	if (SDL_Vulkan_GetInstanceExtensions(window, &numExtensions, NULL) == SDL_FALSE) {
		// TODO: does this call SDL_SetError?
	}

	if (numExtensions > 0) {
		char **extensions = extensions[numExtensions][256];
		SDL_Vulkan_GetInstanceExtensions(window, numExtensions, extensions);
	}
	
	if (SDL_Vulkan_CreateSurface(window, vkInstance, &surface) == SDL_FALSE) {
		// TODO: does this call SDL_SetError?
	}
}
*/

int main(int argc, char **argv) {
	// GAMECONTROLLER automatically initializes JOYSTICK and EVENTS.
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
		fprintf(stderr, "Failed to initialize SDL due to error: %s\n", SDL_GetError());
		return 1;
	}

	// Other flags that may be needed:
	// SDL_WINDOW_FULLSCREEN
	// SDL_WINDOW_FULLSCREEN_DESKTOP (fullscreen window at desktop res)
	// SDL_WINDOW_VULKAN - implicitly loads and unloads the default vulkan lib
	// SDL_WINDOW_BORDERLESS
	// SDL_WINDOW_INPUT_GRABBED
	// SDL_WINDOW_ALLOW_HIGHDPI
	SDL_Window *window = SDL_CreateWindow("gfg",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // screen position
			WINDOW_WIDTH, WINDOW_HEIGHT,                    // window size
			SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);      // window flags

	if (window == NULL) {
		fprintf(stderr, "SDL failed to create window with error: %s\n", SDL_GetError());
		return 2;
	}

	int running = 1;

	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYUP:
					if (event.key.keysym.sym == SDLK_q) {
						running = 0;
					}
					break;
				default:
					break;
			}
		}
	}

	// SDL cleanup
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
