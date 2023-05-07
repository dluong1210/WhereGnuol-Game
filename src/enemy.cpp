#include "enemy.h"

Enemy::Enemy( const int& _x, const int& _y, SDL_Texture* image, gameMap& _map, int _type ) : baseObject( _x, _y ){
    origin_x = _x ;
    x_vel = E_SPEED/2;
    type = _type;

    texture = image;
    Map = _map;
    collision = { x, y+16, E_WIDTH, E_HEIGHT };

    for( int i=0 ; i<WALK_FRAMES ; i++){
        animationWalk[i] = {i*32, 0, 32, 32};
    }
    for( int i=0 ; i<FALL_FRAMES ; i++){
        animationFall[i] = {i*32, 32, 32, 32};
    }
    for( int i=0 ; i<HURT_FRAMES ; i++){
        animationHurt[i] = {i*32, 96, 32, 32};
    }
    for( int i=0 ; i<ATTACK_FRAMES ; i++){
        animationAttack[i] = {i*32, 64, 32, 32};
    }
    for( int i=0 ; i<DIE_FRAMES ; i++){
        if( i<2 ) animationDie[i] = {i*32 + 128, 96, 32, 32};
        else animationDie[i] = {i*32 - 64, 128, 32, 32};
    }

    if( type == 0 ) HP = 3;
    else if( type == 1 ) HP = 5;
    else HP = 2;
}

void Enemy::moveToPlayer( Player& _player ){
    attacking = false;
    if( getCollision().x < _player.getCollision().x ) x_vel = E_SPEED;
    else if ( getCollision().x > _player.getCollision().x ) x_vel = -E_SPEED;
    else attacking = true;
}

bool Enemy::enableToMove( Player& _player )
{
    if( _player.isDead() ) return false;
    if( getCollision().x > _player.getCollision().x ){
        for( int i = Map.getIndexInMap( _player.getCollision() )%MAP_WIDTH ; i<=Map.getIndexInMap(collision)%MAP_WIDTH ; i++){
            if( Map.getDataMap(i, y/TILE_SIZE+1).getType()<140 ) return false;
            if( Map.getDataMap(i, y/TILE_SIZE+2).getType() > 140 )
            for( int j=y/TILE_SIZE+1 ; j<=MAP_HEIGHT ; j++){
                if( j == MAP_HEIGHT ) return false;
                if( Map.getDataMap(i, j).getType() < 140 ) break;
            }
        }
        return true;
    }
    else{
        for( int i = Map.getIndexInMap( _player.getCollision() )%MAP_WIDTH ; i>=Map.getIndexInMap(getCollision())%MAP_WIDTH ; i--){
            if( Map.getDataMap(i, y/TILE_SIZE+1).getType()<140 ) return false;
            if( Map.getDataMap(i, y/TILE_SIZE+2).getType() > 140 )
            for( int j=y/TILE_SIZE+1 ; j<=MAP_HEIGHT ; j++){
                if( j == MAP_HEIGHT ) return false;
                if( Map.getDataMap(i, j).getType() < 140 ) break;
            }
        }
        return true;
    }
}

bool Enemy::enableToMove()
{
    if( !grounded ) return true;
    int check_x = Map.getIndexInMap( getCollision() )%MAP_WIDTH;
    int check_y = Map.getIndexInMap( getCollision() )/MAP_WIDTH;

    if( Map.getDataMap( check_x, check_y+2 ).getType() >140 ){
        for( int i=check_y+1 ; i<MAP_HEIGHT ; i++){
            if( Map.getDataMap( check_x, i ).getType() < 140 ) return true;
        }
        return false;
    }
    return true;
}

