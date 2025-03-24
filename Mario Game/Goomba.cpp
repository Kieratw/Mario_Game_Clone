#include "Goomba.h"
#include "Mario.h"
#include "Mapa.h"
#include "Global.h"
#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include <iostream>

   
Goomba::Goomba(const float i_x, const float i_y)
	:Enemy(i_x, i_y), GoombaAnimation("Resources/GoombaWalk.png", CELL_SIZE, 10), no_collision_dying(0), death_timer(GOOMBA_DEATH_DURATION)
{
	horizontal_speed = -GOOMBA_SPEED;
	GoombaTexture.loadFromFile("Resources/Goombadeath0.png");
	GoombaSprite.setTexture(GoombaTexture);
	x = i_x;
	y = i_y;
}
bool Goomba::getDead(const bool type) const
{
	if (1 == type)
	{
		return dead;
	}
	else
	{
		return dead || no_collision_dying || GOOMBA_DEATH_DURATION > death_timer;
	}
}


void Goomba::draw(const unsigned view_x, sf::RenderWindow& window)
{
	if (-CELL_SIZE < round(y) && round(x) > static_cast<int>(view_x) - SCREEN_WIDTH  && round(x) < SCREEN_WIDTH + view_x && round(y) < SCREEN_HEIGHT)
	{
		if (1 == no_collision_dying || GOOMBA_DEATH_DURATION > death_timer)
		{
			GoombaSprite.setPosition(round(x), round(y));
			GoombaSprite.setTexture(GoombaTexture);
			window.draw(GoombaSprite);
		}
		else
		{
			GoombaAnimation.setPosition(round(x), round(y));
			GoombaAnimation.draw(window);
		}
	}
}

void Goomba::update(const unsigned view_x, const std::vector<std::shared_ptr<Enemy>>& enemies, Mapa& mapa, Mario& mario)
{

	if (-CELL_SIZE < y && x >= static_cast<int>(view_x) - CELL_SIZE - ENTITY_UPDATE_AREA - SCREEN_WIDTH / 2 && x < ENTITY_UPDATE_AREA + SCREEN_WIDTH + view_x && y < SCREEN_HEIGHT)
	{
		
		std::vector<unsigned char> collision;

		sf::FloatRect hit_box = getHitbox();

		vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);

		hit_box.top += vertical_speed;

		collision = mapa.mapCollision({ Cell::Brick, Cell::Wall,Cell::Ground,Cell::Pipehighleft,Cell::Pipehighright,Cell::Pipelowleft,Cell::Pipelowright,Cell::QuestionBlock,Cell::QuestionBlockarmor,Cell::EmptyQuestionBlock }, hit_box);

		if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
			{
				return 0 == i_value;
			}))
		{
			if (0 > vertical_speed)
			{
				y = CELL_SIZE * (1 + floor(vertical_speed + y) / CELL_SIZE);
			}
			else
			{
				y = CELL_SIZE * (ceil((vertical_speed + y) / CELL_SIZE) - 1);
			}

			vertical_speed = 0;
		}
		else
		{
			bool changed = 0;

			if (0 == getDead(0))
			{
				for (unsigned short a = 0; a < enemies.size(); a++)
				{
					if (shared_from_this() != enemies[a] && 0 == enemies[a]->getDead(0) && 1 == hit_box.intersects(enemies[a]->getHitbox()))
					{
						changed = 1;

						if (0 > vertical_speed)
						{
							y = enemies[a]->getHitbox().height + enemies[a]->getHitbox().top;
						}
						else
						{
							y = enemies[a]->getHitbox().top - CELL_SIZE;
						}

						vertical_speed = 0;

						break;
					}
				}
			}

			if (0 == changed)
			{
				y += vertical_speed;
			}
		}

		if (0 == getDead(0))
		{
			hit_box = getHitbox();
			hit_box.left += horizontal_speed;

			collision = mapa.mapCollision({ Cell::Brick, Cell::Wall,Cell::Ground,Cell::Pipehighleft,Cell::Pipehighright,Cell::Pipelowleft,Cell::Pipelowright,Cell::QuestionBlock,Cell::QuestionBlockarmor,Cell::EmptyQuestionBlock }, hit_box);

			if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
				{
					return 0 == i_value;
				}))
			{
				if (0 < horizontal_speed)
				{
					x = CELL_SIZE * (ceil((horizontal_speed + x) / CELL_SIZE) - 1);
				}
				else
				{
					x = CELL_SIZE * (1 + floor((horizontal_speed + x) / CELL_SIZE));
				}

				horizontal_speed *= -1;
			}
			else
			{
				bool changed = 0;

				//kolizja innym goomba .
				for (unsigned short a = 0; a < enemies.size(); a++)
				{
					if (shared_from_this() != enemies[a] && 0 == enemies[a]->getDead(0) && 1 == hit_box.intersects(enemies[a]->getHitbox()))
					{
						changed = 1;

						horizontal_speed *= -1;

						break;
					}
				}

				if (0 == changed)
				{
					x += horizontal_speed;
				}

				if (0 == mario.getDead() && 1 == getHitbox().intersects(mario.getHitbox()))
				{

					if (0 < mario.getVertical_speed())
					{

						die(1);

						mario.setVertical_speed(0.6f * MARIO_JUMP_SPEED);
					}
					else
					{

						mario.die();
					}
				}

			}
			GoombaAnimation.update();

		}
		else if (GOOMBA_DEATH_DURATION > death_timer)
		{
			if (0 < death_timer)
			{
				death_timer--;
			}
			else
			{
				die(0);
			}
		}
		else
		{
			y += vertical_speed;
		}
	}
	if (SCREEN_HEIGHT <= y)
	{
		die(0);
	}
}

void Goomba::die(const unsigned short type)
{
	switch (type)
	{
	case 0:
	{
		 
		dead = 1;

		break;
	}
	case 1:
	{
		//mario zabija 
		if (0 == no_collision_dying)
		{
			death_timer--;
		}

		break;
	}
	case 2:
	{
		if (GOOMBA_DEATH_DURATION == death_timer)
		{
			//Koopa zabija Goombe
			no_collision_dying = 1;
			death_timer--;
			vertical_speed = 0.5f * MARIO_JUMP_SPEED;
			GoombaTexture.loadFromFile("Resources/Goombadeath1.png");

		}
	}
	}
}