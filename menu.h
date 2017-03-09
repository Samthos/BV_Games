#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

enum menuObjectType{ MENU, PLAY, QUIT };

class MenuObject
{
private:
	int state;
public:
	MenuObject();
	~MenuObject();
	MenuObject(std::string);

	void display(SDL_Window*&);
	void read( std::ifstream&, TTF_Font*&, TTF_Font*& );
	void print();

	menuObjectType type;
	std::string title;
	std::vector<MenuObject> objects;

	SDL_Surface* titleSurfaceLarge;
	SDL_Surface* titleSurfaceSmall;
};
