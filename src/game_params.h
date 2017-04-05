#ifndef GAME_PARAM
#define GAME_PARAM
struct gameParameters
{
	int width,height; //screen width and height
	const int defaultWidth  = 640;
	const int defaultHeight = 480;
	double scaleFactor;

	int gameMode;   //0 for score; 1 for time
	int numPlayers; //2 for two player mode; 1 for one player mode;
};
#endif
