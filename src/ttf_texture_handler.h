#ifndef TTF_TEXTURE_HANDLER
#define TTF_TEXTURE_HANDLER

class TextureHandler
{
private:
	SDL_Surface* surface;
	SDL_Texture* texture;
	bool loadedSurface;
	bool loadedTexture;

public:
	TextureHandler();
	~TextureHandler();
	TextureHandler (const TextureHandler&);
	TextureHandler& operator= (const TextureHandler&);
	TextureHandler (TextureHandler&&);
	TextureHandler& operator= (TextureHandler&&);

	bool loadText();
	bool loadBMP();
	SDL_Texture* getPoints();
};
#endif

