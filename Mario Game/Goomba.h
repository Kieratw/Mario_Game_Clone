#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Global.h"
#include "Animation.h"
#include "Enemy.h"


class Mapa;
class Mario;

class Goomba : public Enemy
{
private:
	sf::Texture GoombaTexture;
	sf::Sprite GoombaSprite;
	Animation GoombaAnimation;
	bool no_collision_dying;//umiera i nie ma z nim kolizji dzieki temu 
	unsigned short death_timer;
public:
	Goomba(const float i_x, const float i_y);
	bool getDead(const bool type) const;
	void draw(const unsigned view_x, sf::RenderWindow& window);
	void update(const unsigned view_x, const std::vector<std::shared_ptr<Enemy>>& enemies, Mapa& mapa, Mario& mario);
	void die(const unsigned short type);
};