#include "eagle.h"

Eagle::Eagle( const int& _x, const int& _y, SDL_Texture* image, gameMap& _map ) : baseObject( _x, _y ){
    texture = image;
    collision = { x, y+16, E_WIDTH, E_HEIGHT };
    Map = _map;

    index_origin =  Map.getIndexInMap( collision );
    indexTarget = index_origin;
    checkPath.resize(34*17);
    resetCheckPath();

    for( int i=0 ; i<FLY_FRAMES ; i++){
        animationFly[i] = {i*40, 0, 40, 41};
    }
    for( int i=0 ; i<DIE_FRAMES ; i++){
        animationDie[i] = {i*40, 41, 40, 41};
    }
}

void Eagle::resetCheckPath()
{
    for( int i=0 ; i<checkPath.size() ; i++){
        checkPath[i].flowDirection_X = 0;
        checkPath[i].flowDirection_Y = 0;
        checkPath[i].flowDistance = flowDistanceMax;

        int X = i%MAP_WIDTH;
        int Y = i/MAP_WIDTH;

        if( Map.getDataMap( X, Y ).getType() < 140 ) checkPath[i].isWall = true;

    }
}

void Eagle::calculateShortestPath( const int& newIndexTarget )
{
    if( newIndexTarget != indexTarget ){
        indexTarget =  newIndexTarget;

        resetCheckPath();
        func::calculateDistance( indexTarget, checkPath );
        func::calculateFlowDirection( checkPath );
    }
}

void Eagle::moveToTarget()
{
    int indexCurrent = ( Map.getIndexInMap( collision ) );

    if( collision.x/TILE_SIZE == (collision.x+collision.w)/TILE_SIZE && collision.y/TILE_SIZE == (collision.y+collision.h)/TILE_SIZE ){
        if( distanceToPlayer <= 48 ){
            if( !hurting ) attacking = true;
            x_vel = 0;
            y_vel = 0;
        }
        else{
            attacking = false;
            x_vel = checkPath[indexCurrent].flowDirection_X * E_SPEED;
            y_vel = checkPath[indexCurrent].flowDirection_Y * E_SPEED;
        }

        if( x_vel > 0 ) flip = SDL_FLIP_HORIZONTAL;
        else if( x_vel < 0 ) flip = SDL_FLIP_NONE;
        else if( !attacking ) {
            if( indexCurrent%MAP_WIDTH < indexTarget%MAP_WIDTH ) flip = SDL_FLIP_HORIZONTAL;
            else flip = SDL_FLIP_NONE;
        }
    }
}

void Eagle::autoMove()
{
    if( indexTarget != index_origin ) {
        flying = true;
        indexTarget = index_origin;

        resetCheckPath();
        func::calculateDistance( indexTarget, checkPath );
        func::calculateFlowDirection( checkPath );
    }
    moveToTarget();
}

void Eagle::handleStatus( const Player& _player )
{
    if( getCollision().x > _player.getCollision().x ) flip = SDL_FLIP_NONE;
    else flip = SDL_FLIP_HORIZONTAL;

    if( HP <= 0 && !hurting ){
        die = true;
        flying = false;
        attacking = false;
        return;
    }

    if( attacking ) flying = false;
    else flying = true;

    if( hurting ){
        countHurt++;
        if( countHurt >= 12 ){
            hurting = false;
            x_vel *= -1;
        }
    }
    else countHurt = 0;
}

void Eagle::beingHit( const Player& _player )
{
    if( func::checkCollision( collision, _player.getCollision() )
        && _player.check_attack() && !hurting
        && ( (_player.getFlip()==SDL_FLIP_NONE && _player.getCollision().x<=collision.x) || (_player.getFlip()==SDL_FLIP_HORIZONTAL && _player.getCollision().x>=collision.x) ) ){
        Mix_PlayChannel( -1, sfx_eagle, 0 );
        hurting = true;
        attacking = false;
        if( flip == SDL_FLIP_NONE ) x_vel = 4;
        else x_vel = -4;
        y_vel = -2;
        HP--;
    }
}

void Eagle::update( const Player& _player)
{
    if( die ) return;

    int centerX = getCollision().x + getCollision().w/2;
    int centerY = getCollision().y + getCollision().h/2;
    int p_centerX = _player.getCollision().x + _player.getCollision().w/2;
    int p_centerY = _player.getCollision().y + _player.getCollision().h/2;
    distanceToPlayer = sqrt( (centerX - p_centerX)*(centerX - p_centerX) + (centerY - p_centerY)*(centerY - p_centerY) );

    if( distanceToPlayer > 8*TILE_SIZE || Map.getStart_x() != _player.getStartMap() ) {
        autoMove();
    }
    else{
        handleStatus( _player );
        if( !hurting ){
            calculateShortestPath( Map.getIndexInMap( _player.getCollision() ) );
            moveToTarget();
            beingHit( _player );
        }
    }

    x += x_vel;
    collision.x = x - Map.getStart_x()+4;
    if( func::checkWall( collision, Map ) ){
        x -= x_vel;
        collision.x = x - Map.getStart_x()+4;
    }

    y += y_vel;
    collision.y = y+16;
    if( func::checkWall( collision, Map ) ){
        y -= y_vel;
        collision.y = y+16;
    }
}

void Eagle::render( const SDL_Rect& camera )
{
    SDL_Rect dst = { x-camera.x, y-camera.y, 64, 64 };

    if( flying ){
        if( (countFly+1)/8 >= FLY_FRAMES ) countFly = 0;
        countFly++;
        func::renderTextureFlip(texture, &animationFly[countFly/8], &dst, flip );
    }
    else countFly=0;

    if( attacking ){
        if( (countHit+1)/6 >= FLY_FRAMES ) countHit = 0;
        countHit++;
        func::renderTextureFlip(texture, &animationFly[countHit/6], &dst, flip );
    }
    else countHit = 0;

    if( die ){
        if( (countDie+1)/6 >= DIE_FRAMES ) return;
        countDie++;
        func::renderTextureFlip( texture, &animationDie[countDie/6], &dst, flip );
    }
    else countDie = 0;
}
