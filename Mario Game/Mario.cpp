#include "Mario.h"
#include <iostream>

Mario::Mario()
	:x(100), y(250), horizontal_speed(0), vertical_speed(0), enemy_bounce_speed(0), jump_timer(0), dead(0), walk_animation("Resources/MarioWalk.png", 
		CELL_SIZE, 5), flipped(0), death_timer(MARIO_DEATH_DURATION), pointstimeminus(0), pointsarmor(0), invincible_timer(0), on_ground(0)
{

	marioTexture.loadFromFile("Resources/Mario.png");
	marioSprite.setTexture(marioTexture);
}

void Mario::draw(sf::RenderWindow& window)
{
	if (0 == invincible_timer / MARIO_BLINKING % 2)
	{
		bool drawsprite = 1;
		marioSprite.setPosition(round(x), round(y));
		if (dead == 0 || dead == 1)
		{
			if (on_ground == 1)
			{
				if (horizontal_speed == 0)
				{
					marioTexture.loadFromFile("Resources/Mario.png");
				}
				else if ((0 < horizontal_speed && 0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
					1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ||
					(0 > horizontal_speed && 0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
						1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
				{
					marioTexture.loadFromFile("Resources/MarioBrake.png");
				}
				else
				{
					drawsprite = 0;
					walk_animation.setFlipped(flipped);
					walk_animation.setPosition(round(x), round(y));
					walk_animation.draw(window);
				}

			}
			else
			{
				marioTexture.loadFromFile("Resources/MarioJump.png");
			}
			if (drawsprite)
			{
				if (!flipped)
				{
					marioSprite.setTextureRect(sf::IntRect(0, 0, marioTexture.getSize().x, marioTexture.getSize().y));
				}
				else
				{
					marioSprite.setTextureRect(sf::IntRect(marioTexture.getSize().x, 0, -static_cast<int>(marioTexture.getSize().x), marioTexture.getSize().y));
				}

				window.draw(marioSprite);
			}
		}
	}
}

void Mario::drawstats(sf::RenderWindow& window)
{
	
	sf::View originalView = window.getView();

	
	window.setView(window.getDefaultView());

	
	sf::Texture heartTexture;
	heartTexture.loadFromFile("Resources/Heart.png");
	sf::Sprite heartSprite;
	heartSprite.setTexture(heartTexture);
	for (unsigned short i = 0; i < pointsarmor; i++)
	{
		heartSprite.setPosition(i * CELL_SIZE, 0);
		window.draw(heartSprite);
	}

	
	sf::RectangleShape whiteSquare(sf::Vector2f(CELL_SIZE, CELL_SIZE));
	whiteSquare.setFillColor(sf::Color(0, 0, 0, 150));
	whiteSquare.setPosition(0, CELL_SIZE);
	window.draw(whiteSquare);

	
	sf::Font font;
	font.loadFromFile("Resources/Alexandriaflf.ttf");
	sf::Text timeText;
	timeText.setFont(font);
	timeText.setCharacterSize(24);
	timeText.setFillColor(sf::Color::White);
	timeText.setPosition((CELL_SIZE / 4)+2, CELL_SIZE + (CELL_SIZE / 4)-7); 
	timeText.setString(std::to_string(pointstimeminus));
	window.draw(timeText);

	
	window.setView(originalView);
}

void Mario::setPosition(const float i_x, const float i_y)
{
	x = i_x;
	y = i_y;
}

sf::Vector2f Mario::getPosition()
{
	return marioSprite.getPosition();
}

sf::FloatRect Mario::getHitbox() const
{
	return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
}
void Mario::update(Mapa& mapa)
{

	if (0 == dead)
	{
		bool moving = 0;
		sf::FloatRect hit_box = getHitbox();
		std::vector<unsigned char> collision;

		on_ground = 0;

		if (0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			moving = 1;
			walk_animation.setFlipped(1);
			horizontal_speed = std::max(horizontal_speed - MARIO_ACCELERATION, -MARIO_WALK_SPEED);
		}
		else if (0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			moving = 1;
			walk_animation.setFlipped(0);
			horizontal_speed = std::min(MARIO_ACCELERATION + horizontal_speed, MARIO_WALK_SPEED);
		}

		if (0 == moving)
		{
			if (0 < horizontal_speed)
			{
				horizontal_speed = std::max<float>(0, horizontal_speed - MARIO_ACCELERATION);
			}
			else if (0 > horizontal_speed)
			{
				horizontal_speed = std::min<float>(0, MARIO_ACCELERATION + horizontal_speed);
			}
		}
		//kolizja pozioma 
		hit_box.left += horizontal_speed;
		collision = mapa.mapCollision({ Cell::Brick, Cell::Wall,Cell::Ground,Cell::Pipehighleft,Cell::Pipehighright,Cell::Pipelowleft,Cell::Pipelowright,Cell::QuestionBlock,Cell::QuestionBlockarmor,Cell::EmptyQuestionBlock }, hit_box);
		if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
			{
				return 0 == i_value;

			}))
		{
			moving = 0;

			if (0 < horizontal_speed)
			{
				x = CELL_SIZE * (ceil((horizontal_speed + x) / CELL_SIZE) - 1);
			}
			else if (0 > horizontal_speed)
			{
				x = CELL_SIZE * (1 + floor((horizontal_speed + x) / CELL_SIZE));
			}

			horizontal_speed = 0;
		}
		else
		{
			x += horizontal_speed;
		}
		if (0 == horizontal_speed)
		{
			if (1 == moving)
			{
				flipped = 1 - flipped;
			}
		}
		else if (0 < horizontal_speed)
		{
			flipped = 0;
		}
		else if (0 > horizontal_speed)
		{
			flipped = 1;
		}
		hit_box = getHitbox();
		hit_box.top++;

		collision = mapa.mapCollision({ Cell::Brick, Cell::Wall,Cell::Ground,Cell::Pipehighleft,Cell::Pipehighright,Cell::Pipelowleft,Cell::Pipelowright,Cell::QuestionBlock,Cell::QuestionBlockarmor,Cell::EmptyQuestionBlock }, hit_box);


		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			if (0 == vertical_speed && 0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
				{
					return 0 == i_value;
				}))
			{
				vertical_speed = MARIO_JUMP_SPEED;
				jump_timer = MARIO_JUMP_TIMER;
			}
			else if (0 < jump_timer) //moc skoku 
			{
				vertical_speed = MARIO_JUMP_SPEED;

				jump_timer--;//d³uzej utrzymujemy vertical speed do pewnego momentu wyzerowania jumptimer 
			}
			else
			{
				vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);
			}
		}
		else
		{
			vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);

			jump_timer = 0;
		}

		hit_box = getHitbox();
		hit_box.top += vertical_speed;

		collision = mapa.mapCollision({ Cell::Brick, Cell::Wall,Cell::Ground,Cell::Pipehighleft,Cell::Pipehighright,Cell::Pipelowleft,Cell::Pipelowright,Cell::QuestionBlock,Cell::QuestionBlockarmor,Cell::EmptyQuestionBlock }, hit_box);
		if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
			{
				return 0 == i_value;
			}))
		{
			if (0 > vertical_speed)
			{
				collision=mapa.mapCollision2({ Cell::QuestionBlock }, hit_box);
				if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
					{
						return 0 == i_value;
					}))
				{
					pointstimeminus += 1;
				 
					mapa.deleteQuestionBlock(round(x / CELL_SIZE), round((y / CELL_SIZE)-1));
				}
				
				collision = mapa.mapCollision2({ Cell::QuestionBlockarmor }, hit_box);
				if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
					{
						return 0 == i_value;
					}))
				{
					if (2 > pointsarmor)
					{
						pointsarmor += 1;
					}
					
					mapa.deleteQuestionBlock(round(x / CELL_SIZE), round((y / CELL_SIZE) - 1));
				}
				y = CELL_SIZE * (1 + floor((vertical_speed + y) / CELL_SIZE));
			}
			else if (0 < vertical_speed)
			{
				y = CELL_SIZE * (ceil((vertical_speed + y) / CELL_SIZE) - 1);
			}

			jump_timer = 0;

			vertical_speed = 0;
		}

		else
		{
			y += vertical_speed;
		}
		hit_box.top++;

		collision = mapa.mapCollision({ Cell::Brick, Cell::Wall,Cell::Ground,Cell::Pipehighleft,Cell::Pipehighright,Cell::Pipelowleft,Cell::Pipelowright,Cell::QuestionBlock,Cell::QuestionBlockarmor,Cell::EmptyQuestionBlock }, hit_box);
		if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
			{
				return 0 == i_value;
			}))
		{
			on_ground = 1;
		}

		walk_animation.update();
		
		if (0 < invincible_timer)
		{
			invincible_timer--;
		}
	}
	else
	{
		if (0 == death_timer)
		{
			vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);
			y += vertical_speed;
		}
		else if (1 == death_timer)
		{
			vertical_speed = MARIO_JUMP_SPEED;
		}

		death_timer = std::max(0, death_timer - 1);
	}
}

bool Mario::getDead() const
{
	return dead;
}

float Mario::getVertical_speed() const
{
	return vertical_speed;
}
void Mario::setVertical_speed(const float value)
{
	vertical_speed = value;
}
void Mario::die()
{
	if(pointsarmor>0&& 0 == invincible_timer)
	{
		pointsarmor -= 1;

		invincible_timer = MARIO_INVINCIBILITY_DURATION;
		std::cout << "armor" << pointsarmor << std::endl;
	}
	
	else if (0 == invincible_timer)
	{
		dead = 1;

		marioTexture.loadFromFile("Resources/MarioDeath.png");
	}
}