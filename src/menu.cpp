#include <sstream>
#include <iostream>

#include "menu.h"
#include "sdl_help.h"

MenuObject::MenuObject()
{
	textRendered = false;
	titleSurfaceLarge = NULL;
	titleSurfaceSmall = NULL;
	titleTextureLarge = NULL;
	titleTextureSmall = NULL;
	smallFontSize = -1;
	largeFontSize = -1;
}

MenuObject::~MenuObject()
{
	free_texture(titleTextureLarge);
	free_texture(titleTextureSmall);
	free_surface(titleSurfaceLarge);
	free_surface(titleSurfaceSmall);
}

MenuObject::MenuObject( std::string filename )
{

	std::ifstream input;
	pugi::xml_document doc;

	//run defualt constructor
	textRendered = false;
	titleSurfaceLarge = NULL;
	titleSurfaceSmall = NULL;
	titleTextureLarge = NULL;
	titleTextureSmall = NULL;
	smallFontSize = -1;
	largeFontSize = -1;
	
    	//if(doc.load_file(filename.c_str()))
    	if(doc.load_file("menu.xml") != -1)
	{
	    	pugi::xml_node node = doc.first_child();
		readXML(node);
	}
	else
	{
		printf("failed to open XML doc\n");
	}
}

void MenuObject::renderText(SDL_Renderer*& renderer)
{
	int i;
	TTF_Font *font = NULL;
	SDL_Color color={0,255,0};

	textRendered = true;
	if( largeFontSize != -1 )
	{

		load_font( font, "assets/high_school_USA_sans.ttf", largeFontSize );
		titleSurfaceLarge = TTF_RenderText_Solid( font, title.c_str(), color );
		titleTextureLarge = SDL_CreateTextureFromSurface(renderer, titleSurfaceLarge);
		free_font( font );
	}
	if( smallFontSize != -1 )
	{
		load_font( font, "assets/high_school_USA_sans.ttf", smallFontSize );
		titleSurfaceSmall = TTF_RenderText_Solid( font, title.c_str(), color );
		titleTextureSmall = SDL_CreateTextureFromSurface(renderer, titleSurfaceSmall);
		free_font( font );
	}
	for(i=0;i<objects.size();i++)
	{
		objects[i].renderText( renderer );
	}
}

void MenuObject::readXML( pugi::xml_node &in )
{
	int i, numNewObjects, numOldObjects;
	std::string str;
	//read attributes

	for(pugi::xml_attribute attr = in.first_attribute(); attr; attr = attr.next_attribute())
	{
		str = attr.name();
		if( str.compare("title") == 0 )
		{
			title = attr.value();
		}
		else if( str.compare("type") == 0 )
		{
			type  = static_cast<menuObjectType>(attr.as_int());
		}
		else if( str.compare("largeFont") == 0 )
		{
			largeFontSize = attr.as_int();
		}
		else if( str.compare("smallFont") == 0 )
		{
			smallFontSize = attr.as_int();
		}
	}

	numNewObjects = 0;
	numOldObjects = objects.size();;
	for(pugi::xml_node node = in.first_child(); node; node = node.next_sibling())
	{
		numNewObjects++;
	}

	i=0;
	objects.resize( numOldObjects + numNewObjects );
	for(pugi::xml_node node = in.first_child(); node; node = node.next_sibling())
	{
		objects[numOldObjects+i].readXML(node);
		i++;
	}
}

void MenuObject::print(int tab)
{
	for(auto i=0;i<tab;i++)
		std::cout << "\t";
	std::cout << title << "\t" << type << "\t";
	std::cout << largeFontSize << "\t" << smallFontSize << "\t" << objects.size() << std::endl;
	for(auto& it : objects )
	{
		it.print(tab+1);
	}
}

