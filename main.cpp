/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"
#include "part.h"
#include "screen_params.h"
#include <string>

//Screen attributes
const int SCREEN_WIDTH = SCREEN_WIDTH_PARAM;
const int SCREEN_HEIGHT = SCREEN_HEIGHT_PARAM;
const int SCREEN_BPP = 32;
const int FONT_SIZE = 36; 

//consts for converting between real and screen
const int LEFT = 13;
const int RIGHT = 1011;
const int TOP = 13;
const int BOTTOM= 1011;
const double YMIN = YMIN_PARAM;
const double YMAX = YMAX_PARAM;
const double VYMIN = VYMIN_PARAM;
const double VYMAX = VYMAX_PARAM;

//The surfaces
SDL_Surface *pixel= NULL;
SDL_Surface *background = NULL;
SDL_Surface *mousePos = NULL;
SDL_Surface *eraser = NULL;
SDL_Surface *button = NULL;
SDL_Surface *screen = NULL;

//clip structure for pts
SDL_Rect clip;

//font and font color
TTF_Font *font = NULL;
SDL_Color textColor = { 255, 255, 255 };

//The event structure that will be used
SDL_Event event;

SDL_Surface *load_image( std::string filename ) {
	//The image that's loaded
	SDL_Surface* loadedImage = NULL;

	//The optimized image that will be used
	SDL_Surface* optimizedImage = NULL;

	//Load the image
	loadedImage = IMG_Load( filename.c_str() );

	//If the image loaded
	if( loadedImage != NULL ) {
		//Create an optimized image
		optimizedImage = SDL_DisplayFormat( loadedImage );

		//Free the old image
		SDL_FreeSurface( loadedImage );
	}

	//Return the optimized image
	return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL ) {
	//Holds offsets
	SDL_Rect offset;

	//Get offsets
	offset.x = x;
	offset.y = y;

	//Blit
	SDL_BlitSurface( source, clip, destination, &offset );
}

bool init() {
	//Initialize all SDL subsystems
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) {
		return false;
	}

	//Set up the screen
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

	//If there was an error in setting up the screen
	if( screen == NULL ) {
		return false;
	}

	//Initialize SDL_ttf
	if( TTF_Init() == -1 ) {
		return false;
	}

	//Set the window caption
	SDL_WM_SetCaption( "Surface of Section", NULL );

	//If everything initialized fine
	return true;
}

bool load_files() {
	//Load the image
	background = load_image( "assets/background.bmp" );
	eraser = load_image( "assets/eraser.bmp");
	button = load_image( "assets/button.bmp");
	pixel = load_image( "assets/pixel.bmp" );

	//load font
	font = TTF_OpenFont( "assets/ubuntu-r.ttf", FONT_SIZE );

	//If there was an error in loading the image
	if( pixel == NULL || background == NULL || font == NULL || eraser == NULL || button == NULL) {
		return false;
	}

	//If everything loaded fine
	return true;
}

void clean_up() {
	//Free the surface
	SDL_FreeSurface( background );
	SDL_FreeSurface( eraser );
	SDL_FreeSurface( button );
	SDL_FreeSurface( pixel );

	//Quit SDL
	SDL_Quit();
}

