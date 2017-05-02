#include "ball.h" 
#include <cmath>
#include <cstdio>
#include <random>

Ball::Ball()
{
	reset();
}

void Ball::reset()
{
	static std::mt19937 g1;
	std::uniform_real_distribution<double> xVelInit (1,2);
	std::uniform_real_distribution<double> yVelInit (1,2);
	std::uniform_int_distribution<int> signVelInit (0,1);

	coordinate = {310, 230, 20, 20};

	xVel = xVelInit(g1);
	yVel = yVelInit(g1) * (signVelInit(g1)==0?-1:1);
}

int Ball::update(std::vector<Player> &player)
{
	coordinate.x += xVel;
	coordinate.y += yVel;

	//see if ball hit the edge of a paddle
	for( auto& it : player )
	{
		SDL_Rect inzone = { it.rect.x, 30, it.rect.w, 420 };
		if( SDL_HasIntersection( &coordinate, &it.rect ) )
		{
			if( xVel < 0 )
			{
				xVel *= -1.0;
				xVel += (xVel>0?0.1:-0.1);
				int temp = it.rect.x+it.rect.w;
				temp *= 2;
				coordinate.x = temp-coordinate.x;
			}
			else if( xVel > 0 )
			{
				xVel *= -1.0;
				xVel += (xVel>0?0.1:-0.1);
				int temp = it.rect.x;
				temp *= 2;
				coordinate.x = temp-coordinate.x-2*coordinate.w;
			}
		}
		else if( SDL_HasIntersection( &coordinate, &inzone ) )
		{
			return &it - &player[0];
		}
	}


	if( coordinate.y < 30 )
	{
		yVel *= -1;
		coordinate.y = 60-coordinate.y;
	}
	else if( (coordinate.y+coordinate.h) > 450)
	{
		yVel *= -1;
		coordinate.y = 2*450 - (coordinate.y+coordinate.h)-coordinate.h;
	}
	return -1;
}

void Ball::draw(SDL_Renderer* &renderer, const SDL_Color& color)
{
		SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
		SDL_RenderFillRect( renderer, &coordinate);
}
