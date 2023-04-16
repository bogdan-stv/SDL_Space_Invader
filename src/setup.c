
#include "common.h"
#include "setup.h"

extern Game game;

//----------------------------INIT--------------------------------//

void initSDL(void)
{
	int rendererFlags, windowFlags;

	rendererFlags = SDL_RENDERER_ACCELERATED;

	windowFlags = 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	game.window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

	game.renderer = SDL_CreateRenderer(game.window, -1, rendererFlags);

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

}



void cleanup(void)
{
	SDL_DestroyRenderer(game.renderer);

	SDL_DestroyWindow(game.window);

	SDL_Quit();
}

//----------------------------INPUT--------------------------------//

void keyDown(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0 &&  event->keysym.scancode < MAX_KEYBOARD_KEYS)
    {
        game.keyboard[event->keysym.scancode] = 1;
    }

}

void keyUp(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0 &&  event->keysym.scancode < MAX_KEYBOARD_KEYS)
    {
        game.keyboard[event->keysym.scancode] = 0;
    }
}

void doInput(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				exit(0);
				break;
            case SDL_KEYDOWN:
                keyDown(&event.key);
                break;
            case SDL_KEYUP:
                keyUp(&event.key);
                break;
			default:
				break;
		}
	}
}

//----------------------------DRAW--------------------------------//

void prepareScene(void)
{
	SDL_SetRenderDrawColor(game.renderer, 96, 128, 255, 255);
	SDL_RenderClear(game.renderer);
}

void presentScene(void)
{
	SDL_RenderPresent(game.renderer);
}

void blit(SDL_Texture *texture, int x, int y)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(game.renderer, texture, NULL, &dest);
}