int main( int argc, char* args[] ) {
	//Make sure the program waits for a quit
	int i,x,y;
	double vy_dbl,y_dbl;
	bool quit = false;
	char message[256];
//	Part part(0.02,0.30751,0,0);//4
//	Part part(0.03,0.170822,0,0); //5
	Part part(0.05,0.283859,0,0);//10

	//Initialize
	if( init() == false ) {
		return 1;
	}

	//Load the files
	if( load_files() == false ) {
		return 1;
	}

	//Apply the surface to the screen/update screen 
	apply_surface( 0, 0, background, screen );
	sprintf(message,"CLEAR");
	mousePos = TTF_RenderText_Solid(font, message, textColor);
	apply_surface(1050,100-0.15*FONT_SIZE,button,screen);
	apply_surface(1080,100+0*FONT_SIZE,mousePos,screen);
	sprintf(message,"vy=");
	mousePos = TTF_RenderText_Solid(font, message, textColor);
	apply_surface(1050,100+1.5*FONT_SIZE,mousePos,screen);
	sprintf(message,"y=");
	mousePos = TTF_RenderText_Solid(font, message, textColor);
	apply_surface(1070,100+3.0*FONT_SIZE,mousePos,screen);
	if( SDL_Flip( screen ) == -1 ) {
		return 1;
	}
	clip.x = 0; clip.y = 0; clip.w = 1; clip.h = 1;

	//While the user hasn't quit
	while( quit == false ) {
		//While there's an event to handle
		while( SDL_PollEvent( &event ) ) {
			//If the user has Xed out the window
			if( event.type == SDL_QUIT ) {
				//Quit the program
				quit = true;
			}
			//Mouse motion
			if( event.type == SDL_MOUSEMOTION ) {
				//erase old position
				apply_surface(1050,100+1.5*FONT_SIZE,eraser,screen);

				//render new position
				x = event.motion.x;
				y = event.motion.y;
				if(x > LEFT && x < RIGHT && y > TOP && y < BOTTOM) {
					vy_dbl = (VYMIN-VYMAX) * ((double) (x - LEFT))/((double) (LEFT-RIGHT)) + VYMIN;
					y_dbl = (YMAX-YMIN) * ((double) (y - TOP))/((double) (TOP-BOTTOM)) + YMAX;

	//				sprintf(message,"%6.4f,%6.4f",vy_dbl,y_dbl);
					sprintf(message,"vy=%6.4f",vy_dbl);
					mousePos = TTF_RenderText_Solid(font, message, textColor);
					apply_surface(1050,100+1.5*FONT_SIZE,mousePos,screen);
					sprintf(message,"y=%6.4f",y_dbl);
					mousePos = TTF_RenderText_Solid(font, message, textColor);
					apply_surface(1070,100+3.0*FONT_SIZE,mousePos,screen);
				}
				else {
					sprintf(message,"vy=");
					mousePos = TTF_RenderText_Solid(font, message, textColor);
					apply_surface(1050,100+1.5*FONT_SIZE,mousePos,screen);
					sprintf(message,"y=");
					mousePos = TTF_RenderText_Solid(font, message, textColor);
					apply_surface(1070,100+3.0*FONT_SIZE,mousePos,screen);
				}
			}
			//start new simulation
			if( event.type == SDL_MOUSEBUTTONDOWN ) {
				if( event.button.button == SDL_BUTTON_LEFT) {
					x = event.motion.x;
					y = event.motion.y;

					if(x > LEFT && x < RIGHT && y > TOP && y < BOTTOM) {
						vy_dbl = (VYMIN-VYMAX) * ((double) (x - LEFT))/((double) (LEFT-RIGHT)) + VYMIN;
						y_dbl = (YMAX-YMIN) * ((double) (y - TOP))/((double) (TOP-BOTTOM)) + YMAX;

						//reset simulation
						if( part.reset(y_dbl,vy_dbl) ) {
							//switch color
							clip.x = clip.x+1;
							if(clip.x > 15) {
								clip.x = 0;
							//	apply_surface( 0, 0, background, screen );
							}
						}
					}
					else if(x > 1050 && x < 1225 && y > 100 -0.15*FONT_SIZE && y < 100+1.35*FONT_SIZE) {
						clip.x = 0;
						apply_surface( 0, 0, background, screen );
						sprintf(message,"CLEAR");
						apply_surface(1050,100-0.15*FONT_SIZE,button,screen);
						mousePos = TTF_RenderText_Solid(font, message, textColor);
						apply_surface(1080,100+0*FONT_SIZE,mousePos,screen);
						sprintf(message,"vy=");
						mousePos = TTF_RenderText_Solid(font, message, textColor);
						apply_surface(1050,100+1.5*FONT_SIZE,mousePos,screen);
						sprintf(message,"y=");
						mousePos = TTF_RenderText_Solid(font, message, textColor);
						apply_surface(1070,100+3.0*FONT_SIZE,mousePos,screen);
					}
				}
			}

		}

		//drives surface of section
		i=0;
		while(i<100) {
			part.update();
			if( part.check() ) {
				i++;
				x = ((double) (LEFT - RIGHT))*(part.vel(1) - VYMIN)/(VYMIN-VYMAX) + LEFT;
				y = ((double) (TOP - BOTTOM))*(part.pos(1) - YMAX)/(YMAX-YMIN) + TOP;
				apply_surface( x, y, pixel, screen, &clip);
			}
		}
		if( SDL_Flip( screen ) == -1 ) {
			return 1;
		}
	}

	//Free the surface and quit SDL
	clean_up();

	return 0;
}
