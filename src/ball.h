#include "sdl_help.h"
#include "player.h"
#include <vector>

#ifndef BALL
#define BALL
class Ball
{
private:
public:
	SDL_Rect coordinate;
	double xVel, yVel;

	Ball();

	int update(std::vector<Player>&);
	void reset();
	void draw(SDL_Renderer* &, const SDL_Color&);
};
#endif
