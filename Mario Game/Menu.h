#pragma once
#include"Global.h"
#include <iostream>
#include "Player.h"
#include <regex>
#include <SFML/Graphics.hpp>;
#include <vector>;
#include <string>;
#include <fstream>;
#include <sstream>;




class Menu
{
    sf::Font font;
    std::vector<sf::Text> menuItems;
    int selectedItemIndex;
    sf::Text loginText;
    sf::Text scoreText;
    sf::Text passwordText;
    sf::Text loginInput;
    sf::Text passwordInput;
    sf::Text messageText;
    sf::Text loggedInUserText;
    std::string inputLogin;
    std::string inputPassword;

    bool rankingScreen;
    bool isLoginScreen;
    bool isRegisterScreen;
    bool isEnteringLogin;
    bool isEnteringPassword;
    bool isLogged;
    bool isEnteringUsername;
    void loadPlayersFromFile();
    void sortRanking();

public:

    std::string loggedInUser;
    std::vector<Player> ranking;
    Menu(unsigned short width, unsigned short height);
    void draw(sf::RenderWindow& window);
    void handleEvent(sf::Event& event, bool& isMenuOpen, bool& isRunning, std::vector<Player>& players);
    bool validatePassword(const std::string& password);
    bool userExists(const std::string& username);
    bool validateLogin(const std::string& username, const std::string& password);
    void drawLoginScreen(sf::RenderWindow& window);
    void drawRegisterScreen(sf::RenderWindow& window);
    void login();
    void registerUser();
    void drawRanking(sf::RenderWindow& window);
    void updateMenuOptionText(int index, const std::string& newText);
};