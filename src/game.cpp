#include "sdl_help.h"
#include "player.h"
#include "ball.h"
#include <vector>

int display_message(SDL_Renderer* &renderer,
		gameParameters &gParam,
		const std::string& message,
		int font_size,
		int x,
		int y,
		bool center=true,
		bool select=false)
{
	SDL_Rect offset;
	TTF_Font *font = NULL;
	std::pair<SDL_Surface*,SDL_Texture*> messageT;
	messageT.first = NULL;
	messageT.second = NULL;
#ifndef WIN32
	load_font( font, "../assets/high_school_USA_sans.ttf", font_size );
#else
	load_font( font, "high_school_USA_sans.ttf", font_size );
#endif

	if( font != NULL )
	{
		messageT.first = TTF_RenderText_Solid( font, message.c_str(), gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::text]);
		if( messageT.first != NULL )
		{
			messageT.second = SDL_CreateTextureFromSurface(renderer, messageT.first);
		}
		free_font(font);
	}

	offset.y = y;
	if(center)
	{
		offset.x = x - messageT.first->w/2;
	}
	else
	{
		offset.x = x;
	}
	offset.w = messageT.first->w;
	offset.h = messageT.first->h;
	SDL_RenderCopy( renderer, messageT.second, NULL, &offset);

	if( select )
	{
		SDL_SetRenderDrawColor( renderer, 
				gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::fg].r, 
				gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::fg].g, 
				gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::fg].b, 
				gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::fg].a );
		offset.w = 5;
		offset.x = x - (messageT.first->w/2) - 5;
		SDL_RenderFillRect( renderer, &offset );
		offset.x = x + (messageT.first->w/2) + 5;
		SDL_RenderFillRect( renderer, &offset );
	}

	free_surface(messageT.first);
	free_texture(messageT.second);

	return y + offset.h;
}

void draw_background( SDL_Renderer* &renderer,
		gameParameters &gParam,
		bool init = false)
{
	static std::vector< SDL_Rect > fg;
	static std::vector< SDL_Rect > bdy;
	if(init == false)
	{
		SDL_SetRenderDrawColor( renderer, 
				gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::bg].r, 
				gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::bg].g, 
				gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::bg].b, 
				gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::bg].a );
		SDL_RenderClear( renderer );

		SDL_SetRenderDrawColor( renderer, 
				gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::fg].r, 
				gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::fg].g, 
				gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::fg].b, 
				gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::fg].a );
		SDL_RenderFillRects( renderer, fg.data(), fg.size());

		SDL_SetRenderDrawColor( renderer, 
				gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::bdy].r, 
				gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::bdy].g, 
				gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::bdy].b, 
				gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::bdy].a );
		SDL_RenderFillRects( renderer, bdy.data(), bdy.size());
	}
	else
	{
		fg.push_back( { 50,  20, 540, 10} );
		fg.push_back( { 50, 450, 540, 10} );
		fg.push_back( {   0,   0, 640,   2} );
		fg.push_back( {   0,   0,   2, 480} );
		fg.push_back( {   0, 478, 640,   2} );
		fg.push_back( { 638,   0,   2, 480} );
		for(int i=30;i<450;i+=40)
		{
			fg.push_back({310, i, 20, 20});
		}

		bdy.push_back({50,30,1,420});
		bdy.push_back({590,30,1,420});
	}

}


std::string get_time(int time,
		int length=1)
{
	std::string str, str_pad;
	str = std::to_string( time );
	for(unsigned int i=0;i<length-str.size();i++)
	{
		str_pad.push_back('0');
	}
	str = str_pad + str;
	return str;
}

