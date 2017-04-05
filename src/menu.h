#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "pugixml.hpp"
#include "game_params.h"

enum menuObjectType{ MENU, PLAY, QUIT, TEXT, UP, OPTION };

class MenuObject
{
private:
	bool textRendered;
	int smallFontSize;
	int largeFontSize;
	int state;

	void stateInit();
	void stateUp();
	void stateDown();

	void readXML( pugi::xml_node& );
	void renderText( SDL_Renderer*& );

public:
	MenuObject();
	~MenuObject();
	MenuObject(std::string);

	int display(SDL_Window*&, SDL_Renderer*&, gameParameters&);
	void read( std::ifstream&, TTF_Font*&, TTF_Font*& );
	void print(int);

	menuObjectType type;
	std::string title;
	std::vector<MenuObject> objects;

	SDL_Surface* titleSurfaceLarge;
	SDL_Surface* titleSurfaceSmall;
	SDL_Texture* titleTextureLarge;
	SDL_Texture* titleTextureSmall;
};
