#pragma once

#include <SFML/Graphics.hpp>
#include "Global.h"
#include "Animation.h"
#include "Mapa.h"

class Mario {
    sf::Sprite marioSprite;
    sf::Texture marioTexture;
    float horizontal_speed;
    float vertical_speed;
    float x;
    float y;
    unsigned int jump_timer;
    unsigned short death_timer;
    bool dead;
    bool flipped;
    bool on_ground;
    float enemy_bounce_speed;
    Animation walk_animation;
  
    unsigned short pointsarmor;
    unsigned short invincible_timer;

public:
    Mario();
    void draw(sf::RenderWindow& window);
    void drawstats(sf::RenderWindow& window);
    void setPosition(const float i_x, const float i_y);
    sf::FloatRect getHitbox() const;
    sf::Vector2f getPosition();
    void update(class Mapa& mapa);
    float getX() const { return x; }
    bool getDead() const;
    float getVertical_speed() const;
    void setVertical_speed(const float value);
    void die();
    unsigned short pointstimeminus;
};

