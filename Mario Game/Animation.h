#pragma once


#include <SFML/Graphics.hpp>

class Animation
{
	bool flipped = false;
	unsigned short animation_iterator;
	unsigned short animation_speed;
	unsigned short current_frame;
	unsigned short frame_width;
	unsigned short total_frames;

	sf::Sprite sprite;

	sf::Texture texture;

public:
	Animation(const std::string& texturee, unsigned short frame_width, unsigned short animation_speed);
	void setAnimationSpeed(unsigned short animation_speedd)
	{
		animation_speed = std::max<unsigned short>(1, animation_speedd);
	}
	void update();
	void setFlipped(bool flippedd)
	{
		flipped = flippedd;
	}
	void draw(sf::RenderWindow& window);
	void setPosition(float x, float y)
	{
		sprite.setPosition(x, y);
	}


};