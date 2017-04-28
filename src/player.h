#include "sdl_help.h"

class Ball;

#ifndef PLAYER
#define PLAYER
class Player
{
private:
public:
	SDL_Rect rect;

	void AIMove(Ball&);
	void up();
	void down();
	void draw(SDL_Renderer* &, const SDL_Color&);
};
#endif

