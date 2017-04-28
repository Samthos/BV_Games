#include <iostream>
#include <sstream>
#include <cmath>

#include "player.h"
#include "ball.h"

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
void Player::draw(SDL_Renderer* &renderer, const SDL_Color& color)
{
		SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
		SDL_RenderFillRect( renderer, &rect);
}
