#include "sdl_help.h"
#include "player.h"
#include "ball.h"
#include <vector>

bool game( SDL_Window* &screen, SDL_Renderer* &renderer) 
{
	Uint32 t1,t2;
	bool quit = false;
	bool leave = false;
	SDL_Event event; 
	SDL_Color bgColor = { 0, 0, 0, 255};
	SDL_Color fgColor = {255, 255, 255, 255};
	SDL_Color bdyColor = {255, 0, 0, 255};
	SDL_Color ballColor = { 0, 255, 0, 255};
	SDL_Color paddleColor = {0, 255, 0, 255};

	Ball ball;
	std::vector< SDL_Rect > fg;
	std::vector< SDL_Rect > bdy;
	std::vector< Player > player(2);;
	{
		player[0].rect = {50,  210, 10, 60};
		player[0].id = 0;
		player[0].renderScore( renderer);
		player[1].rect = {580, 210, 10, 60};
		player[1].id = 1;
		player[1].renderScore( renderer);

		fg.push_back( { 50,  20, 540, 10} );
		fg.push_back( { 50, 450, 540, 10} );
		fg.push_back( {   0,   0, 640,   2} );
		fg.push_back( {   0,   0,   2, 480} );
		fg.push_back( {   0, 478, 640,   2} );
		fg.push_back( { 638,   0,   2, 480} );

		bdy.push_back({50,30,1,420});
		bdy.push_back({590,30,1,420});
		for(int i=30;i<450;i+=40)
		{
			fg.push_back({310, i, 20, 20});
		}
	}

	t1 = SDL_GetTicks();
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	while( quit == false && leave == false ) 
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
/*
			if( event.type == SDL_KEYDOWN )
			{
				switch( event.key.keysym.sym )
				{
					case SDLK_UP:
						player[0].up();
						break;
					case SDLK_DOWN:
						player[0].down();
						break;

					case SDLK_k:
						player[1].up();
						break;
					case SDLK_j:
						player[1].down();
						break;

					case SDLK_ESCAPE:
						leave = true;
						break;
				}
			}
*/
		}
		if( state[SDL_SCANCODE_J] ||  state[SDL_SCANCODE_W] )
		{
			player[0].up();
		}
		if( state[SDL_SCANCODE_K] ||  state[SDL_SCANCODE_S] )
		{
			player[0].down();
		}
		if( state[SDL_SCANCODE_UP] )
		{
			player[1].up();
		}
		if( state[SDL_SCANCODE_DOWN] )
		{
			player[1].down();
		}
		if( state[SDL_SCANCODE_ESCAPE] )
		{
			leave = true;
		}

		int s = ball.update( player );
		if( s == 1 )
		{
			player[0].score++;
			player[0].renderScore( renderer );
			ball.reset();
		}
		else if( s == 0 )
		{
			player[1].score++;
			player[1].renderScore( renderer );
			ball.reset();
		}

		SDL_SetRenderDrawColor( renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a );
		SDL_RenderClear( renderer );

		SDL_SetRenderDrawColor( renderer, fgColor.r, fgColor.g, fgColor.b, fgColor.a );
		SDL_RenderFillRects( renderer, fg.data(), fg.size());

		SDL_SetRenderDrawColor( renderer, bdyColor.r, bdyColor.g, bdyColor.b, bdyColor.a );
		SDL_RenderFillRects( renderer, bdy.data(), bdy.size());

		SDL_SetRenderDrawColor( renderer, ballColor.r, ballColor.g, ballColor.b, ballColor.a );
		SDL_RenderFillRect( renderer, &ball.coordinate );

		SDL_SetRenderDrawColor( renderer, paddleColor.r, paddleColor.g, paddleColor.b, paddleColor.a );

		SDL_RenderFillRect( renderer, &player[0].rect );
		SDL_RenderCopy( renderer, player[0].scoreTexture, NULL, &player[0].scoreRect );

		SDL_RenderFillRect( renderer, &player[1].rect );
		SDL_RenderCopy( renderer, player[1].scoreTexture, NULL, &player[1].scoreRect );

		
		SDL_RenderPresent( renderer );
		t2 = SDL_GetTicks();
		if( (t2 - t1) < 25 )
		{
			SDL_Delay( 25 - (t2-t1) );
		}
		t1 = SDL_GetTicks();
	}
	return quit;
}
