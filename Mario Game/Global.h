#pragma once
#pragma once

#include <chrono>

constexpr unsigned short CELL_SIZE = 32;
constexpr unsigned short MARIO_HEIGHT = 32;
constexpr unsigned short MARIO_WIDTH = 32;
constexpr unsigned short SCREEN_HEIGHT = 480;
constexpr unsigned short SCREEN_WIDTH = 640;
constexpr unsigned short SCREEN_MENU_HEIGHT = 320;
constexpr unsigned short SCREEN_MENU_WIDTH = 520;
constexpr unsigned short ENTITY_UPDATE_AREA = 640;
constexpr float KOOPA_SHELL_SPEED = 4;
constexpr float MARIO_ACCELERATION = 0.5f;
constexpr float MARIO_JUMP_SPEED = -8;
constexpr float MARIO_WALK_SPEED = 4;
constexpr float MAX_VERTICAL_SPEED = 16;
constexpr unsigned char MARIO_BLINKING = 4;
constexpr unsigned char MARIO_INVINCIBILITY_DURATION = 128;
constexpr float GRAVITY = 0.5f;
constexpr float GOOMBA_SPEED = 2.5f;
constexpr float KOOPA_SPEED = 2.5f;
constexpr unsigned short MARIO_JUMP_TIMER = 8;
constexpr unsigned short MARIO_DEATH_DURATION = 32;
constexpr unsigned short GOOMBA_DEATH_DURATION = 32;
constexpr unsigned short KOOPA_GET_OUT_DURATION = 512;
constexpr int MAX_NUMBER_OF_ITEMS = 5;
using MapKey = std::pair<int, int>;
constexpr std::chrono::microseconds FRAME_DURATION(16667);

enum class Cell 
{
    Brick,
    QuestionBlock,
    QuestionBlockarmor,
    Empty,
    Pipehighleft,
    Pipehighright,
    Pipelowleft,
    Pipelowright,
    Wall,
    EmptyQuestionBlock,
    Ground
};

enum class Background {
    Grassleft,
    Grassright,
    Grass,
    Grasstree,
    Grasstop,
    Cloudleftup,
    Cloudup,
    Cloudrightup,
    Cloudleft,
    Cloud,
    Cloudright,
    Bushleft,
    Bushright,
    Bush,
    Emptyy
};