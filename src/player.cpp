#include "player.h"

Player::Player( int _x, int _y, SDL_Texture* image[], Mix_Chunk* sfx[] ) : baseObject( _x, _y )
{
    collision = { _x, _y, P_WIDTH, P_HEIGHT };

    p_texture[jump] = image[jump];
    p_texture[attack] = image[attack];
    p_texture[hurt] = image[hurt];
    p_texture[dead] = image[dead];
    p_texture[idle] = image[idle];
    p_texture[walk] = image[walk];

    p_sfx[attack] = sfx[attack];
    p_sfx[jump] = sfx[jump];
    p_sfx[fall] = sfx[fall];
    p_sfx[hurt] = sfx[hurt];
    p_sfx[dead] = sfx[dead];

    for( int i=0 ; i<FALLING_FRAMES ; i++) {
        animationFalling[i] = {512 + i*64, 0, 64, 64};
    }
    for( int i=0 ; i<32 ; i++){
        animationPlayer[i] = {i*64, 0, 64, 64};
    }
}

void Player::updatePlayer( std::deque<gameMap>& listM, std::vector<Enemy*>& enemyList, std::vector<Eagle*>& eagleList )
{
    mapPresent( listM );

    if( !die ) {
        handleStatus();
        beingHit( enemyList, eagleList );
        handleCollision( listM );
    }

    if( y > MAP_HEIGHT*64 +128 && !die ){
        die = true;
        countDead = 6*24;
        Mix_PlayChannel( -1, p_sfx[dead], 0);
    }
}

void Player::handleEvent(SDL_Event &e )
{
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
        switch( e.key.keysym.sym ){
        case SDLK_d:
            x_vel += P_SPEED;
            break;
        case SDLK_a:
            x_vel -= P_SPEED;
            break;
        case SDLK_w:
        case SDLK_SPACE:
            Jump();
            grounded = false;
            break;
        case SDLK_j:
            if( !hurting && !die ) {
                attacking = true;
                Mix_PlayChannel( -1, p_sfx[attack], 0 );
            }
            break;
        }
    }
    if( e.type == SDL_KEYUP && e.key.repeat == 0 ){
        switch( e.key.keysym.sym ){
        case SDLK_d:
            x_vel -= P_SPEED;
            break;
        case SDLK_a:
            x_vel += P_SPEED;
            break;
        }
    }
}

void Player::handleStatus()
{
    if( HP <= 0 && !hurting ){
        die = true;
        Mix_PlayChannel(-1, p_sfx[dead], 0);
    }

    if( !die && !attacking && !hurting && grounded && x_vel != 0 ) walking = true;
    else walking = false;
    if( !die && !hurting && grounded && x_vel == 0 ) idling = true;
    else idling = false;
    if( !die && !hurting && !grounded && y_vel >= 0 ) falling = true;
    else falling = false;
    if( !die && !hurting && !grounded && y_vel < 0 ) jumping = true;
    else jumping = false;

    if( hurting ) attacking = false;

    if( x_vel < 0 ) flip = SDL_FLIP_HORIZONTAL;
    else if( x_vel > 0 ) flip = SDL_FLIP_NONE;

}

void Player::handleCollision( std::deque<gameMap>& listM )
{
    x += x_vel;
    collision.x = x - listM[index_mapP].getStart_x()+36;
    if( func::checkWall( getCollision(), listM[index_mapP] ) || (attacking && grounded) || x+36<listM[0].getStart_x()  ){
        x -= x_vel;
        collision.x = getX() - listM[index_mapP].getStart_x()+36;
    }
     else if( getCollision().x + getCollision().w >= 64*MAP_WIDTH ){
        if( func::checkWall(getCollision(), listM[index_mapP], listM[index_mapP+1] ) ){
            x -= x_vel;
            collision.x = x-listM[index_mapP].getStart_x()+36;
        }
    }

    y_vel += gForce;
    y += y_vel;
    collision.y = y+36;
    if( func::checkWall( getCollision(), listM[index_mapP], &grounded ) ){
        if( y_vel > 0 && falling ){
            grounded = true;
            Mix_PlayChannel( -1, p_sfx[fall], 0 );
        }
        y -= y_vel;
        y_vel = 0;
        collision.y = y+36;
    }
    if( getCollision().x + getCollision().w >= 64*MAP_WIDTH ){
        if( func::checkWall(getCollision(), listM[index_mapP], listM[index_mapP+1], &grounded ) ){
            y -= y_vel;
            y_vel = 0;
            collision.y = y+36;
        }
    }
}

