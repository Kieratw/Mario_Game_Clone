#include "Koopa.h"
#include "Mario.h"
#include "Mapa.h"
#include "Global.h"
#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include "Animation.h"

Koopa::Koopa(const float i_x, const float i_y)
	:Enemy(i_x, i_y), KoopaAnimation("Resources/KoopaWalk.png", CELL_SIZE, 15), no_collision_dying(0), death_timer(GOOMBA_DEATH_DURATION), state(0), GetOutAnimation("Resources/KoopaGetOut.png", CELL_SIZE, 5)
{
	horizontal_speed = -KOOPA_SPEED;
	KoopaTexture.loadFromFile("Resources/KoopaShell.png");
	KoopaSprite.setTexture(KoopaTexture);
	x = i_x;
	y = i_y;
}

bool Koopa::getDead(const bool type) const
{
	if (1 == type)
	{
		return dead;
	}
	else
	{
		return dead || no_collision_dying;
	}
}

void Koopa::die(const unsigned short type)
{
	switch (type)
	{
	case 0:
	{

		dead = 1;

		break;
	}
	case 2:
	{

		no_collision_dying = 1;
		vertical_speed = 0.5f * MARIO_JUMP_SPEED;
		KoopaTexture.loadFromFile("Resources/KoopaDeath.png");
	}
	}
}

void Koopa::draw(const unsigned view_x, sf::RenderWindow& window)
{
	if (-CELL_SIZE < round(y) && round(x) > static_cast<int>(view_x) - SCREEN_WIDTH  && round(x) < SCREEN_WIDTH + view_x && round(y) < SCREEN_HEIGHT)
	{
		if (0 < state || 1 == no_collision_dying)
		{
			if (0 == getDead(0) && 1 == state && get_out_timer <= 0.25f * KOOPA_GET_OUT_DURATION)
			{
				GetOutAnimation.setPosition(round(x), round(y));
				GetOutAnimation.draw(window);
			}
			else
			{
				KoopaSprite.setPosition(round(x), round(y));
				KoopaSprite.setTexture(KoopaTexture);

				window.draw(KoopaSprite);
			}
		}
		else
		{
			KoopaAnimation.setFlipped(flipped);
			KoopaAnimation.setPosition(round(x), round(y));
			KoopaAnimation.draw(window);
		}
	}
}

void Koopa::update(const unsigned view_x, const std::vector<std::shared_ptr<Enemy>>& enemies, Mapa& mapa, Mario& mario)
{


	if (-CELL_SIZE < y && x >= static_cast<int>(view_x) - CELL_SIZE - ENTITY_UPDATE_AREA - SCREEN_WIDTH / 2 && x < ENTITY_UPDATE_AREA + SCREEN_WIDTH + view_x && y < SCREEN_HEIGHT)
	{
		vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);

		if (0 == getDead(0))
		{
			std::vector<unsigned char> collision;

			sf::FloatRect hit_box = getHitbox();
			//grawitacja spadek 

			hit_box.top += vertical_speed;

			collision = mapa.mapCollision({ Cell::Brick, Cell::Wall,Cell::Ground,Cell::Pipehighleft,Cell::Pipehighright,Cell::Pipelowleft,Cell::Pipelowright,Cell::QuestionBlock,Cell::QuestionBlockarmor,Cell::EmptyQuestionBlock }, hit_box);

			if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
				{
					return 0 == i_value;
				}))
			{
				if (0 > vertical_speed)
				{
					y = CELL_SIZE * (1 + floor((vertical_speed + y) / CELL_SIZE));
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

				for (unsigned short a = 0; a < enemies.size(); a++)
				{
					if (shared_from_this() != enemies[a] && 0 == enemies[a]->getDead(0) && 1 == hit_box.intersects(enemies[a]->getHitbox()))
					{
						changed = 1;

						if (0 > vertical_speed)
						{
							y = enemies[a]->getHitbox().top + enemies[a]->getHitbox().height;
						}
						else
						{
							y = enemies[a]->getHitbox().top - CELL_SIZE;
						}

						vertical_speed = 0;

						break;
					}
				}

				if (0 == changed)
				{
					y += vertical_speed;
				}
			}

			hit_box = getHitbox();
			hit_box.left += horizontal_speed;
			//poruszanie sie 
			collision = mapa.mapCollision({ Cell::Brick, Cell::Wall,Cell::Ground,Cell::Pipehighleft,Cell::Pipehighright,Cell::Pipelowleft,Cell::Pipelowright,Cell::QuestionBlock,Cell::QuestionBlockarmor,Cell::EmptyQuestionBlock }, hit_box);

			if (0 == no_collision_dying && 0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
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

				for (unsigned short a = 0; a < enemies.size(); a++)
				{
					if (shared_from_this() != enemies[a] && 0 == enemies[a]->getDead(0) && 1 == hit_box.intersects(enemies[a]->getHitbox()))
					{
						if (0 == state)
						{
							changed = 1;

							horizontal_speed *= -1;
						}
						else if (2 == state)
						{
							enemies[a]->die(2);
						}

						break;
					}
				}

				if (0 == changed)
				{
					x += horizontal_speed;
				}
			}

			if (0 == mario.getDead() && 1 == getHitbox().intersects(mario.getHitbox()))
			{
				if (1 == check_collision)
				{
					if (0 == state)
					{

						if (0 < mario.getVertical_speed())
						{

							check_collision = 0;

							horizontal_speed = 0;

							state = 1;

							get_out_timer = KOOPA_GET_OUT_DURATION;

							mario.setVertical_speed(0.5f * MARIO_JUMP_SPEED);
						}
						else
						{
							mario.die();
						}
					}

					else if (1 == state)
					{
						check_collision = 0;

						state = 2;

						//w ktorym kierunku bedzie sie poruszac 
						if (x < mario.getX())
						{
							horizontal_speed = -KOOPA_SHELL_SPEED;
						}
						else
						{
							horizontal_speed = KOOPA_SHELL_SPEED;
						}

						if (0 < mario.getVertical_speed())
						{
							mario.setVertical_speed(0.5f * MARIO_JUMP_SPEED);
						}
					}
					else
					{

						if (0 < mario.getVertical_speed())
						{
							check_collision = 0;

							horizontal_speed = 0;

							state = 1;

							get_out_timer = KOOPA_GET_OUT_DURATION;

							mario.setVertical_speed(0.5f * MARIO_JUMP_SPEED);
						}
						//zabicie mario 
						else if ((0 < horizontal_speed && x < mario.getX()) ||
							(0 > horizontal_speed && x > mario.getX()))
						{
							mario.die();
						}
					}
				}
			}
			else
			{

				check_collision = 1;
			}

			if (0 < horizontal_speed)
			{
				flipped = 0;
			}
			else if (0 > horizontal_speed)
			{
				flipped = 1;
			}

			if (1 == state)
			{
				get_out_timer--;

				if (0 == get_out_timer)
				{
					state = 0;

					if (0 == flipped)
					{
						horizontal_speed = KOOPA_SPEED;
					}
					else
					{
						horizontal_speed = -KOOPA_SPEED;
					}
				}
				else
				{
					GetOutAnimation.update();
				}
			}

			KoopaAnimation.update();
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