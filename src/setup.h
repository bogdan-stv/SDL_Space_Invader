
void initSDL(void);
void cleanup(void);

void keyDown(SDL_KeyboardEvent *event);
void keyUp(SDL_KeyboardEvent *event);
void moveEntity(void);
void doInput(void);

void prepareScene(void);
void presentScene(void);
void blit(SDL_Texture *texture, int x, int y);