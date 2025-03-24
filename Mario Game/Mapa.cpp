#include "Mapa.h"
#include <iostream>

Mapa::Mapa(int version)
    : question_block_animation("Resources/QuestionBlock.png", CELL_SIZE, 5)
{
    if (version == 1) {
        image.loadFromFile("Resources/MAPlevel1.png");
    }
   
    
}
void Mapa::loadMap(std::vector<std::shared_ptr<Enemy>>& enemies)
{
    unsigned short mapheight;
    mapheight = floor(image.getSize().y / 3.f);


    for (unsigned short x = 0; x < image.getSize().x; ++x)
    {
        for (unsigned short y = 0; y < 3 * mapheight; y++)
        {
            sf::Color pixelColor = image.getPixel(x, y);


            if (y <= mapheight)
            {
                sf::Color pixelColor = image.getPixel(x, y);
                MapKey key = std::make_pair(x, y);
                if (sf::Color(0, 0, 0) == pixelColor)
                {
                    basic[key] = Cell::Ground;
                }
                else if (sf::Color(255, 255, 255) == pixelColor)
                {
                    basic[key] = Cell::Wall;
                }
                else if (sf::Color(182, 73, 0) == pixelColor)
                {
                    basic[key] = Cell::Brick;
                }
                else if (sf::Color(255, 73, 85) == pixelColor)
                {
                    specialelement[key] = Cell::QuestionBlock;
                    basic[key] = Cell::EmptyQuestionBlock;
                }
                else if (sf::Color(255, 146, 85) == pixelColor)
                {
                    specialelement[key] = Cell::QuestionBlockarmor;
                    basic[key] = Cell::EmptyQuestionBlock;
                }
                else if (sf::Color(0, 145, 0) == pixelColor)
                {
                    basic[key] = Cell::Pipehighleft;
                }
                else if (sf::Color(145, 244, 145) == pixelColor)
                {
                    basic[key] = Cell::Pipehighright;
                }
                else if (sf::Color(2, 69, 2) == pixelColor)
                {
                    basic[key] = Cell::Pipelowleft;
                }
                else if (sf::Color(11, 245, 11) == pixelColor)
                {
                    basic[key] = Cell::Pipelowright;
                }
                else if (sf::Color(240, 156, 0) == pixelColor)
                {
                    basic[key] = Cell::Wall;
                }
                else
                {
                    basic[key] = Cell::Empty;
                }


            }
            else if (y <= 2 * mapheight)
            {

                MapKey key = std::make_pair(x, y - 15);
                if (sf::Color(255, 0, 0) == pixelColor)
                {

                }
                else if (sf::Color(152, 0, 25) == pixelColor)
                {
                      enemies.push_back(std::make_shared<Goomba>(x * CELL_SIZE, (y - 15) * CELL_SIZE));
                }
                else if (sf::Color(0, 18, 152) == pixelColor)
                {                  
                    enemies.push_back(std::make_shared<Koopa>(x*CELL_SIZE, (y - 15)*CELL_SIZE));
                }

            }
            else if (y <= 3 * mapheight)
            {

                MapKey key = std::make_pair(x, y - 30);
                MapKey key2 = std::make_pair(x, y - 29);
                if (sf::Color(41, 135, 0) == pixelColor)
                {
                    background[key] = Background::Grassleft;

                }
                else if (sf::Color(59, 136, 26) == pixelColor)
                {
                    background[key] = Background::Grassright;
                }
                else if (sf::Color(76, 255, 0) == pixelColor)
                {
                    background[key] = Background::Grass;
                }
                else if (sf::Color(98, 255, 31) == pixelColor)
                {
                    background[key] = Background::Grasstop;
                }
                else if (sf::Color(7, 161, 1) == pixelColor)
                {
                    background[key] = Background::Grasstree;
                }
                else if (sf::Color(255, 255, 255) == pixelColor)
                {
                    background[key] = Background::Cloudleftup;
                    background[key2] = Background::Cloudleft;
                }
                else if (sf::Color(189, 197, 255) == pixelColor)
                {
                    background[key] = Background::Cloudup;
                    background[key2] = Background::Cloud;
                }
                else if (sf::Color(68, 91, 254) == pixelColor)
                {
                    background[key] = Background::Cloudrightup;
                    background[key2] = Background::Cloudright;
                }
                else if (sf::Color(52, 235, 255) == pixelColor)
                {
                    background[key] = Background::Bushleft;
                }
                else if (sf::Color(60, 92, 95) == pixelColor)
                {
                    background[key] = Background::Bushright;
                }
                else if (sf::Color(157, 245, 255) == pixelColor)
                {
                    background[key] = Background::Bush;
                }


            }
        }
    }
}

