#include "sdl_help.h"

#ifndef PLAYER
#define PLAYER
class Player
{
private:
public:
	SDL_Rect rect;
	int score;
	int id;

	SDL_Rect scoreRect;
	SDL_Surface* scoreSurface;
	SDL_Texture* scoreTexture;

	Player();
	~Player();
	Player (const Player&);
	Player& operator= (const Player&);
	Player (Player&&);
	Player& operator= (Player&&);

	void up();
	void down();
	void renderScore( SDL_Renderer*&);
};
#endif

