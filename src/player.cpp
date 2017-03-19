#include <iostream>
#include <sstream>
#include <cmath>

#include "player.h"
#include "ball.h"

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
	return *this;
}

void Player::renderScore(SDL_Renderer*& renderer)
{
	TTF_Font *font = NULL;
	SDL_Color color={0,255,0};

	std::string str = "01";
	char temp[256];
	sprintf(temp,"%02i",score);

	
	free_texture(scoreTexture);
	free_surface(scoreSurface);

	load_font( font, "../assets/high_school_USA_sans.ttf", 40);
	scoreSurface = TTF_RenderText_Solid( font, temp, color );
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
	free_font( font );

	scoreRect.w = scoreSurface->w;
	scoreRect.h = scoreSurface->h;
	scoreRect.x = 320+(1-id)*(-scoreRect.w-40) + id*(40);
	scoreRect.y = 40;
}

void Player::AIMove(Ball &ball)
{
	double t1,t2;
	double x,y;
	double vx,vy;

	//if ball is moving toward left size of screen
	//try to move
	if( ball.xVel > 0 )
	{
		x  = ball.coordinate.x;
		y  = ball.coordinate.y;
		vx = ball.xVel;
		vy = ball.yVel;

		do
		{
			if( vy < 0 ) //ball is moving toward top of the screeen
			{
				t1 = (30 - y)/vy;
			}
			else
			{
				t1 = (450 - (y + ball.coordinate.h))/vy;
			}

			t2 = ( 580 - (x + ball.coordinate.w))/vx;

			if( t1 < t2 )
			{
				x  += vx * t1;
				y  += vy * t1;
				vy *= -1;
			}
		} while( t1 < t2 );

		y += vy * t2;

		if( std::abs( y+ball.coordinate.h/2 - (rect.y+rect.h/2) ) > 10 )
		{
			if( y+ball.coordinate.h/2 < rect.y+rect.h/2 )
			{
				up();
			}
			else
			{
				down();
			}
		}
	}
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
