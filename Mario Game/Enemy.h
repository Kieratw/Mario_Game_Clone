#pragma once

#include <SFML/Graphics.hpp>
#include "Global.h"
#include <memory>

class Mapa;
class Mario;

class Enemy : public std::enable_shared_from_this<Enemy>
{
protected:
    bool dead;
    float horizontal_speed;
    float vertical_speed;
    float x, y;

public:
    Enemy(const float i_x, const float i_y);
    virtual float getX();
    virtual bool getDead(const bool type) const;
    virtual void die(const unsigned short type);
    sf::FloatRect getHitbox() const;
    virtual void draw(const unsigned view_x, sf::RenderWindow& window) = 0;

    virtual void update(const unsigned view_x, const std::vector<std::shared_ptr<Enemy>>& enemies, Mapa& mapa, Mario& mario) = 0;



};