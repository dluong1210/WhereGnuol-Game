#include "Game.h"

int main( int argc, char* argv[] )
{
    SDL_Event e;
    Game mainGame;
    if( !mainGame.init() ) return 0;
    else{
        if( !mainGame.loadMedia() || !mainGame.loadMap() ) return 0;
        else{
            mainGame.setTileClip();
            if( !mainGame.createMap() || !mainGame.createPlayer() || !mainGame.createEnemy() || !mainGame.createEagle() ) return 0;
            else{
                while( mainGame.isRunning() ){
                    SDL_PollEvent( &e );
                    mainGame.runGame( e );
                }
            }
        }
    }
    mainGame.clearMedia();
    func::destroyAll();
    return 0;
}
