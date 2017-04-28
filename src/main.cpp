/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//C++ headers
#include <vector>
#include <fstream>
#include <iostream>

//SDL headers

//project headers
#include "game_params.h"
#include "sdl_help.h"
#include "game.h"
#include "menu.h"

int main( int argc, char* args[] ) 
{
	int status;
	SDL_Window *screen = NULL;
	SDL_Renderer *renderer = NULL;
	gameParameters gParams;
	gParams.numPlayers = 1;
	gParams.gameMode = 0;

	//Initialize
	if( init(screen, renderer, gParams) == false ) 
	{
		return 1;
	}

#ifndef WIN32
	MenuObject menuObject( "../config/menu.xml" );
#else
	MenuObject menuObject( "menu.xml" );
#endif


	defaultColors( gParams );
	loadColors( gParams );

	status = 0;
	while( status >= 0 )
	{
		switch( status )
		{
			case 0: status = menuObject.display(screen, renderer, gParams); break;
			case 1: status = game(renderer, gParams); break;
		}
	}

	clean_up(screen);
	return 0;
}
