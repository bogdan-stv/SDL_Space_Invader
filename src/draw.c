
#include "common.h"
#include "draw.h"

extern Game game;

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
