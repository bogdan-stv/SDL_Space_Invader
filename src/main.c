#include "common.h"

#include "setup.h"
#include "level.h"

Game game;
Level level;

static void capFrameRate(long *then, float *remainder);

int main(int argc, char *argv[])
{
	long then;
	float remainder;

	memset(&game, 0, sizeof(Game));

	initSDL();

	atexit(cleanup);

	initLevel();

	then = SDL_GetTicks();

	remainder = 0;

	while (1)
	{
		prepareScene();

		doInput();

		logic();

		draw();

		presentScene();

		capFrameRate(&then, &remainder);
	}

	return 0;
}

static void capFrameRate(long *then, float *remainder)
{
	long wait, frameTime;

	wait = 16 + *remainder;

	*remainder -= (int)*remainder;

	frameTime = SDL_GetTicks() - *then;

	wait -= frameTime;

	if (wait < 1)
	{
		wait = 1;
	}

	SDL_Delay(wait);

	*remainder += 0.667;

	*then = SDL_GetTicks();
}