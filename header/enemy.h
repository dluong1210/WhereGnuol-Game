#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include <cmath>

#include "func.h"
#include "gameMap.h"
#include "player.h"

class gameMap;
class Player;

class Enemy : public baseObject
{
private:
    int E_WIDTH = 56;
    int E_HEIGHT = 48;
    int E_SPEED = 4;
    int HP;
    int type;
    int origin_x, distanceToPlayer;
    int x_vel = 0, y_vel = 0;
    gameMap Map;

    static const int WALK_FRAMES = 6;
    static const int FALL_FRAMES = 6;
    static const int ATTACK_FRAMES = 6;
    static const int HURT_FRAMES = 4;
    static const int DIE_FRAMES = 8;

    SDL_Rect animationWalk[WALK_FRAMES];
    SDL_Rect animationFall[FALL_FRAMES];
    SDL_Rect animationAttack[ATTACK_FRAMES];
    SDL_Rect animationHurt[HURT_FRAMES];
    SDL_Rect animationDie[DIE_FRAMES];

    int countWalk = 0, countFall = 0, countHit = 0, countHurt = 0, countDie = 0;
    bool walking = false, falling = false, grounded = true, attacking = false, hurting = false, die = false;

public:
    Enemy( const int& _x, const int& _y, SDL_Texture* image, gameMap& _map, int _type );

    bool enableToMove( Player& _player);
    bool enableToMove();
    void moveToPlayer( Player& _player );
    void autoMove( Player& _player );

    void updateEnemy( Player& _player);
    void render( SDL_Rect& camera);

    int getStartMap() const { return Map.getStart_x(); }
    int getDistance() const { return distanceToPlayer; }
    int getType() const { return type; }
    bool isAttack() const{ return countHit >= 30; }
    bool isDead() const { return countDie >= 63; }
};

#endif // ENEMY_H
