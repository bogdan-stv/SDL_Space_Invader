
#include "common.h"

#include "setup.h"
#include "level.h"
#include "utils.h"


extern Game game;
extern Level level;

static void fireBullet(void);
static void spawnShips(void);
static int shipHit(Entity *b);
static void fireEnemyBullet(Entity *s);

static void doPlayer(void);
static void doBullets(void);
static void doShips(void);
static void doEnemies(void);

static void drawBullets(void);
static void drawShips(void);

static void resetLevel(void);

static void clipPlayer(void);

static Entity *player;
static SDL_Texture *bulletTexture;
static SDL_Texture *shipTexture;
static SDL_Texture *enemyBulletTexture;
static int shipSpawnTimer;
static int levelResetTimer;

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
    player->side = SIDE_PLAYER;
    player->health = 1;
    player->texture = IMG_LoadTexture(game.renderer, "gfx/player.png");
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);

    bulletTexture = IMG_LoadTexture(game.renderer, "gfx/playerBullet.png");
    enemyBulletTexture = IMG_LoadTexture(game.renderer, "gfx/alienBullet.png");
    shipTexture = IMG_LoadTexture(game.renderer, "gfx/enemy.png");

    shipSpawnTimer = 0;

    levelResetTimer = FPS * 2;
}

static void resetLevel(void)
{
   Entity *e;

   while(level.shipHead.next)
   {
        e = level.shipHead.next;
        level.shipHead.next = e->next;
        free(e);
   }

   while(level.bulletHead.next)
   {
        e = level.bulletHead.next;
        level.bulletHead.next = e->next;
        free(e);
   }

    initLevel();
}

static int shipHit(Entity *b)
{
    Entity *s;

    for(s = level.shipHead.next; s != NULL; s = s->next)
    {
		if (s->side != b->side && collision(b->x, b->y, b->w, b->h, s->x, s->y, s->w, s->h))
		{
			b->health = 0;
			s->health = 0;

            // if(s == player)
            // {
            //     player = NULL;
            // }

			return 1;
		}
	}

	return 0;
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
    bullet->side = SIDE_PLAYER;
	bullet->health = 1;
	bullet->texture = bulletTexture;
	SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

	bullet->y += (player->h / 2) - (bullet->h / 2);

	player->reload = 8;
}

static void fireEnemyBullet(Entity *s)
{
    Entity *bullet;

    bullet = malloc(sizeof(Entity));
	memset(bullet, 0, sizeof(Entity));
	level.bulletTail->next = bullet;
	level.bulletTail = bullet;

    bullet->x = s->x;
    bullet->y = s->y;
    bullet->health = 1;
    bullet->texture = enemyBulletTexture;
    bullet->side = SIDE_ALIEN;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->x += (s->w / 2) - (bullet->w / 2);
	bullet->y += (s->h / 2) - (bullet->h / 2);

    calcSlope(player->x + (player->w / 2), player->y + (player->h / 2), s->x, s->y, &bullet->dx, &bullet->dy);

    bullet->dx *= ENEMY_BULLET_SPEED;
    bullet->dy *= ENEMY_BULLET_SPEED;

    s->reload = 100;

}

//-------------------------------------LOGIC--------------------------------//
void logic(void)
{
	doPlayer();

    doEnemies();
    
    doShips();

	doBullets();

    spawnShips();

    clipPlayer();

    if(player == NULL)
    {
        resetLevel();
    }

}

static void doEnemies(void)
{
	Entity *e;

	for (e = level.shipHead.next ; e != NULL ; e = e->next)
	{
		if (e != player && player != NULL && --e->reload <= 0)
		{
			fireEnemyBullet(e);
		}
	}
}

static void spawnShips(void)
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
        ship->side = SIDE_ALIEN;
        ship->texture = shipTexture;
        ship->health = 1;
        SDL_QueryTexture(ship->texture, NULL, NULL, &ship->w, &ship->h);

        if(ship->y < ship->h)
        {
            ship->y += ship->h;
        }

        if(ship->y > SCREEN_HEIGHT - ship->h)
        {
            ship->y -= ship->h;
        }

        ship->dx = -(2 + (rand() % 4));

        ship->reload = FPS * (1 + (rand() % 3));

        shipSpawnTimer = 30 + (rand() % FPS);
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

		if (b->x > SCREEN_WIDTH || shipHit(b) || b->x < 0 || b->x < -b->w || b->y < -b->h)
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

        if(s != player && s->x < -s->w)
        {
            s->health = 0;
        }

        if(s->health == 0)
        {
            if(s == player)
            {
                player = NULL;
            }
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

static void clipPlayer(void)
{
    if (player != NULL)
	{
		if (player->x < 0)
		{
			player->x = 0;
		}

		if (player->y < 0)
		{
			player->y = 0;
		}

		if (player->x > SCREEN_WIDTH / 2)
		{
			player->x = SCREEN_WIDTH / 2;
		}

		if (player->y > SCREEN_HEIGHT - player->h)
		{
			player->y = SCREEN_HEIGHT - player->h;
		}
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