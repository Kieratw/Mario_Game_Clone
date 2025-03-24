#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Global.h"
#include "Animation.h"
#include "Enemy.h"

class Mapa;
class Mario;

class Koopa : public Enemy
{
private:
	sf::Texture KoopaTexture;
	sf::Sprite KoopaSprite;
	Animation KoopaAnimation;
	Animation GetOutAnimation;
	bool check_collision;
	bool no_collision_dying;
	unsigned short death_timer;
	unsigned short state;
	unsigned short get_out_timer;
	bool flipped;
public:
	Koopa(const float i_x, const float i_y);
	bool getDead(const bool type) const;
	void draw(const unsigned view_x, sf::RenderWindow& window);
	void update(const unsigned view_x, const std::vector<std::shared_ptr<Enemy>>& enemies, Mapa& mapa, Mario& mario);
	void die(const unsigned short type);
};