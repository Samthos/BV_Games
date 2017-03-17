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
	std::uniform_real_distribution<double> xVelInit (4,5);
	std::uniform_real_distribution<double> yVelInit (1,5);
	std::uniform_int_distribution<int> signVelInit (0,1);

	coordinate = {310, 230, 20, 20};

	xVel = xVelInit(g1) * (signVelInit(g1)==0?-1:1);
	yVel = yVelInit(g1) * (signVelInit(g1)==0?-1:1);
	
	printf("%f %f\n",xVel,yVel);
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
				xVel *= -1.1;
				int temp = it.rect.x+it.rect.w;
				temp *= 2;
				coordinate.x = temp-coordinate.x;
			}
			else if( xVel > 0 )
			{
				xVel *= -1.1;
				int temp = it.rect.x;
				temp *= 2;
				coordinate.x = temp-coordinate.x-2*coordinate.w;
			}
		}
		else if( SDL_HasIntersection( &coordinate, &inzone ) )
		{
			return it.id;
		}
	}


	if( coordinate.y < 30 )
	{
		printf("%i\t",coordinate.y);
		yVel *= -1;
		coordinate.y = 60-coordinate.y;
		printf("%i\n",coordinate.y);
	}
	else if( (coordinate.y+coordinate.h) > 450)
	{
		printf("%i\n",coordinate.y+coordinate.h);
		yVel *= -1;
		coordinate.y = 2*450 - (coordinate.y+coordinate.h)-coordinate.h;
		printf("%i\n",coordinate.y+coordinate.h);
	}
	return -1;
}
