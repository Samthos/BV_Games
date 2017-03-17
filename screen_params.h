#ifndef GAME_PARAM
#define GAME_PARAM
struct gameParameters
{
	int width,height; //screen width and height
	const int defaultWidth  = 640;
	const int defaultHeight = 480;
	double scaleFactor;
};
#endif
