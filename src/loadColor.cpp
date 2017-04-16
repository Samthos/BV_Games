#include "game_params.h"
#include "pugixml.hpp"
#include <iostream>
#include <string>

//void loadColors(gameParameters &gParam, int colorID)
void loadColors(gameParameters &gParam)
{
	SDL_Color temp_color;
	pugi::xml_document doc;
    	if(doc.load_file("../config/color.xml") == -1)
	{
	}

	for(pugi::xml_node nodeA = doc.first_child(); nodeA; nodeA = nodeA.next_sibling()) //traverse list of color scheme
	{
		std::cout << nodeA.name() << std::endl;
		for(pugi::xml_attribute attr = nodeA.first_attribute(); attr; attr = attr.next_attribute())
		{
			std::cout << attr.name() << "\t" << attr.value() << std::endl;
		}

		for(pugi::xml_node nodeB = nodeA.first_child(); nodeB; nodeB = nodeB.next_sibling()) //traverse list of colors
		{
			std::cout << "\t" << nodeB.name() << std::endl;
			std::string color_name;
			for(pugi::xml_attribute attr = nodeB.first_attribute(); attr; attr = attr.next_attribute())
			{
				std::string attr_name = attr.name();
				std::string attr_value = attr.value();
				
				if( attr_name.compare("title") == 0 )
				{
					std::cout << "\t" << attr_name << "\t" << attr_value << std::endl;
					color_name = attr_value;
				}
			}

			//load components of color
			for(pugi::xml_node nodeC = nodeB.first_child(); nodeC; nodeC = nodeC.next_sibling()) //traverse list of colors values
			{
				std::string component_name = nodeC.name();
				std::string value = nodeC.first_child().value();

				std::cout << "\t\t" << component_name << "\t" << value << std::endl;

				if( component_name.compare("red") == 0 )
				{
					temp_color.r = std::stoi( value );
				}
				else if( component_name.compare("green") == 0 )
				{
					temp_color.g = std::stoi( value );
				}
				else if( component_name.compare("blue") == 0 )
				{
					temp_color.b = std::stoi( value );
				}
				else if( component_name.compare("alpha") == 0 )
				{
					temp_color.a = std::stoi( value );
				}
				else
				{
					std::cerr << "component " << component_name << " not recognized" << std::endl;
				}
			}

			//set color
			if( color_name.compare("bgcolor") == 0 )
			{
				gParam.bgColor = temp_color;
			}
			else if( color_name.compare("fgcolor") == 0 )
			{
				gParam.fgColor = temp_color;
			}
			else if( color_name.compare("bdycolor") == 0 )
			{
				gParam.bdyColor = temp_color;
			}
			else if( color_name.compare("ballcolor") == 0 )
			{
				gParam.ballColor = temp_color;
			}
			else if( color_name.compare("paddle_p1_color") == 0 )
			{
				gParam.paddle_p1_Color = temp_color;
			}
			else if( color_name.compare("paddle_p2_color") == 0 )
			{
				gParam.paddle_p2_Color = temp_color;
			}
		}
	}
}
