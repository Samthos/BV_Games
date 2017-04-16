#include <sstream>
#include <iostream>

#include "menu.h"
#include "sdl_help.h"

MenuObject::MenuObject()
{
	smallFontSize = -1;
	largeFontSize = -1;
}

MenuObject::~MenuObject()
{
}

MenuObject::MenuObject(const std::string filename )
{

	std::ifstream input;
	pugi::xml_document doc;

	//run defualt constructor
	smallFontSize = -1;
	largeFontSize = -1;
	
    	//if(doc.load_file(filename.c_str()))
    	if(doc.load_file("../config/menu.xml") != -1)
	{
	    	pugi::xml_node node = doc.first_child();
		readXML(node);
	}
	else
	{
		printf("failed to open XML doc\n");
	}
}

void MenuObject::readXML(const pugi::xml_node &in)
{
	int i, numNewObjects;
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

	//count number of children
	numNewObjects = 0;
	for(pugi::xml_node node = in.first_child(); node; node = node.next_sibling())
	{
		numNewObjects++;
	}

	i=0;
	objects.resize( numNewObjects );
	for(pugi::xml_node node = in.first_child(); node; node = node.next_sibling())
	{
		objects[i].readXML(node);
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

int MenuObject::display(SDL_Window* &screen, SDL_Renderer* &renderer, gameParameters& gParams)
{
	Uint32 t1,t2;
	unsigned int i,j;
	int status = 0;
	bool leave = false;

	SDL_Event event;
	SDL_Rect offset;
	std::vector< SDL_Rect > fg;
	{
		fg.push_back( { 50,  20, 540, 10} );
		fg.push_back( { 50, 450, 540, 10} );
		fg.push_back( {   0,   0, 640,   2} );
		fg.push_back( {   0,   0,   2, 480} );
		fg.push_back( {   0, 478, 640,   2} );
		fg.push_back( { 638,   0,   2, 480} );
	}

	TTF_Font *font = NULL;
	std::pair<SDL_Surface*,SDL_Texture*> menu_title;
	std::vector< std::pair<SDL_Surface*,SDL_Texture*> > menu_items(objects.size());

	//render title message
	{
		menu_title.first = NULL;
		menu_title.second = NULL;
		load_font( font, "../assets/high_school_USA_sans.ttf", largeFontSize );
		if( font != NULL )
		{
			menu_title.first = TTF_RenderText_Solid( font, title.c_str(), gParams.textColor);
			if( menu_title.first != NULL )
			{
				menu_title.second = SDL_CreateTextureFromSurface(renderer, menu_title.first);
				if(menu_title.second == NULL)
				{
					std::cout << "texture failed" << std::endl;
				}
			}
			else
			{
				std::cout << "no font" << std::endl;
			}
			free_font(font);
		}
	}

	//render menu 
	for(i = 0; i < objects.size(); i++)
	{
		menu_items[i].first = NULL;
		menu_items[i].second = NULL;

		load_font( font, "../assets/high_school_USA_sans.ttf", objects[i].smallFontSize );
		if( font != NULL )
		{
			menu_items[i].first = TTF_RenderText_Solid( font, objects[i].title.c_str(), gParams.textColor);
			if( menu_items[i].first != NULL )
			{
				menu_items[i].second = SDL_CreateTextureFromSurface(renderer, menu_items[i].first);
				if(menu_items[i].second == NULL)
				{
					std::cout << "texture failed" << std::endl;
				}
			}
			else
			{
				std::cout << "no surface" << std::endl;
			}
			free_font(font);
		}
		else
		{
			std::cout << "no font" << std::endl;
		}
	}

	stateInit();

	t1 = SDL_GetTicks();
	while( status == 0 && leave == false )
	{
		//While there's an event to handle
		while( SDL_PollEvent( &event ) ) 
		{
			//If the user has Xed out the window
			if( SDL_QUIT == event.type )
			{
				//Quit the program
				status = -1;
			}
			if( event.type == SDL_KEYDOWN )
			{
				//commands to navigate menu
				switch( event.key.keysym.sym )
				{
					case SDLK_k:
						stateUp();
						break;
					case SDLK_UP:
						stateUp();
						break;
					case SDLK_j:
						stateDown();
						break;
					case SDLK_DOWN:
						stateDown();
						break;
					case SDLK_RIGHT:
						//stateDown();
						break;
					case SDLK_LEFT:
						//stateDown();
						break;
					case SDLK_RETURN:
						if( objects[state].type == PLAY )
						{
							status = 1;
						}
						else if( objects[state].type == MENU )
						{
							status = objects[state].display(screen, renderer, gParams);
						}
						else if( objects[state].type == UP )
						{
							leave = true;
						}
						else if( objects[state].type == QUIT )
						{
							status = -1;
						}
						break;
				}
			}
		}

		//clear window
		SDL_SetRenderDrawColor( renderer, gParams.bgColor.r, gParams.bgColor.g, gParams.bgColor.b, gParams.bgColor.a );
		SDL_RenderClear( renderer );

		//draw screen foreground
		SDL_SetRenderDrawColor( renderer, gParams.fgColor.r, gParams.fgColor.g, gParams.fgColor.b, gParams.fgColor.a );
		SDL_RenderFillRects( renderer, fg.data(), fg.size());

		//draw menu title
		offset.x = (640 - menu_title.first->w)/2;
		offset.y = 30;
		offset.w = menu_title.first->w;
		offset.h = menu_title.first->h;
		SDL_RenderCopy( renderer, menu_title.second, NULL, &offset);

		//draw menu options
		offset.y = 200;
		for(i=0;i<objects.size();i++)
		{
			offset.x = (640 - menu_items[i].first->w)/2;
			offset.w = menu_items[i].first->w;
			offset.h = menu_items[i].first->h;
			SDL_RenderCopy( renderer, menu_items[i].second, NULL, &offset);
			if(i == static_cast<unsigned int>(state))
			{
				SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255 );
				for(j=0;j<2;j++)
				{
					int shift = (j==0?-10 : menu_items[i].first->w + 5);
					SDL_Rect paddle = { offset.x + shift, offset.y , 5, menu_items[i].first->h };
					SDL_RenderFillRect( renderer, &paddle );
				}
			}
			offset.y += offset.h;
		}
		SDL_RenderPresent( renderer );

		//frame rate limiters
		t2 = SDL_GetTicks();
		if( (t2 - t1) < 25 )
		{
			SDL_Delay( 25 - (t2-t1) );
		}
		t1 = SDL_GetTicks();
	}

	free_surface(menu_title.first);
	free_texture(menu_title.second);
	for(auto& it : menu_items)
	{
		free_surface( it.first );
		free_texture( it.second );
	}

	return status;
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
	while( newState < static_cast<int>(objects.size()) && objects[newState].type == TEXT )
	{
		newState++;
	}
	if(newState < static_cast<int>(objects.size()))
	{
		state = newState;
	}
}

