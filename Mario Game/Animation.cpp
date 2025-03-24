#include "Animation.h"

Animation::Animation(const std::string& texturee, unsigned short frame_width, unsigned short animation_speed)
	: frame_width(frame_width), animation_speed(std::max<unsigned short>(1, animation_speed)), flipped(0), animation_iterator(0), current_frame(0)
{
	texture.loadFromFile(texturee);
	total_frames = texture.getSize().x / frame_width;
}

void Animation::draw(sf::RenderWindow& window)
{

	sprite.setTexture(texture);

	if (0 == flipped)
	{
		sprite.setTextureRect(sf::IntRect(current_frame * frame_width, 0, frame_width, texture.getSize().y));
	}
	else
	{
		sprite.setTextureRect(sf::IntRect(frame_width * (1 + current_frame), 0, -frame_width, texture.getSize().y));//odwrocenie za pomoca minusa 
	}

	window.draw(sprite);
}
void Animation::update()
{
	while (animation_iterator >= animation_speed)
	{
		animation_iterator -= animation_speed;

		current_frame = (1 + current_frame) % total_frames;
	}

	animation_iterator++;
}
