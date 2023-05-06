#include "gameMap.h"

tile::tile( int _x, int _y, int _type ) : baseObject( _x, _y )
{
    tileType = _type;
    collision = {_x, _y, TILE_SIZE, TILE_SIZE };
}
void tile::setCollisionH( int _h )
{
    collision.h = _h;
}

int tile::getType() const { return tileType; }


gameMap::gameMap( int s_x, const char* path, SDL_Texture* _tileSet, const int& _STT)
{
    start_x = s_x;
    start_y = 0;
    STT = _STT;
    tileSet = _tileSet;

    loadMap(path, _STT);
}

void gameMap::clearMap()
{
    Tile.clear();
}

void gameMap::setStart_x( int _x )
{
    start_x = _x;
}

void gameMap::setMapTexture( SDL_Texture* _Map )
{
    tileSet = _Map;
}

void gameMap::setEnemyList( std::vector<int> _enemyList )
{
    enemyList = _enemyList;
}

void gameMap::setEagleList( std::vector<int> _eagleList )
{
    eagleList = _eagleList;
}

void gameMap::loadMap( const char* path, int _STT )
{
    std::ifstream ifs( path );
    if( !ifs )
    {
        std::cout << "Error load data map";
    }

    STT = _STT;
    int x = 0, y = 0, data;
    char temp;

    for( int i=0 ; i<MAP_HEIGHT; i++){
        std::vector<tile> row;
        for( int j=0 ; j<MAP_WIDTH ; j++){
            ifs >> data;
            ifs >> temp;
            tile column( x, y, data );

            if( data > 120 && data < 140 ) column.setCollisionH(24);
            row.push_back(column);
            x += TILE_SIZE;
        }
        Tile.push_back(row);
        x = 0;
        y += TILE_SIZE;
    }
    ifs.close();
}

void gameMap::renderMap( std::vector<SDL_Rect> clipTile, SDL_Rect& camera )
{
    for( int i=camera.y/TILE_SIZE ; i*TILE_SIZE - camera.y < HEIGHT ; i++ ){
        for( int j=(camera.x-getStart_x())/TILE_SIZE ; j< MAP_WIDTH ; j++ ){
            if( j<0 ) continue;
            if( j*TILE_SIZE+getStart_x()-camera.x >= WIDTH ) break;

            SDL_Rect dst = { getDataMap(j, i).getX() + getStart_x() - camera.x, getDataMap(j, i).getY() - camera.y, TILE_SIZE, TILE_SIZE};
            func::renderTexture( tileSet, &clipTile[getDataMap(j, i).getType()], &dst );
        }
    }
}

void gameMap::fillEmptyR( std::vector<SDL_Rect> clipTile, SDL_Rect& camera )
{
    if( camera.x + 64*MAP_WIDTH <= getStart_x() ) return;

    for( int i=camera.y/TILE_SIZE ; i*TILE_SIZE - camera.y < HEIGHT ; i++ ){
        for( int j=0 ; j*64+getStart_x()-camera.x<WIDTH ; j++){
            SDL_Rect dst = { getStart_x()-camera.x+j*64, getDataMap(j, i).getY() - camera.y, TILE_SIZE, TILE_SIZE };
            func::renderTexture( tileSet, &clipTile[getDataMap(j, i).getType()], &dst );
        }
    }
}

void gameMap::fillEmptyL( std::vector<SDL_Rect> clipTile, SDL_Rect& camera )
{
    if( camera.x >= getStart_x() + 64*MAP_WIDTH ) return;

    renderMap( clipTile, camera );
}
