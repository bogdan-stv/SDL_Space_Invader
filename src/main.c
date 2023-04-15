
#include "common.h"

#include "draw.h"
#include "init.h"
#include "input.h"
#include "main.h"

Game game;
Player player;


int main(int argc, char *argv[])
{
	memset(&game, 0, sizeof(Game));
	memset(&player, 0, sizeof(Player));

	initSDL();

	player.texture = IMG_LoadTexture(game.renderer, "gfx/player.png");

	atexit(cleanup);

	while (1)
	{
		prepareScene();

		doInput();

		blit(player.texture, player.x, player.y);

		presentScene();

		SDL_Delay(16);
	}

	return 0;
}
