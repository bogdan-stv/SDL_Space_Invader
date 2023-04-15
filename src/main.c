#include "common.h"

#include "setup.h"
#include "main.h"

Game game;
Entity player;
Entity bullet;


int main(int argc, char *argv[])
{
	memset(&game, 0, sizeof(Game));
	memset(&player, 0, sizeof(Entity));
	memset(&bullet, 0, sizeof(Entity));
	initSDL();

	player.x = 100;
	player.y = 100;
	player.texture = IMG_LoadTexture(game.renderer, "gfx/player.png");
	bullet.texture = IMG_LoadTexture(game.renderer, "gfx/playerBullet.png");

	atexit(cleanup);

	while (1)
	{
		prepareScene();

		doInput();
		moveEntity();

		blit(player.texture, player.x, player.y);
		if(bullet.health == 1)
		{
			blit(bullet.texture, bullet.x, bullet.y);
		}

		presentScene();

		SDL_Delay(16);
	}

	return 0;
}