
#include "common.h"

#include "setup.h"
#include "level.h"

extern Game game;
extern Level level;

static void fireBullet(void);
static void spawnShips(void);

static void doPlayer(void);
static void doBullets(void);
static void doShips(void);

static void drawBullets(void);
static void drawShips(void);

static Entity *player;
static SDL_Texture *bulletTexture;
static SDL_Texture *shipTexture;
static int shipSpawnTimer;

void initLevel(void)
{
    // init level
    memset(&level, 0, sizeof(Level));
    level.shipTail = &level.shipHead;
    level.bulletTail = &level.bulletHead;
    
    //init player
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    level.shipTail->next = player;
    level.shipTail = player;

    player->x = 100;
    player->y = 100;

    player->texture = IMG_LoadTexture(game.renderer, "gfx/player.png");
    bulletTexture = IMG_LoadTexture(game.renderer, "gfx/playerBullet.png");
    shipTexture = IMG_LoadTexture(game.renderer, "gfx/enemy.png");

    shipSpawnTimer = 0;
}


static void fireBullet(void)
{
	Entity *bullet;

	bullet = malloc(sizeof(Entity));
	memset(bullet, 0, sizeof(Entity));
	level.bulletTail->next = bullet;
	level.bulletTail = bullet;

	bullet->x = player->x;
	bullet->y = player->y;
	bullet->dx = PLAYER_BULLET_SPEED;
	bullet->health = 1;
	bullet->texture = bulletTexture;
	SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

	bullet->y += (player->h / 2) - (bullet->h / 2);

	player->reload = 8;
}

//-------------------------------------LOGIC--------------------------------//
void logic(void)
{
	doPlayer();

	doBullets();

    spawnShips();

    doShips();
}

static void spawnShips()
{
    Entity *ship;

    if (--shipSpawnTimer <= 0)
    {
        ship = malloc(sizeof(Entity));
        memset(ship, 0, sizeof(Entity));
        level.shipTail->next = ship;
        level.shipTail = ship;

        ship->x = SCREEN_WIDTH;
        ship->y = rand() % SCREEN_HEIGHT;
        ship->texture = shipTexture;
        SDL_QueryTexture(ship->texture, NULL, NULL, &ship->w, &ship->h);

        ship->dx = -(2 + (rand() % 4));

        shipSpawnTimer = 30 + (rand() % 60);
    }
}

static void doPlayer(void)
{
	player->dx = player->dy = 0;

	if (player->reload > 0)
	{
		player->reload--;
	}

	if (game.keyboard[SDL_SCANCODE_UP])
	{
		player->dy = -PLAYER_SPEED;
	}

	if (game.keyboard[SDL_SCANCODE_DOWN])
	{
		player->dy = PLAYER_SPEED;
	}

	if (game.keyboard[SDL_SCANCODE_LEFT])
	{
		player->dx = -PLAYER_SPEED;
	}

	if (game.keyboard[SDL_SCANCODE_RIGHT])
	{
		player->dx = PLAYER_SPEED;
	}

	if (game.keyboard[SDL_SCANCODE_LCTRL] && player->reload == 0)
	{
		fireBullet();
	}

	player->x += player->dx;
	player->y += player->dy;
}

static void doBullets(void)
{
	Entity *b, *prev;

	prev = &level.bulletHead;

	for (b = level.bulletHead.next ; b != NULL ; b = b->next)
	{
		b->x += b->dx;
		b->y += b->dy;

		if (b->x > SCREEN_WIDTH)
		{
			if (b == level.bulletTail)
			{
				level.bulletTail = prev;
			}

			prev->next = b->next;
			free(b);
			b = prev;
		}

		prev = b;
	}
}

static void doShips(void)
{
    Entity *s, *prev;
    
    prev = &level.shipHead;

    for(s = level.shipHead.next; s != NULL; s = s->next)
    {
        s->x += s->dx;
        s->y += s->dy;

        if( s!= player && s->x < -s->w)
        {
            if (s == level.shipTail)
            {
                level.shipTail = prev;
            }
            prev->next = s->next;
            free(s);
            s = prev;
        }
        prev = s;
    }
}

//--------------------------------DRAW---------------------------------//
void draw(void)
{
    drawShips();
	drawBullets();
}

static void drawBullets(void)
{
	Entity *b;

	for (b = level.bulletHead.next ; b != NULL ; b = b->next)
	{
		blit(b->texture, b->x, b->y);
	}
}

static void drawShips(void)
{
    Entity *s;

    for(s = level.shipHead.next; s != NULL; s = s->next)
    {
        blit(s->texture, s->x, s->y);
    }
}