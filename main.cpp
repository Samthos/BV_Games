/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//C++ headers
#include <vector>
#include <fstream>

//SDL headers

//project headers
#include "screen_params.h"
#include "sdl_help.h"
#include "menu.h"

//Screen attributes
const int FONT_SIZE_LARGE = 120; 
const int FONT_SIZE_MEDIUM = 40; 

void menu(SDL_Window* &screen)
{
	std::string title;
	int i,j;
	int state = 0;
	bool quit = false;
	bool change = true;;

	std::ifstream input;
	std::vector<std::string> text;

	SDL_Color color={0,255,0};
	SDL_Surface *paddle =NULL;
	SDL_Surface *message = NULL;
	SDL_Surface *background = NULL;
	SDL_Surface *screenSurface = NULL;
	SDL_Event event;
	TTF_Font *fontLarge, *fontMed;

	input.open("assets/menu.dat");
	if( input.is_open() )
	{
		std::string str;
		while( getline( input, str ) )
		{
			text.push_back(str);
		}
	}

	//Set up Surface for screen
	screenSurface = SDL_GetWindowSurface( screen );
	if( screen == NULL ) 
	{
		printf("ScreenSurface not set\n");
	}

	load_image( background, screenSurface->format, "assets/title_background.bmp");
	load_image( paddle, screenSurface->format, "assets/paddle.bmp" );
	load_font( fontLarge, "assets/high_school_USA_sans.ttf", FONT_SIZE_LARGE);
	load_font( fontMed, "assets/high_school_USA_sans.ttf", FONT_SIZE_MEDIUM);

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
						if( state < text.size()-1 )
						{
							state++;
							change = true;
						}
						break;
					case SDLK_RETURN:
						switch(state)
						{
							case 3:
							quit = true;
							break;
						}
						break;
				}
			}
		}
		if(change)
		{
			change = false;

			SDL_BlitScaled(background, NULL, screenSurface, NULL);
			
			title = "PONG";
			message = TTF_RenderText_Solid( fontLarge, title.c_str(), color );
			if( message != NULL )
			{
				SDL_Rect offset;
				offset.x = (screenSurface->w - message->w)/2;
				offset.y = 30;
				SDL_BlitSurface(message, NULL, screenSurface, &offset);
				free_surface(message);
			}

			title = "Play";
			for(i = 0; i < text.size(); i++)
			{
				message = TTF_RenderText_Solid( fontMed, text[i].c_str(), color );
				if( message != NULL )
				{
					SDL_Rect offset;
					offset.x = (screenSurface->w - message->w)/2;
					offset.y = 200 + i*FONT_SIZE_MEDIUM;
					SDL_BlitSurface(message, NULL, screenSurface, &offset);
				}
				if(i == state)
				{
					for(j=0;j<2;j++)
					{
						SDL_Rect offset;
						offset.x = (screenSurface->w - message->w)/2 - (1-j)*2*paddle->w + j*(message->w + paddle->w/2);
						offset.y = 209 + state*FONT_SIZE_MEDIUM;
						SDL_BlitSurface( paddle, NULL, screenSurface, &offset);
					}
				}
				free_surface(message);
			}
			SDL_UpdateWindowSurface(screen);
		}
	}

	free_surface( background );
	free_surface( paddle );
	free_surface( screenSurface );
}

int main( int argc, char* args[] ) 
{
	SDL_Window *screen = NULL;
//	menuObject.print();

	//Initialize
	if( init(screen) == false ) 
	{
		return 1;
	}

	MenuObject menuObject( "menu.dat" );

	menuObject.display(screen);

//	menu(screen);

	clean_up(screen);
	return 0;
}
