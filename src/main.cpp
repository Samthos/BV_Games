/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//C++ headers
#include <vector>
#include <fstream>
#include <iostream>

//SDL headers

//project headers
#include "screen_params.h"
#include "sdl_help.h"
#include "menu.h"

int main( int argc, char* args[] ) 
{
	SDL_Window *screen = NULL;
	SDL_Renderer *renderer = NULL;
	gameParameters gParams;

	//Initialize
	if( init(screen, renderer, gParams) == false ) 
	{
		return 1;
	}

	MenuObject menuObject( "config/menu.xml" );
	menuObject.display(screen, renderer);

	clean_up(screen);
	return 0;
}