bool MenuObject::display(SDL_Window* &screen, SDL_Renderer* &renderer)
{
	Uint32 t1,t2;
	std::string title;
	int i,j;
	bool quit = false;
	bool leave = false;
	bool change = true;;

	SDL_Event event;
	SDL_Rect offset;
	std::vector< SDL_Rect > fg;
	fg.push_back( { 50,  20, 540, 10} );
	fg.push_back( { 50, 450, 540, 10} );
	fg.push_back( {   0,   0, 640,   2} );
	fg.push_back( {   0,   0,   2, 480} );
	fg.push_back( {   0, 478, 640,   2} );
	fg.push_back( { 638,   0,   2, 480} );

	SDL_Color bgColor = { 0, 0, 0, 255};
	SDL_Color fgColor = {255, 255, 255, 255};

	if( textRendered == false )
	{
		renderText(renderer);
	}

	stateInit();

	t1 = SDL_GetTicks();
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
			if( event.type == SDL_KEYDOWN )
			{
				switch( event.key.keysym.sym )
				{
					case SDLK_k:
						stateUp();
						change = true;
						break;
					case SDLK_UP:
						stateUp();
						change = true;
						break;
					case SDLK_j:
						stateDown();
						change = true;
						break;
					case SDLK_DOWN:
						stateDown();
						change = true;
						break;
					case SDLK_RETURN:
						if( objects[state].type == PLAY )
						{
							quit = game(screen,renderer);
							change = true;
						}
						else if( objects[state].type == MENU )
						{
							quit = objects[state].display(screen, renderer);
							change = true;
						}
						else if( objects[state].type == UP )
						{
							leave = true;
						}
						else if( objects[state].type == QUIT )
						{
							quit = true;
						}
						break;
				}
			}
		}
		if(change && !quit && !leave)
		{
			change = false;

			SDL_SetRenderDrawColor( renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a );
			SDL_RenderClear( renderer );
			SDL_SetRenderDrawColor( renderer, fgColor.r, fgColor.g, fgColor.b, fgColor.a );
			SDL_RenderFillRects( renderer, fg.data(), fg.size());

			offset.x = (640 - titleSurfaceLarge->w)/2;
			offset.y = 30;
			offset.w = titleSurfaceLarge->w;
			offset.h = titleSurfaceLarge->h;
			SDL_RenderCopy( renderer, titleTextureLarge, NULL, &offset);

			offset.y = 200;
			for(i=0;i<objects.size();i++)
			{
				offset.x = (640 - objects[i].titleSurfaceSmall->w)/2;
				offset.w = objects[i].titleSurfaceSmall->w;
				offset.h = objects[i].titleSurfaceSmall->h;
				SDL_RenderCopy( renderer, objects[i].titleTextureSmall, NULL, &offset);
				if(i == state)
				{
					SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255 );
					for(j=0;j<2;j++)
					{
						int shift = (j==0?-10 : objects[i].titleSurfaceSmall->w + 5);
						SDL_Rect paddle = { offset.x + shift, offset.y , 5, objects[i].titleSurfaceSmall->h };
						SDL_RenderFillRect( renderer, &paddle );
					}
				}
				offset.y += offset.h;
			}
			SDL_RenderPresent( renderer );
		}
		t2 = SDL_GetTicks();
		if( (t2 - t1) < 25 )
		{
			SDL_Delay( 25 - (t2-t1) );
		}
		t1 = SDL_GetTicks();
	}

	return quit;
}

void MenuObject::stateInit()
{
	state = 0;
	while( objects[state].type == TEXT )
	{
		state++;
	}
}

void MenuObject::stateUp()
{
	int newState = state-1;
	while( newState >= 0 && objects[newState].type == TEXT )
	{
		newState--;
	}
	if(newState >= 0)
	{
		state = newState;
	}
}

void MenuObject::stateDown()
{
	int newState = state+1;
	while( newState < objects.size() && objects[newState].type == TEXT )
	{
		newState++;
	}
	if(newState < objects.size())
	{
		state = newState;
	}
}

