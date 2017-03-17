#include "player.h"
#include <iostream>
#include <sstream>

Player::Player()
{
	score = 0;
	scoreSurface = NULL;
	scoreTexture = NULL;
}
Player::~Player()
{
	free_texture(scoreTexture);
	free_surface(scoreSurface);
}
Player::Player (const Player& p1)
{
	rect = p1.rect;
	score = p1.score;
	scoreSurface = NULL;
	scoreTexture = NULL;
}
Player& Player::operator= (const Player& p1)
{
	rect = p1.rect;
	score = p1.score;
	scoreSurface = NULL;
	scoreTexture = NULL;
	return *this;
}
Player::Player (Player&& p1)
{
}
Player& Player::operator= (Player&& p1)
{
}


void Player::renderScore(SDL_Renderer*& renderer)
{
	int i;
	TTF_Font *font = NULL;
	SDL_Color color={0,255,0};

	std::string str = "01";
	char temp[256];
	sprintf(temp,"%02i",score);

	
	free_texture(scoreTexture);
	free_surface(scoreSurface);

	load_font( font, "assets/high_school_USA_sans.ttf", 40);
	scoreSurface = TTF_RenderText_Solid( font, temp, color );
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
	free_font( font );

	scoreRect.w = scoreSurface->w;
	scoreRect.h = scoreSurface->h;
	scoreRect.x = 320+(1-id)*(-scoreRect.w-40) + id*(40);
	scoreRect.y = 40;
}
void Player::up()
{
	if( rect.y > 30 )
		rect.y-=5;

	if( rect.y < 30 )
		rect.y = 30;
}
void Player::down()
{
	if( rect.y < 390 )
		rect.y+=5;

	if( rect.y > 390 )
		rect.y = 390;
}
