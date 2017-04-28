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
	int state;

	void stateInit();
	void stateUp();
	void stateDown();

	void readXML(const pugi::xml_node& );

public:
	MenuObject();
	~MenuObject();
	MenuObject(const std::string);

	int display(SDL_Window*&, SDL_Renderer*&, gameParameters&);
	void print(int);

	menuObjectType type;
	std::vector<MenuObject> objects;

	std::string title;
	int smallFontSize;
	int largeFontSize;
	int draw(SDL_Renderer*&, SDL_Color&, int, int, bool, bool);
};
