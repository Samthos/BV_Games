#include <SDL2/SDL.h>
#include <vector>
#include <string>

#ifndef GAME_PARAM
#define GAME_PARAM

namespace Color
{
	enum Color_Type {bg, fg, bdy, text, ball, paddle_p1, paddle_p2, numColor};
	const std::vector< std::string > Color_String = {"bg", "fg", "bdy", "text", "ball", "paddle_p1", "paddle_p2"};
	struct Color_Scheme
	{
		std::string name;
		std::vector<SDL_Color> colors;
	};
}

struct gameParameters
{
	int width,height; //screen width and height
	const int defaultWidth  = 640;
	const int defaultHeight = 480;
	double scaleFactor;

	std::vector<SDL_Color> colors;
	
	int currentColorScheme;
	std::vector<Color::Color_Scheme> colorSchemeList;

	int gameMode;   //0 for score; 1 for time
	int numPlayers; //2 for two player mode; 1 for one player mode;
};

void defaultColors(gameParameters&);
void loadColors(gameParameters&);
void nextColorScheme(gameParameters&);
void prevColorScheme(gameParameters&);
bool setColors(gameParameters&, const std::string);
#endif
