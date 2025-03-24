#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <chrono>
#include "Mapa.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Enemy.h"
#include "Player.h"
#include <fstream>
#include <regex>
#include "Menu.h"



class Game
{
    sf::RenderWindow window;
    std::chrono::microseconds lag;
    std::chrono::steady_clock::time_point frame_start_time;
    std::chrono::steady_clock::time_point game_previous_time;
    sf::Font font;
    sf::Text player;
    Mapa mapa;
    Mario mario;
    Menu menu;
    bool menu_open;
    bool isRunning;
    bool gameEnded;
    std::chrono::steady_clock::time_point gameStartTime;
    std::chrono::milliseconds totalTime;
    std::vector<std::shared_ptr<Enemy>> enemies;

    std::vector<Player> players;
    void update();
    void render();
    void processEvents();
    void saveScoreToFile();
	void displayGameOver();
	void resetGame();
	void updateMenuStartOption();
    void drawTime(sf::RenderWindow& window);
  
 


public:
    Game();
    void run();
    std::string getFormattedTime() const;

   
};