int game(SDL_Renderer* &renderer,
		gameParameters &gParam) 
{
	Uint32 t1,t2;
	Uint32 time1,time2;
	std::string message;
	int game_status = 1;
	int return_status = 1;
	SDL_Event event; 

	static Ball ball;
	std::vector< Player > player(2);;
	player[0].rect = {50,  210, 10, 60};
	player[1].rect = {580, 210, 10, 60};

	t1 = SDL_GetTicks();
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	draw_background(renderer, gParam, true);

	ball.reset();

	time1 = SDL_GetTicks();
	time2 = time1+1;
	while( time2-time1 < 3000 && game_status == 1 )
	{
		//While there's an event to handle
		while( SDL_PollEvent( &event ) ) 
		{

			//If the user has Xed out the window
			if( SDL_QUIT == event.type )
			{
				//Quit the program
				game_status = 3;
				return_status = -1;
			}
		}

		if( state[SDL_SCANCODE_ESCAPE] )
		{
			game_status = 3;
			return_status = 0;
		}

		draw_background(renderer, gParam);
		ball.draw(renderer, gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::ball]);
		player[0].draw(renderer, gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::paddle_p1]);
		player[1].draw(renderer, gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::paddle_p2]);

		message = get_time(3-(time2-time1)/1000);
		display_message(renderer, gParam, message, 160, 320, 50);
		
		SDL_RenderPresent( renderer );
		t2 = SDL_GetTicks();
		if( (t2 - t1) < 25 )
		{
			SDL_Delay( 25 - (t2-t1) );
		}
		t1 = SDL_GetTicks();
		time2 = SDL_GetTicks();
	}

	time1 = SDL_GetTicks();
	time2 = time1+1;
	while( game_status == 1 )
	{
		//While there's an event to handle
		while( SDL_PollEvent( &event ) ) 
		{

			//If the user has Xed out the window
			if( SDL_QUIT == event.type )
			{
				//Quit the program
				game_status = 3;
				return_status = -1;
			}
		}

		if( state[SDL_SCANCODE_J] ||  state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])
		{
			player[0].up();
		}
		if( state[SDL_SCANCODE_K] ||  state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])
		{
			player[0].down();
		}
			
		player[1].AIMove(ball);
		if( state[SDL_SCANCODE_ESCAPE] )
		{
			game_status = 3;
			return_status = 0;
		}

		if( ball.update(player) >= 0 )
		{
			game_status = 2;
		}

		draw_background(renderer, gParam);
		ball.draw(renderer, gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::ball]);
		player[0].draw(renderer, gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::paddle_p1]);
		player[1].draw(renderer, gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::paddle_p2]);

		message = get_time((time2-time1)/1000,3);
		display_message( renderer, gParam, message, 40, 340, 40, false);
		
		SDL_RenderPresent( renderer );
		t2 = SDL_GetTicks();
		time2 = SDL_GetTicks();
		if( (t2 - t1) < 25 )
		{
			SDL_Delay( 25 - (t2-t1) );
		}
		t1 = SDL_GetTicks();
	}

	while( game_status == 2 )
	{
		//While there's an event to handle
		while( SDL_PollEvent( &event ) ) 
		{

			//If the user has Xed out the window
			if( SDL_QUIT == event.type )
			{
				//Quit the program
				return_status = -1;
				game_status=3;
			}
		}

		if( state[SDL_SCANCODE_RIGHT] )
		{
			if(return_status > 0)
			{
				return_status--;
			}
		}
		if( state[SDL_SCANCODE_LEFT] )
		{
			if(return_status < 1)
			{
				return_status++;
			}
		}
		if( state[SDL_SCANCODE_RETURN] )
		{
			game_status = 3;
		}
		if( state[SDL_SCANCODE_ESCAPE] )
		{
			return_status = 0;
			game_status = 3;
		}

		draw_background(renderer, gParam);
		ball.draw(renderer, gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::ball]);
		player[0].draw(renderer, gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::paddle_p1]);
		player[1].draw(renderer, gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::paddle_p2]);

		//draw message box
		{
			SDL_Rect box;
			box.x = 160;
			box.y = 120;
			box.w = 320;
			box.h = 240;
			SDL_SetRenderDrawColor( renderer, 
					gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::bg].r, 
					gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::bg].g, 
					gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::bg].b, 
					gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::bg].a );
			SDL_RenderFillRect( renderer, &box );

			SDL_SetRenderDrawColor( renderer, 
					gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::fg].r, 
					gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::fg].g, 
					gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::fg].b, 
					gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::fg].a );
			for(int i=0;i<10;i++)
			{
				SDL_RenderDrawRect( renderer, &box );
				box.x = box.x+1;
				box.y = box.y+1;
				box.w = box.w-2;
				box.h = box.h-2;
			}
		}

		//write out messgae
		int y = 130;
		message = "Final Time";
		y = display_message(renderer, gParam, message, 40, 320, y);

		message = get_time((time2-time1)/1000,3);
		y = display_message(renderer, gParam, message, 40, 320, y);

		message = "Play again?";
		y = display_message(renderer, gParam, message, 40, 320, y);

		message = "Yes";
		display_message(renderer, gParam, message, 40, 240, y, true, return_status==1);

		message = "No";
		display_message(renderer, gParam, message, 40, 400, y, true, return_status==0);

		SDL_RenderPresent( renderer );
		t2 = SDL_GetTicks();
		if( (t2 - t1) < 25 )
		{
			SDL_Delay( 25 - (t2-t1) );
		}
		t1 = SDL_GetTicks();
	}

	return return_status;
}
