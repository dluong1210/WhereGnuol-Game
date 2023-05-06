#ifndef _FUNC_H
#define _FUNC_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <deque>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#define WIDTH 1280
#define HEIGHT 720

#define MAP_WIDTH 34
#define MAP_HEIGHT 17
#define TILE_SIZE 64
#define TOTAL_TILE 5

#include "baseObject.h"
#include "gameMap.h"

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static TTF_Font* font = nullptr;

const int gForce = 1;

const int flowDistanceMax = 34*17;
const std::vector<std::vector<int>> neighbors = { {0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {1, -1}, {-1, 1}, {1, 1} };


class gameMap;

struct path_pos{
    bool isWall;
    int flowDirection_X, flowDirection_Y, flowDistance;
    path_pos( int _x=0, int _y=0, int dtc=255, bool wall=false){
        flowDirection_X = _x;
        flowDirection_Y = _y;
        flowDistance = dtc;
        isWall = wall;
    }
};

struct map_enemy
{
    const char* path;
    std::vector<int> enemy_pos;
    std::vector<int> eagle_pos;

    map_enemy( const char* _path, std::vector<int> enm, std::vector<int> egl ){
        path = _path;
        enemy_pos = enm;
        eagle_pos = egl;
    }
    const char* getPath() { return path; }
};

namespace func
{
    bool initWindow();
    void clearRender();
    SDL_Texture* loadTexture( std::string path );
    void renderTexture( SDL_Texture* image, SDL_Rect* src, SDL_Rect* dst );
    void renderTexture( SDL_Texture* image, SDL_Rect* src, int _x, int _y, int _w, int _h );
    void renderTextureFlip( SDL_Texture* image, SDL_Rect* src, SDL_Rect* dst, SDL_RendererFlip flip );
    void renderPresent();
    void destroyAll();

    bool initFont( const char* path );
    SDL_Texture* createText( std::string text, SDL_Color color);

    void calculateDistance( int indexTarget, std::vector<path_pos>& checkPath );
    void calculateFlowDirection( std::vector<path_pos>& checkPath );

    bool checkCollision( SDL_Rect object_1, SDL_Rect object_2 );
    bool checkWall( SDL_Rect object, gameMap Map, bool* grounded = nullptr );
    bool checkWall( SDL_Rect object, gameMap Map1, gameMap Map2, bool* grounded = nullptr );

    void setWindowFScr();
    void quitWindowFScr();
    void handleQuit();

}

#endif
