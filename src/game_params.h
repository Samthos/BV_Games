#include <SDL2/SDL.h>

#ifndef GAME_PARAM
#define GAME_PARAM
struct gameParameters
{
	int width,height; //screen width and height
	const int defaultWidth  = 640;
	const int defaultHeight = 480;
	double scaleFactor;

	SDL_Color bgColor = { 0, 0, 0, 255};
	SDL_Color fgColor = {255, 255, 255, 255};
	SDL_Color bdyColor = {255, 0, 0, 255};
	SDL_Color ballColor = { 0, 255, 0, 255};
	SDL_Color paddle_p1_Color = {0, 255, 0, 255};
	SDL_Color paddle_p2_Color = {0, 255, 0, 255};

	int gameMode;   //0 for score; 1 for time
	int numPlayers; //2 for two player mode; 1 for one player mode;
};

void loadColors(gameParameters &gParam);
#endif
