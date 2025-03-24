#pragma once
#include <string>

struct Player
{
    std::string username;
    std::string password;
    std::string score;

    Player(const std::string& name, const std::string& pass, std::string initialScore)
        : username(name), password(pass), score(initialScore) {}
};
