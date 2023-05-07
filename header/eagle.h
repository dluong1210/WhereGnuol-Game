#ifndef EAGLE_H
#define EAGLE_H

#include <iostream>
#include <vector>
#include <cmath>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "baseObject.h"
#include "gameMap.h"
#include "func.h"
#include "player.h"

class Player;

class Eagle : public baseObject
{
private:
    const int E_WIDTH = 52;
    const int E_HEIGHT = 32;
    const int E_SPEED = 4;
    int HP = 3;
    int x_vel = 0, y_vel = 0;
    int index_origin, indexTarget, distanceToPlayer;
    std::vector<path_pos> checkPath;
    gameMap Map;

    static const int FLY_FRAMES = 4;
    static const int DIE_FRAMES = 6;

    SDL_Rect animationFly[FLY_FRAMES];
    SDL_Rect animationDie[DIE_FRAMES];

    Mix_Chunk* sfx_eagle = Mix_LoadWAV( "src/Sfx/sfx_enemy_hurt.wav" );

    int countFly = 0, countHit = 0, countHurt = 0, countDie = 0;
    bool flying = true, attacking = false, hurting = false, die = false;

public:
    Eagle( const int& _x, const int& _y, SDL_Texture* image, gameMap& _map );

    void calculateShortestPath( const int& newIndexTarget );
    void moveToTarget();
    void autoMove();
    void handleStatus( const Player& _player );
    void beingHit( const Player& _player );
    void update( const Player& _player );
    void render( const SDL_Rect& camera );

    void resetCheckPath();
    int getStartMap() const { return Map.getStart_x(); }
    int getDistance() const { return distanceToPlayer; }
    bool isAttack() const { return countHit >= 22; }
    bool isDead() const { return countDie >= 35; }
};

#endif // EAGLE_H
