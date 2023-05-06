#include "func.h"

bool func::initWindow()
{
    window = SDL_CreateWindow( "Where's Gnuol !?", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if( window == nullptr ){
        std::cout << "Error init Window: " << SDL_GetError() << std::endl;
        return false;
    }
    else {
        renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
        if( renderer == nullptr ){
            std::cout << "Error init Renderer: " << SDL_GetError() << std::endl;
            return false;
        }
        else{
            SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
            SDL_RenderClear( renderer );
        }
    }
    return true;
}

void func::clearRender()
{
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
    SDL_RenderClear( renderer );
}

SDL_Texture* func::loadTexture( std::string path )
{
    SDL_Texture* texture = nullptr;
    texture = IMG_LoadTexture( renderer, path.c_str() );
    if( texture == nullptr ){
        std::cout << "Error load texture: " << SDL_GetError() << std::endl;
    }
    std::cout << "Success load texture: " << path << std::endl;
    return texture;
}

void func::renderTexture( SDL_Texture* image, SDL_Rect* src, SDL_Rect* dst )
{
    SDL_RenderCopy( renderer, image, src, dst );
}

void func::renderTexture( SDL_Texture* image, SDL_Rect* src, int _x, int _y, int _w, int _h )
{
    SDL_Rect dst = { _x, _y, _w, _h };
    SDL_RenderCopy( renderer, image, src, &dst );
}

void func::renderTextureFlip( SDL_Texture* image, SDL_Rect* src, SDL_Rect* dst, SDL_RendererFlip flip )
{
    SDL_RenderCopyEx( renderer, image, src, dst, 0, nullptr, flip );
}

void func::renderPresent()
{
    SDL_RenderPresent(renderer);
}

void func::destroyAll()
{
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    renderer = nullptr;
    window = nullptr;

    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
}

bool func::initFont( const char* path )
{
	TTF_CloseFont(font);
	font = TTF_OpenFont( path, 24);
	if (font == NULL) {
		std::cout << "Error init font: " << TTF_GetError();
		return false;
	}
	return true;
}

SDL_Texture* func::createText( std::string text, SDL_Color color )
{
    SDL_Texture* texture = nullptr;
    SDL_Surface* textSur = TTF_RenderText_Solid( font, text.c_str(), color );

    if( textSur == nullptr ) TTF_GetError();
    texture = SDL_CreateTextureFromSurface( renderer, textSur );
    if( texture == nullptr ) SDL_GetError();
    SDL_FreeSurface( textSur );

    return texture;
}

void func::calculateDistance( int indexTarget, std::vector<path_pos>& checkPath )
{
    checkPath[indexTarget].flowDistance = 0;

    std::queue<int> listIndexToCheck;
    int indexCurrent, neighborX, neighborY;

    listIndexToCheck.push( indexTarget );
    while( !listIndexToCheck.empty() ){
        indexCurrent = listIndexToCheck.front();
        listIndexToCheck.pop();

        for( int i=0 ; i<4 ; i++){
            neighborX = neighbors[i][0] + indexCurrent%MAP_WIDTH;
            neighborY = neighbors[i][1] + indexCurrent/MAP_WIDTH;

            if( (neighborX >= 0 && neighborX < MAP_WIDTH) && (neighborY >= 0 && neighborY < MAP_HEIGHT) ){
                int indexNeighbor = neighborX + neighborY*MAP_WIDTH;
                if( checkPath[indexNeighbor].flowDistance == flowDistanceMax && !checkPath[indexNeighbor].isWall ){
                    checkPath[indexNeighbor].flowDistance = checkPath[indexCurrent].flowDistance + 1;
                    listIndexToCheck.push( indexNeighbor );
                }
            }
        }
    }
}

void func::calculateFlowDirection( std::vector<path_pos>& checkPath )
{
    int flowFieldBest, neighborX, neighborY;

    for( int indexCurrent = 0 ; indexCurrent < checkPath.size() ; indexCurrent++){
        flowFieldBest = checkPath[indexCurrent].flowDistance;

        if( checkPath[indexCurrent].flowDistance != flowDistanceMax ){
            for( int i=0 ; i<8 ; i++){
                neighborX = neighbors[i][0] + indexCurrent%MAP_WIDTH;
                neighborY = neighbors[i][1] + indexCurrent/MAP_WIDTH;

                if( (neighborX >= 0 && neighborX < MAP_WIDTH) && (neighborY >= 0 && neighborY < MAP_HEIGHT) ){
                    int indexNeighbor = neighborX + neighborY*MAP_WIDTH;

                    if( checkPath[indexNeighbor].flowDistance < flowFieldBest ){
                        if( neighbors[i][0] == 0 || neighbors[i][1] == 0 || (!checkPath[indexCurrent+neighbors[i][0]].isWall && !checkPath[indexCurrent+MAP_WIDTH*neighbors[i][1]].isWall) ){
                            flowFieldBest = checkPath[indexNeighbor].flowDistance;
                            checkPath[indexCurrent].flowDirection_X = neighbors[i][0];
                            checkPath[indexCurrent].flowDirection_Y = neighbors[i][1];
                        }
                    }
                }
            }
        }
    }
}

bool func::checkCollision( SDL_Rect object_1, SDL_Rect object_2 )
{
    if( object_1.x+object_1.w < object_2.x ) return false;
    if( object_2.x+object_2.w < object_1.x ) return false;

    if( object_1.y+object_1.h < object_2.y ) return false;
    if( object_2.y+object_2.h < object_1.y ) return false;

    return true;
}

/*
 ABCD cua ham check cham tuong tuong ung voi 4 dinh cua hinh chu nhat nhu sau

                AC****BC
                *    *
                *    *
                AD****BD
*/

bool func::checkWall( SDL_Rect object, gameMap Map, bool* grounded )
{
    int A = (object.x)/TILE_SIZE;
    int B = A+1;
    int C = object.y/TILE_SIZE;
    int D = C+1;

    if( D >= MAP_HEIGHT ) return false;

    if( grounded != nullptr ){
        if( Map.getDataMap(A, D).getType() > 140 && Map.getDataMap(B, D).getType() > 140 ) *grounded = false;
        else if( Map.getDataMap(A, D).getType() > 140 && object.x+object.w < Map.getDataMap(B, D).getX() ) *grounded = false;
    }

    if( Map.getDataMap(A, C).getType() < 140 ){
        if( func::checkCollision( object, Map.getDataMap(A, C).getCollision()) ) return true;
    }
    if( Map.getDataMap(B, C).getType() < 140 ){
        if( func::checkCollision( object, Map.getDataMap(B, C).getCollision()) ) return true;
    }
    if( Map.getDataMap(A, D).getType() < 140 ){
        if( func::checkCollision( object, Map.getDataMap(A, D).getCollision()) ) return true;
    }
    if( Map.getDataMap(B, D).getType() < 140 ){
        if( func::checkCollision( object, Map.getDataMap(B, D).getCollision()) ) return true;
    }

    return false;
}

bool func::checkWall( SDL_Rect object, gameMap Map1, gameMap Map2, bool* grounded )
{
    bool check = false;
    int A = MAP_WIDTH-1;
    int B = 0;
    int C = object.y/TILE_SIZE;
    int D = C+1;

    if( D >= MAP_HEIGHT ) return false;

    if( grounded != nullptr ){
        if( Map1.getDataMap(A, D).getType() > 140 && Map2.getDataMap(B, D).getType() > 140 && grounded != nullptr ) *grounded = false;
        else if( Map1.getDataMap(A, D).getType() > 140 && object.x+object.w <= Map2.getDataMap(B, D).getX() ) *grounded = false;
    }

    if( Map1.getDataMap(A, C).getType() < 140 ){
        if( func::checkCollision( object, Map1.getDataMap(A, C).getCollision()) ) return true;
    }
    if( Map1.getDataMap(A, D).getType() < 140 ){
        if( func::checkCollision( object, Map1.getDataMap(A, D).getCollision()) ) return true;
    }

    object.x -= 64*MAP_WIDTH;
    if( Map2.getDataMap(B, C).getType() < 140 ){
        if( func::checkCollision( object, Map2.getDataMap(B, C).getCollision()) ) check = true;
    }
    if( Map2.getDataMap(B, D).getType() < 140 ){
        if( func::checkCollision( object, Map2.getDataMap(B, D).getCollision()) ) check = true;
    }

    return check;
}

void func::setWindowFScr()
{
    SDL_SetWindowFullscreen( window, SDL_WINDOW_FULLSCREEN ); 
}

void func::quitWindowFScr() { SDL_SetWindowFullscreen( window, 0 ); }

void func::handleQuit()
{
    clearRender();
    destroyAll();
}
