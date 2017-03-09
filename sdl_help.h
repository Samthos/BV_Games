#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

bool init(SDL_Window* &screen);
void clean_up( SDL_Window* &screen);
bool load_font( TTF_Font* &in, std::string filename, int fontSize);
bool free_font( TTF_Font* &in);
bool load_image( SDL_Surface* &surface, SDL_PixelFormat *pxl, std::string filename);
void free_surface( SDL_Surface* &surface);
