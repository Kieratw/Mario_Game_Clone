#include "Enemy.h"
#include "Mapa.h"
#include "Mario.h"
#include "Global.h"
#include <SFML/Graphics.hpp>
#include "Animation.h"



Enemy::Enemy(const float i_x, const float i_y)
	: dead(0), horizontal_speed(0), vertical_speed(0), x(i_x), y(i_y)
{}

float Enemy::getX()
{
	return x;
}

bool Enemy::getDead(const bool type) const
{
	return dead;
}

void Enemy::die(const unsigned short type)
{
	dead = 1;
}
sf::FloatRect Enemy::getHitbox() const
{
	return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
}
