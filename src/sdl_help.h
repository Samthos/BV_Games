#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game_params.h"

bool init(SDL_Window* &screen, SDL_Renderer*&, gameParameters &);
void clean_up( SDL_Window* &screen);
bool load_font( TTF_Font* &in, std::string filename, int fontSize);
void free_font( TTF_Font* &in);
void free_surface( SDL_Surface* &surface);
void free_texture( SDL_Texture* &texture);
