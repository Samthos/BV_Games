#include <sstream>
#include <iostream>

#include "menu.h"
#include "sdl_help.h"

#define FONT_SMALL  40
#define FONT_LARGE 120

MenuObject::MenuObject()
{
	titleSurfaceLarge = NULL;
	titleSurfaceSmall = NULL;
	state = 0;
}

MenuObject::~MenuObject()
{
	free_surface(titleSurfaceLarge);
	free_surface(titleSurfaceSmall);
}

MenuObject::MenuObject( std::string filename )
{

	std::ifstream input;
	TTF_Font *fontLarge = NULL;
	TTF_Font *fontSmall = NULL;

	//set title surfaces to null
	titleSurfaceLarge = NULL;
	titleSurfaceSmall = NULL;
	state = 0;
	
	//load fonts
	load_font( fontLarge, "assets/high_school_USA_sans.ttf", FONT_LARGE );
	load_font( fontSmall, "assets/high_school_USA_sans.ttf", FONT_SMALL );

	//read file
	input.open( filename.c_str() );
	if( input.is_open() )
	{
		read(input, fontLarge, fontSmall);
	}

	free_font( fontLarge );
	free_font( fontSmall );
}

void MenuObject::read( std::ifstream &input, TTF_Font* &fontLarge, TTF_Font* &fontSmall )
{
	unsigned int i,j;
	SDL_Color color={0,255,0};

	input >> title;
	titleSurfaceLarge = TTF_RenderText_Solid( fontLarge, title.c_str(), color );
	titleSurfaceSmall = TTF_RenderText_Solid( fontSmall, title.c_str(), color );

	input >> j;
	type = static_cast<menuObjectType>(j);

	input >> j;
	objects.resize(j);
	for(i=0;i<j;i++)
	{
		objects[i].read( input, fontLarge, fontSmall );
	}
}

void MenuObject::print()
{
	std::cout << title << "\t" << type << std::endl;
	for( auto it : objects )
	{
		it.print();
	}
}

void MenuObject::display(SDL_Window* &screen)
{
	std::string title;
	int i,j;
	bool quit = false;
	bool change = true;;

	SDL_Surface *paddle = NULL;
	SDL_Surface *background = NULL;
	SDL_Surface *screenSurface = NULL;
	SDL_Event event;
	SDL_Rect offset;

	//Set up Surface for screen
	screenSurface = SDL_GetWindowSurface( screen );
	if( screen == NULL ) 
	{
		printf("ScreenSurface not set\n");
	} 

	load_image( paddle, screenSurface->format, "assets/paddle.bmp" );
	load_image( background, screenSurface->format, "assets/title_background.bmp");

	while( quit == false )
	{
		//While there's an event to handle
		while( SDL_PollEvent( &event ) ) 
		{
			//If the user has Xed out the window
			if( SDL_QUIT == event.type )
			{
				//Quit the program
				quit = true;
			}
			if( event.type == SDL_KEYDOWN )
			{
				switch( event.key.keysym.sym )
				{
					case SDLK_UP:
						if( state > 0 )
						{
							state--;
							change = true;
						}
						break;
					case SDLK_DOWN:
						if( state < objects.size()-1 )
						{
							state++;
							change = true;
						}
						break;
					case SDLK_RETURN:
						if( objects[state].type == PLAY )
						{
						}
						else if( objects[state].type == MENU )
						{
							objects[state].display(screen);
							change = true;
						}
						else if( objects[state].type == QUIT )
						{
							quit = true;
						}
						break;
				}
			}
		}
		if(change && !quit)
		{
			change = false;

			SDL_BlitScaled(background, NULL, screenSurface, NULL);

			offset.x = (screenSurface->w - titleSurfaceLarge->w)/2;
			offset.y = 30;
			SDL_BlitSurface(titleSurfaceLarge, NULL, screenSurface, &offset);

			for(i=0;i<objects.size();i++)
			{
				//offset.x = (screenSurface->w - titleSurfaceSmall->w)/2;
				offset.x = (screenSurface->w - objects[i].titleSurfaceSmall->w)/2;
				offset.y = 200 + i*FONT_SMALL;
				SDL_BlitSurface( objects[i].titleSurfaceSmall, NULL, screenSurface, &offset);
				if(i == state)
				{
					for(j=0;j<2;j++)
					{
						offset.x = (screenSurface->w - objects[i].titleSurfaceSmall->w)/2 - (1-j)*2*paddle->w + j*(objects[i].titleSurfaceSmall->w + paddle->w/2);
						offset.y = 209 + state*FONT_SMALL;
						SDL_BlitSurface( paddle, NULL, screenSurface, &offset);
					}
				}
			}
			SDL_UpdateWindowSurface(screen);
		}
	}

	free_surface( paddle );
	free_surface( background );
	free_surface( screenSurface );
}
