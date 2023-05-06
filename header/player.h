#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <deque>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "func.h"
#include "enemy.h"
#include "eagle.h"

class gameMap;
class Enemy;
class Eagle;

class Player : public baseObject
{
private:
    int P_WIDTH = 48;
    int P_HEIGHT = 56;
    int P_SPEED = 4;
    int HP = 3;
    int index_mapP = 0, startX_mapP;
    int x_vel = 0, y_vel = 0;

    static const int WALKING_FRAMES = 8;
    static const int IDLING_FRAMES = 14;
    static const int JUMPING_FRAMES = 8;
    static const int FALLING_FRAMES = 8;
    static const int ATTACKING_FRAMES = 6;
    static const int HURTING_FRAMES = 6;
    static const int DIE_FRAMES = 32;

    SDL_Rect animationFalling[FALLING_FRAMES];
    SDL_Rect animationPlayer[32];

    SDL_Texture* p_texture[7];
    Mix_Chunk* p_sfx[5];

    int countWalk = 0, countIdle = 0, countJump = 0, countFall = 0, countHit = 0, countHurt = 0, countDead = 0;
    bool idling = false, grounded = false, walking = false, jumping= false, falling = false, attacking = false, hurting = false, die = false ;

public:
    enum status{
        jump,
        fall,
        attack,
        hurt,
        dead,
        idle,
        walk
    };
    Player( int _x , int _y , SDL_Texture* image[], Mix_Chunk* sfx[] );
    void updatePlayer( std::deque<gameMap>& listM, std::vector<Enemy*>& enemyList, std::vector<Eagle*>& eagleList );

    void handleEvent( SDL_Event& e );
    void handleStatus();
    void handleCollision( std::deque<gameMap>& listM );
    void beingHit( std::vector<Enemy*>& enemyList, std::vector<Eagle*>& eagleList );
    void Jump();
    void buffHP() { if( HP < 3 ) HP++; }

    void mapPresent( std::deque<gameMap>& listM );
    void changeCam( SDL_Rect& camera, std::deque<gameMap>& listM );
    void renderPlayer( SDL_Rect& camera );
    void resetPlayer();

    int getStartMap() const { return startX_mapP; }
    int getHP() const { return HP; }
    bool check_attack() const { return countHit == 26; }
    bool isDead() const { return countDead >= 32*6-1; }
};

#endif // PLAYER_H
