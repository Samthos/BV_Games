#include "sdl_help.h"

bool init(SDL_Window* &screen, SDL_Renderer* &renderer,  gameParameters &gParams ) 
{
	//Initialize all SDL subsystems
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) 
	{
		printf("SDL did not init\n");
		return false;
	}
	if(TTF_Init()==-1) 
	{
		printf("TTF_Init: %s\n", TTF_GetError());
		return false;
	}

	gParams.scaleFactor = 1;
	gParams.width  = gParams.defaultWidth  * gParams.scaleFactor;
	gParams.height = gParams.defaultHeight * gParams.scaleFactor;

	//Set up the screen
	screen = SDL_CreateWindow("BV-PONG",
                          SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED,
                          gParams.width, gParams.height, 0);

	if( screen == NULL )
	{
		printf("Window could not be created\n");
		return false;
	}

	renderer = SDL_CreateRenderer(screen, -1, 0);

	//If everything initialized fine
	return true;
}

void clean_up(SDL_Window* &screen) 
{
	//Free the surface
	SDL_DestroyWindow( screen );
	screen = NULL;

	//Quit SDL
	SDL_Quit();
}

bool load_font( TTF_Font* &in, std::string filename, int fontSize ) 
{
	in=TTF_OpenFont(filename.c_str(), fontSize);
	if(in == NULL) 
	{
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		return false;
	}
	return true;
}

void free_font( TTF_Font* &in ) 
{
	if( in != NULL )
	{
		TTF_CloseFont(in);
		in = NULL;
	}
}

void free_surface( SDL_Surface* &surface)
{
	if( surface != NULL )
	{
		SDL_FreeSurface(surface);
		surface = NULL;
	}
}

void free_texture( SDL_Texture* &texture)
{
	if( texture != NULL )
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
}
