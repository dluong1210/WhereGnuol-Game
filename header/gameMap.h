#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <SDL.h>
#include <SDL_image.h>
#include <fstream>
#include <vector>

#include "baseObject.h"
#include "func.h"

class tile : public baseObject
{
private:
    int tileType;

public:
    tile( int _x, int _y, int _type );
    void setCollisionH( int _h );

    int getType() const;
};

class gameMap
{
private:
    int start_x, start_y, STT;
    std::vector<std::vector<tile>> Tile;
    std::vector<int> enemyList;
    std::vector<int> eagleList;

    SDL_Texture* tileSet;
public:
    gameMap(){;}
    gameMap( int s_x, const char* path, SDL_Texture* _tileSet, const int& _STT );
    void clearMap();

    void setStart_x( int _x );
    void setMapTexture( SDL_Texture* _Map );
    void setEnemyList( std::vector<int> _enemyList );
    void setEagleList( std::vector<int> _eagleList );
    void loadMap( const char* path, int _STT );
    void renderMap( std::vector<SDL_Rect> clipTile, SDL_Rect& camera );
    void fillEmptyR( std::vector<SDL_Rect> clipTile, SDL_Rect& camera );
    void fillEmptyL( std::vector<SDL_Rect> clipTile, SDL_Rect& camera );

    int getStart_x() const { return start_x; }
    int getStart_y() const { return start_y; }
    int getSTT() const { return STT; }
    std::vector<int> getEnemyList() { return enemyList; }
    std::vector<int> getEagleList() { return eagleList; }
    int getIndexInMap( SDL_Rect objectInMap ) const { return (objectInMap.y/TILE_SIZE)*MAP_WIDTH + objectInMap.x/TILE_SIZE; }
    tile getDataMap( int _x, int _y) const { return Tile[_y][_x]; }

};
#endif // GAMEMAP_H
