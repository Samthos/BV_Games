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
    	if(doc.load_file(filename.c_str()) != -1)
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

int MenuObject::draw(SDL_Renderer*& renderer,
		SDL_Color& color,
	       	int x,
	       	int y,
	       	bool large,
	       	bool select)
{
	int fontSize;
	TTF_Font *font = NULL;
	SDL_Rect offset;
	std::pair<SDL_Surface*,SDL_Texture*> messageT;

	messageT.first = NULL;
	messageT.second = NULL;
	if(large)
	{
		fontSize = largeFontSize;
	}
	else
	{
		fontSize = smallFontSize;
	}
#ifndef WIN32
	load_font( font, "../assets/high_school_USA_sans.ttf", fontSize);
#else
	load_font( font, "high_school_USA_sans.ttf", FontSize);
#endif
	if( font != NULL )
	{
		messageT.first = TTF_RenderText_Solid( font, title.c_str(), color);
//		messageT.first = TTF_RenderText_Solid( font, title.c_str(), gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::text]);
		if( messageT.first != NULL )
		{
			messageT.second = SDL_CreateTextureFromSurface(renderer, messageT.first);
			if(messageT.second == NULL)
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
	
	offset.x = x - (messageT.first->w)/2;
	offset.y = y;
	offset.w = messageT.first->w;
	offset.h = messageT.first->h;
	SDL_RenderCopy( renderer, messageT.second, NULL, &offset);

	if( select )
	{
		SDL_SetRenderDrawColor( renderer,
				color.r,
				color.g,
				color.b,
				color.a );
		for(int j=0;j<2;j++)
		{
			int shift = (j==0?-10 : messageT.first->w + 5);
			SDL_Rect paddle = { offset.x + shift, offset.y , 5, messageT.first->h };
			SDL_RenderFillRect( renderer, &paddle );
		}
	}

	free_surface(messageT.first);
	free_texture(messageT.second);

	return y + offset.h;
}

int MenuObject::display(SDL_Window* &screen, SDL_Renderer* &renderer, gameParameters& gParam)
{
	Uint32 t1,t2;
	unsigned int i;
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
	std::pair<SDL_Surface*,SDL_Texture*> menu_option;

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
						if( objects[state].type == OPTION )
						{
							nextColorScheme(gParam);
						}
						break;
					case SDLK_LEFT:
						if( objects[state].type == OPTION )
						{
							prevColorScheme(gParam);
						}
						break;
					case SDLK_RETURN:
						if( objects[state].type == PLAY )
						{
							status = 1;
						}
						else if( objects[state].type == MENU )
						{
							status = objects[state].display(screen, renderer, gParam);
						}
						else if( objects[state].type == UP )
						{
							leave = true;
						}
						else if( objects[state].type == OPTION )
						{
							loadColors( gParam );
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
		SDL_SetRenderDrawColor( renderer,
			       	gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::bg].r,
			       	gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::bg].g,
			       	gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::bg].b,
				gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::bg].a );
		SDL_RenderClear( renderer );

		//draw screen foreground
		SDL_SetRenderDrawColor( renderer,
			       	gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::fg].r,
			       	gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::fg].g,
			       	gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::fg].b,
			       	gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::fg].a );
		SDL_RenderFillRects( renderer, fg.data(), fg.size());

		//draw menu title
		offset.y = draw( renderer, gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::text], 320, 30, true, false);

		//draw menu options
		offset.y = 200;
		for(i=0;i<objects.size();i++)
		{
			offset.y = objects[i].draw(renderer, gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::text], 320, offset.y, false, i == static_cast<unsigned int>(state));
		}

		if( objects[state].type == OPTION )
		{
			menu_option.first = NULL;
			menu_option.second = NULL;
#ifndef WIN32
			load_font( font, "../assets/high_school_USA_sans.ttf", objects[state].smallFontSize);
#else
			load_font( font, "high_school_USA_sans.ttf", objects[state].smallFontSize);
#endif
			if( font != NULL )
			{
				menu_option.first = TTF_RenderText_Solid( font, gParam.colorSchemeList[gParam.currentColorScheme].name.c_str(), gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::text]);
				if( menu_option.first != NULL )
				{
					menu_option.second = SDL_CreateTextureFromSurface(renderer, menu_option.first);
					if(menu_option.second == NULL)
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

			offset.y = 190;
			offset.x = (640 - menu_option.first->w)/2;
			offset.w = menu_option.first->w;
			offset.h = menu_option.first->h;
			SDL_RenderCopy( renderer, menu_option.second, NULL, &offset);
			free_surface(menu_option.first);
			free_texture(menu_option.second);

			SDL_SetRenderDrawColor( renderer,
					gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::text].r,
					gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::text].g,
					gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::text].b,
					gParam.colorSchemeList[gParam.currentColorScheme].colors[Color::text].a );
			std::vector<SDL_Point> arrow(3);
			for(i=0;i<8;i++)
			{
				arrow[0].x = offset.x - 5;
				arrow[0].y = offset.y+i;
				arrow[1].x = offset.x - 5 - offset.h/2+i;
				arrow[1].y = offset.y+offset.h/2;
				arrow[2].x = offset.x - 5;
				arrow[2].y = offset.y+offset.h-i-1;
				SDL_RenderDrawLines( renderer, arrow.data(), 3);

				arrow[0].x = offset.x + offset.w + 5;
				arrow[0].y = offset.y+i;
				arrow[1].x = offset.x + offset.w + 5 + offset.h/2-i;
				arrow[1].y = offset.y+offset.h/2;
				arrow[2].x = offset.x + offset.w + 5;
				arrow[2].y = offset.y+offset.h-i-1;
				SDL_RenderDrawLines( renderer, arrow.data(), 3);
			}

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

