#pragma once

#include <map>
#include <SFML/Graphics.hpp>
#include "Global.h"
#include "Enemy.h"
#include "Goomba.h"
#include "Koopa.h"
class Mapa
{
    using MapKey = std::pair<int, int>;
    std::map<MapKey, Cell> basic;
    std::map<MapKey, Background> background;
    std::map<MapKey, Cell> specialelement;
    sf::Image image;
    Animation question_block_animation;

public:
    Mapa(int version);
    void loadMap(std::vector<std::shared_ptr<Enemy>>& enemies);
    void drawMap(sf::RenderWindow& window, const unsigned view_x);
    std::vector<unsigned char> mapCollision(const std::vector<Cell>& i_check_cells, const sf::FloatRect& i_hitbox);
    std::vector<unsigned char> mapCollision2(const std::vector<Cell>& i_check_cells, const sf::FloatRect& i_hitbox);
    void updateView(sf::RenderWindow& window, float x);
    void deleteQuestionBlock(const int x, const int y);
    bool isEndReached(const float x);
};

