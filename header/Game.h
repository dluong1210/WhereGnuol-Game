#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <cstdlib>
#include <fstream>

#include <SDL.h>
#include <SDL_image.h>

#include "func.h"
#include "baseObject.h"
#include "gameMap.h"
#include "timer.h"
#include "player.h"
#include "enemy.h"
#include "eagle.h"
#include "menu.h"

class Game
{
private:
    bool runningGame = false;
    int scoreRun = 0, scoreEnemy = 0, totalScore = 0, hightScore = 0;
    int musicStatus = 0;

    SDL_Texture* tileSet = nullptr;
    SDL_Texture* playerTex[7] = {nullptr};
    SDL_Texture* enemyTex[3] = {nullptr};
    SDL_Texture* eagleTex = nullptr;
    SDL_Texture* liveBar = nullptr;
    SDL_Texture* heart = nullptr;
    SDL_Texture* menuTex[15] = {nullptr};

    Mix_Chunk* p_sfx[5] = {nullptr};
    Mix_Chunk* eagle_sfx = nullptr;
    Mix_Chunk* menuSfx[2] = {nullptr};
    Mix_Music* menuMus = nullptr;
    Mix_Music* gameMus = nullptr;

    SDL_Rect camera = {0, 0, WIDTH, HEIGHT };
    std::vector<SDL_Rect> tileClip;
    std::vector<SDL_Rect> HP_pos;

    Menu* menu;
    Player* player;
    std::vector<map_enemy> totalMap;
    std::deque<gameMap> listM;

    std::vector<Enemy*> enemyList;
    std::vector<Eagle*> eagleList;

public:
    Game(){;};
    bool init();
    bool loadMedia();
    bool loadMap();
    void setTileClip();

    bool createMap();
    bool createPlayer();
    bool createEnemy();
    bool createEagle();

    void update_Game();
    bool updateMap();
    void update_Enemy();
    void update_Eagle();
    void getHightScore();
    void setHightScore();

    void render_Game();
    void render_Map();
    void render_Enemy();
    void render_Eagle();
    void render_HP_Score();

    void playMusic();
    void resetGame();
    void handleInputGame( SDL_Event& e );

    void runGame( SDL_Event& e);
    bool isRunning() { return menu->isRunning() || runningGame; }
};
