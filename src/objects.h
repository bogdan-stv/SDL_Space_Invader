
typedef struct Entity Entity;

typedef struct {
	SDL_Renderer *renderer;
	SDL_Window   *window;
    int keyboard[MAX_KEYBOARD_KEYS];
	
} Game;

struct Entity{
    float x;
    float y;
    float dx;
    float dy;
    int w;
    int h;
    int health;
    int reload;
    int side;
    SDL_Texture *texture;
    Entity *next;
};

typedef struct {
    Entity shipHead, *shipTail;
    Entity bulletHead, *bulletTail;
} Level;