void Mapa::drawMap(sf::RenderWindow& window,const unsigned view_x)
{
    sf::Texture cellTexture;
    sf::Sprite cellSprite;
    cellTexture.loadFromFile("Resources/Map.png");
    cellSprite.setTexture(cellTexture);
    float start = std::max(0.0f, (static_cast<float>(view_x) - static_cast<float>(SCREEN_WIDTH))) / static_cast<float>(CELL_SIZE);
    
    float end = ceil(view_x + SCREEN_WIDTH) / CELL_SIZE;
  
    for (const auto& mapa : background)
    {
        const MapKey& key = mapa.first;
        const Background& background = mapa.second;
     
        if (key.first >= start && key.first <= end)
        {
            cellSprite.setPosition(key.first * CELL_SIZE, key.second * CELL_SIZE);
            unsigned short sprite_x = 0;
            unsigned short sprite_y = 3;
            if (background == Background::Grassleft)
            {
                sprite_x = 1;
                sprite_y = 1;
            }
            else if (background == Background::Grassright)
            {
                sprite_x = 2;
                sprite_y = 1;
            }
            else if (background == Background::Grasstop)
            {
                sprite_x = 0;
                sprite_y = 1;
            }
            else if (background == Background::Grass)
            {
                sprite_x = 4;
                sprite_y = 1;
            }
            else if (background == Background::Grasstree)
            {
                sprite_x = 3;
                sprite_y = 1;
            }
            else if (background == Background::Cloud)
            {
                sprite_x = 8;
                sprite_y = 1;
            }
            else if (background == Background::Cloudup)
            {
                sprite_x = 8;
                sprite_y = 0;
            }
            else if (background == Background::Cloudright)
            {
                sprite_x = 9;
                sprite_y = 1;
            }
            else if (background == Background::Cloudrightup)
            {
                sprite_x = 9;
                sprite_y = 0;
            }
            else if (background == Background::Cloudleft)
            {
                sprite_x = 7;
                sprite_y = 1;
            }
            else if (background == Background::Cloudleftup)
            {
                sprite_x = 7;
                sprite_y = 0;
            }

            else if (background == Background::Bushleft)
            {
                sprite_x = 4;
                sprite_y = 0;
            }
            else if (background == Background::Bushright)
            {
                sprite_x = 6;
                sprite_y = 0;
            }
            else if (background == Background::Bush)
            {
                sprite_x = 5;
                sprite_y = 0;
            }
            else if (background == Background::Emptyy)
            {
                continue;
            }

            cellSprite.setTextureRect(sf::IntRect(CELL_SIZE * sprite_x, CELL_SIZE * sprite_y, CELL_SIZE, CELL_SIZE));

            window.draw(cellSprite);
        }
    }
    for (const auto& mapa : basic)
    {
        const MapKey& key = mapa.first;
        const Cell& cell = mapa.second;
        if (key.first >= start && key.first <= end)
        {
            cellSprite.setPosition(key.first * CELL_SIZE, key.second * CELL_SIZE);
            unsigned short sprite_x = 6;
            unsigned short sprite_y = 1;
            if (cell == Cell::Ground)
            {
                sprite_x = 2;
                sprite_y = 0;

            }
            else if (cell == Cell::Wall)
            {
                sprite_x = 3;
                sprite_y = 0;

            }
            else if (cell == Cell::Brick)
            {
                sprite_x = 0;
                sprite_y = 0;
            }
            else if (cell == Cell::Pipehighleft)
            {
                sprite_x = 10;
                sprite_y = 0;
            }
            else if (cell == Cell::Pipehighright)
            {
                sprite_x = 11;
                sprite_y = 0;
            }
            else if (cell == Cell::Pipelowleft)
            {
                sprite_x = 10;
                sprite_y = 1;
            }
            else if (cell == Cell::Pipelowright)
            {
                sprite_x = 11;
                sprite_y = 1;
            }         
            else if (cell == Cell::Wall)
            {
                sprite_x = 3;
                sprite_y = 0;
            }
            else if (cell== Cell::EmptyQuestionBlock)
            {
                sprite_x = 6;
                sprite_y = 1;
            }
            else if (cell == Cell::Empty)
            {
                continue;
            }
            cellSprite.setTextureRect(sf::IntRect(CELL_SIZE * sprite_x, CELL_SIZE * sprite_y, CELL_SIZE, CELL_SIZE));

            window.draw(cellSprite);
        }
        
    }
    for (const auto& mapa : specialelement)
    {
        const MapKey& key = mapa.first;
        const Cell& cell = mapa.second;
        
        if (key.first >= start && key.first <= end)
        {
           
            if (cell == Cell::QuestionBlock)
            {
                question_block_animation.setPosition(key.first* CELL_SIZE, key.second* CELL_SIZE);
                question_block_animation.draw(window);
            }
            else if (cell == Cell::QuestionBlockarmor)
            {
                question_block_animation.setPosition(key.first* CELL_SIZE, key.second* CELL_SIZE);
                question_block_animation.draw(window);
            }
        }
    }
}