void Enemy::autoMove( Player& _player ){
    distanceToPlayer = abs( getCollision().x - _player.getCollision().x ); // Tính khoảng cách theo trục X

    if( _player.getStartMap() != Map.getStart_x() || distanceToPlayer > 4*TILE_SIZE || !enableToMove(_player) ){
        if( attacking ) attacking = false;
        if( x > origin_x && x-origin_x >= 3*TILE_SIZE ) x_vel = -E_SPEED/2;
        if( x < origin_x && origin_x-x >= 3*TILE_SIZE ) x_vel = E_SPEED/2;
    }

    else if( distanceToPlayer <= 4*TILE_SIZE ){
        if( getCollision().x < _player.getCollision().x ) flip = SDL_FLIP_NONE;
        else flip = SDL_FLIP_HORIZONTAL;

        if( abs(y - _player.getY()) > 32 ){
            if( x_vel == 0 ) moveToPlayer( _player);
            attacking = false;
            if( distanceToPlayer >= 96 ){
                if( getCollision().x > _player.getCollision().x ) x_vel = -E_SPEED;
                else x_vel = E_SPEED;
            }
        }
        else if( !hurting ){
            if( distanceToPlayer <= 40 ){
                x_vel = 0;
                attacking = true;
            }
            else moveToPlayer( _player );
        }
    }
}

void Enemy::updateEnemy( Player& _player){
//    Cập nhật trạng thái
    if( (HP <= 0 && !falling) || y > 64*MAP_HEIGHT ){
        die = true;
        walking = false;
        attacking = false;
        return;
    }
    if( !die ){
        if( x_vel != 0 && grounded ) walking = true;
        else walking = false;
        if( y_vel >= 0 && !grounded ) falling = true;
        else falling = false;

        if( func::checkCollision( getCollision(), _player.getCollision() ) && _player.check_attack() && flip!=_player.getFlip() ){
            hurting = true;
            attacking = false;
        }
    }
//    Cập nhật tốc độ
    autoMove( _player );

    if( hurting && countHurt == 0 ){
        y_vel -= 8;
        if( getCollision().x < _player.getCollision().x ) x_vel = -4;
        else x_vel = 4;
    }

    if( x_vel < 0 && !hurting ) flip = SDL_FLIP_HORIZONTAL;
    else if( x_vel > 0 && !hurting ) flip = SDL_FLIP_NONE;

//    Cập nhật tọa độ ( vị trí )
//    Theo X
    if( !falling ) x += x_vel;
    collision.x = x-Map.getStart_x();
    if( func::checkWall( getCollision(), Map ) || !enableToMove() || x>=Map.getStart_x()+64*MAP_WIDTH || x<=Map.getStart_x() ){
        x -= x_vel;
        origin_x = x - 3*x_vel/abs(x_vel)*TILE_SIZE;
        x_vel *= -1;
        collision.x = x-Map.getStart_x();
    }
//    Theo Y
    y_vel += gForce;
    y += y_vel;
    collision.y = y+16;
    if( func::checkWall( getCollision(), Map, &grounded ) ){
        if( y_vel > 0 && falling ) grounded = true;
        y -= y_vel;
        y_vel = 0;
        collision.y = y+16;
    }
}

void Enemy::render( SDL_Rect& camera ){

    SDL_Rect dst = { x-camera.x, y-camera.y, 64, 64 };

    if( walking ){
        if( (countWalk+1)/6 >= WALK_FRAMES ) countWalk = 0;
        countWalk++;
        func::renderTextureFlip(texture, &animationWalk[countWalk/6], &dst, flip );
    }
    else countWalk=0;

    if( falling ){
        if( (countFall+1)/6 >= FALL_FRAMES ) countFall = 0;
        countFall++;
        func::renderTextureFlip(texture, &animationFall[countFall/6], &dst, flip );
    }
    else countFall = 0;

    if( attacking ){
        if( (countHit+1)/6 >= ATTACK_FRAMES ) countHit = 0;
        countHit++;
        func::renderTextureFlip(texture, &animationAttack[countHit/6], &dst, flip );
    }
    else countHit = 0;

    if( hurting ){
        if( (countHurt+1)/6 >= HURT_FRAMES ){
            hurting = false;
            HP--;
        }
        countHurt++;
        func::renderTextureFlip(texture, &animationHurt[countHurt/6], &dst, flip );
    }
    else countHurt = 0;

    if( die ){
        if( (countDie+1)/8 >= DIE_FRAMES ) return;
        countDie++;
        func::renderTextureFlip(texture, &animationDie[countDie/8], &dst, flip );
    }
    else countDie = 0;
}