void Player::beingHit( std::vector<Enemy*>& enemyList, std::vector<Eagle*>& eagleList )
{
    for( int i=0 ; i<enemyList.size() ; i++){
        if( enemyList[i]->getStartMap() == getStartMap() && enemyList[i] != nullptr ){
            if( enemyList[i]->getDistance() <= TILE_SIZE && enemyList[i]->isAttack() && !hurting ){
                hurting = true;
                y_vel = -6;
                HP--;
                Mix_PlayChannel(-1, p_sfx[hurt], 0 );
            }
        }
    }

    for( int i=0 ; i<eagleList.size() ; i++){
        if( eagleList[i]->getStartMap() == getStartMap() && eagleList[i] != nullptr ){
            if( eagleList[i]->getDistance() <= 48 && eagleList[i]->isAttack() && !hurting ){
                hurting = true;
                y_vel = -6;
                HP--;
                Mix_PlayChannel(-1, p_sfx[hurt], 0 );
            }
        }
    }
}

void Player::Jump()
{
    if( grounded ) {
        y_vel = -17;
        jumping = true;
        Mix_PlayChannel( -1, p_sfx[jump], 0 );
    }
}

void Player::mapPresent( std::deque<gameMap>& listM )
{
    if( getCollision().x >= MAP_WIDTH*64 && index_mapP<2 ){
        index_mapP++;
    }
    else if( getCollision().x < 0 && index_mapP>0 ){
        index_mapP--;
    }
    startX_mapP = listM[index_mapP].getStart_x();
}

// Hàm lerp
int lerp(int a, int b, float t)
{
    return (1-t)*a + t*b;
}
void Player::changeCam( SDL_Rect& camera, std::deque<gameMap>& listM )
{
    // Tính toán vị trí camera target
    int cam_target_x = x - (float)1/2*WIDTH;
    int cam_target_y = y - (float)1/3*HEIGHT;

    // Giới hạn camera target theo kích thước map
    if( cam_target_x < listM[index_mapP].getStart_x() && index_mapP == 0 ) {
        cam_target_x = listM[index_mapP].getStart_x();
    }
    if( cam_target_y < 0.5*TILE_SIZE ) {
        cam_target_y = 0.5*TILE_SIZE;
    }
    else if( cam_target_y > 64*MAP_HEIGHT - HEIGHT - 0.5*TILE_SIZE ) {
        cam_target_y = 64*MAP_HEIGHT - HEIGHT - 0.5*TILE_SIZE;
    }
    if( hurting && countHurt < 18 ){
        cam_target_x += rand() % 51 - 25;
        cam_target_y += rand() % 51 - 25;
    }


    // Áp dụng smoothing bằng cách sử dụng lerp
    float smoothing = 0.1; // Điều chỉnh độ mượt của camera, có thể thay đổi
    camera.x = lerp(camera.x, cam_target_x, smoothing);
    camera.y = lerp(camera.y, cam_target_y, smoothing);
}

void Player::renderPlayer( SDL_Rect& camera )
{

    SDL_Rect str = {x-camera.x, y-camera.y, 128, 128};

    if( attacking ){
        if( (countHit+2)/6>= ATTACKING_FRAMES ) attacking = false;
        countHit++;
        func::renderTextureFlip(p_texture[attack], &animationPlayer[countHit/6], &str, flip );
    }
    else{
        countHit = 0;
        if( hurting ){
            if( (countHurt+2)/6 >= HURTING_FRAMES ) hurting = false;
            countHurt++;
            func::renderTextureFlip(p_texture[hurt], &animationPlayer[countHurt/6], &str, flip );
        }
        else countHurt=0;

        if( jumping ){
            if( (countJump+2)/6 >= JUMPING_FRAMES ) countJump = 0;
            countJump++;
            func::renderTextureFlip(p_texture[jump], &animationPlayer[countJump/6], &str, flip );
        }
        else countJump=0;

        if( falling ){
            if( (countFall+1)/6 >= FALLING_FRAMES ) countFall = 0;
            countFall++;
            func::renderTextureFlip( p_texture[jump], &animationFalling[countFall/6], &str, flip );
        }
        else countFall=0;

        if( walking ){
            if( (countWalk+1)/6 >= WALKING_FRAMES ) countWalk = 0;
            countWalk++;
            func::renderTextureFlip(p_texture[walk], &animationPlayer[countWalk/6], &str, flip );
        }
        else countWalk=0;

        if( idling ){
            if( (countIdle+1)/8 >= IDLING_FRAMES ) countIdle = 0;
            countIdle++;
            func::renderTextureFlip(p_texture[idle], &animationPlayer[countIdle/8], &str, flip );
        }
        else countIdle=0;

        if( die ){
            if( (countDead+1)/6 >= DIE_FRAMES ) Mix_HaltChannel( -1 );
            countDead++;
            func::renderTextureFlip(p_texture[dead], &animationPlayer[countDead/6], &str, flip );
        }
        else countDead = 0;
    }
}

void Player::resetPlayer()
{
    die = false;
    x = 64;
    y = 128;
    y_vel = 0;
    HP = 3;
}