std::vector<unsigned char> Mapa::mapCollision(const std::vector<Cell>& i_check_cells, const sf::FloatRect& i_hitbox)
{
    std::vector<unsigned char> output;
    for (short a = floor(i_hitbox.top / CELL_SIZE); a <= floor((ceil(i_hitbox.height + i_hitbox.top) - 1) / CELL_SIZE); a++)
    {
        output.push_back(0);

        for (short b = floor(i_hitbox.left / CELL_SIZE); b <= floor((ceil(i_hitbox.left + i_hitbox.width) - 1) / CELL_SIZE); b++)
        {
            if (0 <= b && b < basic.size())
            {
                if (0 <= a && a < 2 * SCREEN_HEIGHT / CELL_SIZE)
                {
                    MapKey key = std::make_pair(b, a);
                    if (i_check_cells.end() != std::find(i_check_cells.begin(), i_check_cells.end(), basic[key]))
                    {
                        output[a - floor(i_hitbox.top / CELL_SIZE)] += pow(2, floor((ceil(i_hitbox.left + i_hitbox.width) - 1) / CELL_SIZE) - b);
                    }
                }
            }
            //wychodzimy poza mape wiec symulujemy ze to nowe b jest sciana 
            else if (i_check_cells.end() != std::find(i_check_cells.begin(), i_check_cells.end(), Cell::Wall))
            {
                output[a - floor(i_hitbox.top / CELL_SIZE)] += pow(2, floor((ceil(i_hitbox.left + i_hitbox.width) - 1) / CELL_SIZE) - b);
            }
        }
    }

    return output;
}
std::vector<unsigned char> Mapa::mapCollision2(const std::vector<Cell>& i_check_cells, const sf::FloatRect& i_hitbox)
{
    std::vector<unsigned char> output;
    for (short a = floor(i_hitbox.top / CELL_SIZE); a <= floor((ceil(i_hitbox.height + i_hitbox.top) - 1) / CELL_SIZE); a++)
    {
        output.push_back(0);

        for (short b = floor(i_hitbox.left / CELL_SIZE); b <= floor((ceil(i_hitbox.left + i_hitbox.width) - 1) / CELL_SIZE); b++)
        {
            if (0 <= b && b < basic.size())
            {
                if (0 <= a && a < 2 * SCREEN_HEIGHT / CELL_SIZE)
                {
                    MapKey key = std::make_pair(b, a);
                    if (i_check_cells.end() != std::find(i_check_cells.begin(), i_check_cells.end(), specialelement[key]))
                    {
                        output[a - floor(i_hitbox.top / CELL_SIZE)] += pow(2, floor((ceil(i_hitbox.left + i_hitbox.width) - 1) / CELL_SIZE) - b);
                    }
                }
            }
            //wychodzimy poza mape wiec symulujemy ze to nowe b jest sciana 
            else if (i_check_cells.end() != std::find(i_check_cells.begin(), i_check_cells.end(), Cell::Wall))
            {
                output[a - floor(i_hitbox.top / CELL_SIZE)] += pow(2, floor((ceil(i_hitbox.left + i_hitbox.width) - 1) / CELL_SIZE) - b);
            }
        }
    }

    return output;
}
void Mapa::updateView(sf::RenderWindow& window, float x)
{
    question_block_animation.update();
    sf::View view = window.getView();
    float viewHalfWidth = SCREEN_WIDTH / 2.0f;
    float view_x = std::max(x + CELL_SIZE, viewHalfWidth);
    sf::Vector2u size = image.getSize();
    // Aby nie wyjœæ poza praw¹ krawêdŸ mapy:
    view_x = std::min(round(view_x), size.x * CELL_SIZE - viewHalfWidth);

    view.setCenter(view_x, SCREEN_HEIGHT / 2);
    window.setView(view);

}

void Mapa::deleteQuestionBlock(const int x, const int y)
{
    MapKey key = std::make_pair(x, y);
	specialelement.erase(key);
    MapKey key1 = std::make_pair(x+1, y);
    specialelement.erase(key1);
    MapKey key2 = std::make_pair(x -1, y);
    specialelement.erase(key2);
}

bool Mapa::isEndReached(const float x)
{
    if(x >= image.getSize().x * CELL_SIZE - CELL_SIZE)
	{
		return true;
	}
    else
    return false;
}
