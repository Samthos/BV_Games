#include "game_params.h"
#include "pugixml.hpp"
#include "config.h"

#include <iostream>
#include <string>

void defaultColors(gameParameters &gParam)
{
	gParam.colors.resize( Color::numColor );
	gParam.colors[Color::bg] = bgDefault;
	gParam.colors[Color::fg] = fgDefault;
	gParam.colors[Color::bdy] = bdyDefault;
	gParam.colors[Color::text] = textDefault;
	gParam.colors[Color::ball] = ballDefault;
	gParam.colors[Color::paddle_p1] = paddle_p1Default;
	gParam.colors[Color::paddle_p2] = paddle_p2Default;
}

SDL_Color loadSDL_Color(const pugi::xml_node colorTypeNode)
{
	SDL_Color tempColor;
	for(pugi::xml_node component = colorTypeNode.first_child(); component; component = component.next_sibling()) 
	{
#ifndef NDEBUG
		std::cout << "\t\t" << component.name() << "\t" << component.first_child().value() << std::endl;
#endif

		if( static_cast<std::string>(component.name()) == "r" )
		{
			tempColor.r = std::stoi( component.first_child().value() );
		}
		else if( static_cast<std::string>(component.name()) == "g" )
		{
			tempColor.g = std::stoi( component.first_child().value() );
		}
		else if( static_cast<std::string>(component.name()) == "b" )
		{
			tempColor.b = std::stoi( component.first_child().value() );
		}
		else if( static_cast<std::string>(component.name()) == "a" )
		{
			tempColor.a = std::stoi( component.first_child().value() );
		}
		else
		{
			std::cerr << "component " << static_cast<std::string>(component.name()) << " not recognized" << std::endl;
		}
	}
	return tempColor;
}

void prevColorScheme(gameParameters &gParam)
{
	gParam.currentColorScheme -= 1;
	if( gParam.currentColorScheme < 0 )
	{
		gParam.currentColorScheme = gParam.colorSchemeList.size() - 1;
	}
}

void nextColorScheme(gameParameters &gParam)
{
	gParam.currentColorScheme += 1;
	gParam.currentColorScheme %= gParam.colorSchemeList.size();
}

void loadColors(gameParameters &gParam)
{
	//used to hold loaded colors
	SDL_Color tempColor;
	std::string colorType;
	std::string oldColorScheme = "a";
	Color::Color_Scheme tempColorScheme;

	std::vector<bool> colorLoaded(Color::numColor);

	pugi::xml_document doc;

	if( gParam.colorSchemeList.size() > 0 )
	{
		oldColorScheme = gParam.colorSchemeList[gParam.currentColorScheme].name;
	}
	gParam.colorSchemeList.clear();
	
	//load default
	{
		tempColorScheme.name = "default";
		tempColorScheme.colors.resize( Color::numColor );
		tempColorScheme.colors[Color::bg] = bgDefault;
		tempColorScheme.colors[Color::fg] = fgDefault;
		tempColorScheme.colors[Color::bdy] = bdyDefault;
		tempColorScheme.colors[Color::text] = textDefault;
		tempColorScheme.colors[Color::ball] = ballDefault;
		tempColorScheme.colors[Color::paddle_p1] = paddle_p1Default;
		tempColorScheme.colors[Color::paddle_p2] = paddle_p2Default;
		gParam.colorSchemeList.push_back(tempColorScheme);
	}

	//open
#ifndef WIN32
    	if(doc.load_file("../config/color.xml") == -1)
#else
    	if(doc.load_file("color.xml") == -1)
#endif
	{
#ifndef WIN32
		std::cerr << "file ../config/color.xml does not exist" << std::endl;
#else
		std::cerr << "file color.xml does not exist" << std::endl;
#endif
	}
	else
	{
		//traverse list of color scheme
		for(pugi::xml_node colorSchemeNode = doc.first_child(); colorSchemeNode; colorSchemeNode = colorSchemeNode.next_sibling()) 
		{
			if( colorSchemeNode.attribute("title") )
			{
				tempColorScheme.name = colorSchemeNode.attribute("title").value();
#ifndef NDEBUG

				std::cout << colorSchemeNode.name() << "\t" << tempColorScheme.name << std::endl;
#endif

				//fill array to check if all color types are loaded with false
				std::fill(colorLoaded.begin(), colorLoaded.end(), false);

				//traverse list of colors types
				for(pugi::xml_node colorTypeNode = colorSchemeNode.first_child(); colorTypeNode; colorTypeNode = colorTypeNode.next_sibling())
				{
					//get color type 
					if( colorTypeNode.attribute("title") )
					{
						colorType = colorTypeNode.attribute("title").value();

#ifndef NDEBUG
						std::cout << "\t" << colorTypeNode.name() << "\t" << colorType << std::endl;
#endif
						//load color
						tempColor = loadSDL_Color( colorTypeNode );

						//set color
						for(auto it = Color::Color_String.begin(); it != Color::Color_String.end(); it++)
						{
							if( colorType == *it )
							{
								tempColorScheme.colors[ std::distance(Color::Color_String.begin(), it) ] = tempColor;
								colorLoaded[ std::distance(Color::Color_String.begin(), it) ] = true;
								break;
							}
						}
					}
				}

				bool allLoaded = true;
				for(auto it : colorLoaded)
				{
					allLoaded = (allLoaded && it);
				}
				if(allLoaded)
				{
					gParam.colorSchemeList.push_back(tempColorScheme);
				}
			}
		}
	}

	gParam.currentColorScheme = 0;
	for(auto it = gParam.colorSchemeList.begin(); it != gParam.colorSchemeList.end(); it++)
	{
		if( it->name == oldColorScheme )
		{
			gParam.currentColorScheme = std::distance(gParam.colorSchemeList.begin(), it);
		}
	}
}
