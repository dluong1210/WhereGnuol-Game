#include "Game.h"

 bool Game::init()
{
    if( SDL_Init( SDL_INIT_EVERYTHING) < 0 ){
        SDL_GetError();
        return false;
    }
    if( !IMG_Init( IMG_INIT_JPG) ){
        IMG_GetError();
        return false;
    }
    if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 ){
        Mix_GetError();
        return false;
    }
    if( TTF_Init() == -1 ){
        TTF_GetError();
        return false;
    }
    if( !func::initWindow() ) return false;

    std::cout << "SUCCESS INIT SDL2 AND WINDOW GAME" << std::endl;
    return true;
}

bool Game::loadMedia()
{
    bool check = true;
    tileSet = func::loadTexture( "src/Texture/TileSet.png" );

    playerTex[0] = func::loadTexture( "src/Texture/Player/Jump.png" );
    playerTex[2] = func::loadTexture( "src/Texture/Player/Attack.png" );
    playerTex[3] = func::loadTexture( "src/Texture/Player/Hurt.png" );
    playerTex[4] = func::loadTexture( "src/Texture/Player/Death.png" );
    playerTex[5] = func::loadTexture( "src/Texture/Player/Idle.png" );
    playerTex[6] = func::loadTexture( "src/Texture/Player/Walk.png" );

    enemyTex[0] = func::loadTexture( "src/Texture/Enemy/enemyPink.png");
    enemyTex[1] = func::loadTexture( "src/Texture/Enemy/enemyWhite.png");
    enemyTex[2] = func::loadTexture( "src/Texture/Enemy/enemyBlue.png");
    eagleTex = func::loadTexture( "src/Texture/Enemy/eagle.png" );

    menuTex[0] = func::loadTexture( "src/Texture/UI/Background.png" );
    menuTex[1] = func::loadTexture( "src/Texture/UI/Mark.png" );
    menuTex[2] = func::loadTexture( "src/Texture/UI/BTN PLAY.png" );
    menuTex[3] = func::loadTexture( "src/Texture/UI/BTN SETTINGS1.png" );
    menuTex[4] = func::loadTexture( "src/Texture/UI/BTN Exit.png" );
    menuTex[5] = func::loadTexture( "src/Texture/UI/BG PAUSE.png" );
    menuTex[6] = func::loadTexture( "src/Texture/UI/BTN SETTINGS2.png" );
    menuTex[7] = func::loadTexture( "src/Texture/UI/BTN MENU.png" );
    menuTex[8] = func::loadTexture( "src/Texture/UI/BTN BACK.png" );
    menuTex[9] = func::loadTexture( "src/Texture/UI/BG END.png" );
    menuTex[10] = func::loadTexture( "src/Texture/UI/BTN Retry.png" );
    menuTex[11] = func::loadTexture( "src/Texture/UI/BG SETTING.png" );
    menuTex[12] = func::loadTexture( "src/Texture/UI/BTN OK.png" );
    menuTex[13] = func::loadTexture( "src/Texture/UI/Arrow.png" );
    menuTex[14] = func::loadTexture( "src/Texture/UI/checkbox.png" );

    liveBar = func::loadTexture( "src/Texture/UI/Live Bar.png" );
    heart = func::loadTexture( "src/Texture/UI/heart.png" );
    HP_pos = { { 20, 20, 36, 28 }, { 44, 20, 36, 28 }, { 68, 20, 36, 28 } };

    p_sfx[0] = Mix_LoadWAV( "src/Sfx/sfx_Jump.wav" );
    p_sfx[1] = Mix_LoadWAV( "src/Sfx/sfx_Fall.wav" );
    p_sfx[2] = Mix_LoadWAV( "src/Sfx/sfx_Attack_2.wav" );
    p_sfx[3] = Mix_LoadWAV( "src/Sfx/sfx_Hurt.wav" );
    p_sfx[4] = Mix_LoadWAV( "src/Sfx/sfx_Death.wav" );
    for( int i=0 ; i<5 ; i++) if( p_sfx[i] == nullptr ) check = false;

    eagle_sfx = Mix_LoadWAV( "src/Sfx/sfx_Hurt.wav" );
    if( eagle_sfx == nullptr ) check = false;

    menuSfx[0] = Mix_LoadWAV( "src/Sfx/sfx_menu_select.wav" );
    menuSfx[1] = Mix_LoadWAV( "src/Sfx/sfx_menu_click.wav" );
    for( int i=0 ; i<2 ; i++) if( menuSfx[i] == nullptr ) check = false;

    menuMus = Mix_LoadMUS( "src/Sfx/Title Theme.wav" );
    if( menuMus == nullptr ) check = false;
    gameMus = Mix_LoadMUS( "src/Sfx/And The Journey Begins.wav" );
    if( gameMus == nullptr ) check = false;
    Mix_VolumeMusic( 30 );

    func::initFont( "src/Texture/font.ttf" );

    getHightScore();

    menu = new Menu( menuTex, menuSfx );
    if( menu == nullptr ){
        std::cout << "Error init Menu" << std::endl;
        check = false;
    }
    if( check ) std::cout << "Success load Audio, Font and create Menu" << std::endl;
    return check;
}

bool Game::loadMap()
{
    map_enemy Map1( "src/Map/mapTest.map", {5}, {200} );
    totalMap.push_back( Map1 );
    map_enemy Map2( "src/Map/map2.map", {5, 15}, {150} );
    totalMap.push_back( Map2 );
    map_enemy Map3( "src/Map/map2.map", {5, 15, 20}, {100} );
    totalMap.push_back( Map3 );
    return true;
}

void Game::setTileClip()
{
    tileClip.push_back( {0, 0, 0, 0} );

    for( int i=0 ; i<13 ; i++){
        for( int j=0 ; j<20 ; j++){
            tileClip.push_back( {64*j, 64*i, 64, 64} );
        }
    }
}

bool Game::createMap()
{
    for( int i=0 ; i<3 ; i++){
        int random = rand() % totalMap.size();
        random = i;
        gameMap Map( i*MAP_WIDTH*TILE_SIZE, totalMap[random].path, tileSet, random );
        Map.setEnemyList( totalMap[random].enemy_pos );
        Map.setEagleList( totalMap[random].eagle_pos );
        listM.push_back( Map );
    }
    if( listM.size() < 3 ){
        std::cout << " Error load List Map";
        return false;
    }
    return true;
}

bool Game::createPlayer()
{
    player = new Player( 64, 128, playerTex, p_sfx );
    if( player == nullptr ) return false;
    return true;
}

bool Game::createEnemy()
{
    for( int i=0 ; i<listM.size() ; i++){
        for( int j=0 ; j<listM[i].getEnemyList().size() ; j++ ){
            int random = rand() % 3;
            Enemy* enm = new Enemy( listM[i].getEnemyList()[j]*TILE_SIZE + listM[i].getStart_x(), 128, enemyTex[random], listM[i], random );
            enemyList.push_back( enm );
        }
    }
    return true;
}
bool Game::createEagle()
{
    for( int i=0 ; i<listM.size() ; i++){
        for( int j=0 ; j<listM[i].getEagleList().size() ; j++ ){
            Eagle* egl = new Eagle( (listM[i].getEagleList()[j]%MAP_WIDTH)*TILE_SIZE + listM[i].getStart_x(), (listM[i].getEagleList()[j]/MAP_WIDTH)*TILE_SIZE, eagleTex, listM[i] );
            eagleList.push_back( egl );
        }
    }
    return true;
}

void Game::update_Game()
{
    if( updateMap() ){
        for( int j=0 ; j<listM[2].getEnemyList().size() ; j++ ){
            int random = rand() % 3;
            Enemy* enm = new Enemy( listM[2].getEnemyList()[j]*TILE_SIZE + listM[2].getStart_x(), 128, enemyTex[random], listM[2], random );
            enemyList.push_back( enm );
        }
        for( int j=0 ; j<listM[2].getEagleList().size() ; j++ ){
            Eagle* egl = new Eagle( (listM[2].getEagleList()[j]%MAP_WIDTH)*TILE_SIZE + listM[2].getStart_x(), (listM[2].getEagleList()[j]/MAP_WIDTH)*TILE_SIZE, eagleTex, listM[2] );
            eagleList.push_back( egl );
        }
    }

    player->updatePlayer( listM , enemyList, eagleList );
    player->changeCam( camera, listM );
    update_Enemy();
    update_Eagle();
    if( player->isDead() ) setHightScore();
}

bool Game::updateMap()
{
    if( listM.size() != 3 ){
        std::cout << "Error update map";
        return false;
    }

    if( camera.x >= listM[2].getStart_x()-1 ){
        listM[0].clearMap();
        listM.pop_front();

        int randomMap ;
        randomMap = rand()%2;
//        while( totalMap[randomMap].checkRepeat(listM[0]) || totalMap[randomMap].checkRepeat(listM[1]) ){
//            randomMap = std::rand()%totalMap.size()-1;
//        }
//        std::cout << randomMap << std::endl;
        gameMap Map( listM[1].getStart_x() + TILE_SIZE*MAP_WIDTH, totalMap[randomMap].path, tileSet, randomMap );
        Map.setEnemyList( totalMap[randomMap].enemy_pos );
        Map.setEagleList( totalMap[randomMap].eagle_pos );
        listM.push_back( Map );
        return true;
    }
    return false;
}

void Game::update_Enemy()
{
    for( int i=0 ; i<enemyList.size() ; i++){
        if( enemyList[i] != nullptr ){
            enemyList[i]->updateEnemy( *player );
            if( enemyList[i]->isDead() ){
                if( enemyList[i]->getType() == 0 ) player->buffHP();
                else if( enemyList[i]->getType() == 1 ) scoreEnemy += 10;
                else scoreEnemy += 5;

                delete enemyList[i];
                enemyList[i] = nullptr;
                enemyList.erase( enemyList.begin() + i );
                i--;
            }
        }
    }
}

void Game::update_Eagle()
{
    for( int i=0 ; i<eagleList.size() ; i++){
        if( enemyList[i] != nullptr ){
            eagleList[i]->update( *player );
            if( eagleList[i]->isDead() ){
                scoreEnemy += 15;
                delete eagleList[i];
                eagleList[i] = nullptr;
                eagleList.erase( eagleList.begin() + i );
                i--;
            }
        }
    }
}

void Game::getHightScore()
{
    std::string temp;
    std::fstream hiScore( "src/Hight Score.txt", std::fstream::in );
    if( !hiScore ) std::cout << "Error open hight score file" ;
    hiScore >> temp;
    hightScore = std::stoi( temp );
}

void Game::setHightScore()
{
    totalScore = scoreEnemy + scoreRun;
    if( totalScore >= hightScore ){
        std::fstream hiScore( "src/Hight Score.txt", std::fstream::out | std::fstream::trunc );
        hiScore << hightScore;
    }
}

void Game::render_Game()
{
    render_Map();
    render_Enemy();
    render_Eagle();
    player->renderPlayer( camera );
    render_HP_Score();
}

void Game::render_Map()
{
    listM[0].fillEmptyL( tileClip, camera );
    listM[1].renderMap( tileClip, camera );
    listM[2].fillEmptyR( tileClip, camera );
}

void Game::render_Enemy()
{
    for( int i=0 ; i<enemyList.size() ; i++){
        if( enemyList[i] != nullptr ){
            enemyList[i]->render( camera );
        }
    }
}

void Game::render_Eagle()
{
    for( int i=0 ; i<eagleList.size() ; i++){
        if( eagleList[i] != nullptr ){
            eagleList[i]->render( camera );
        }
    }
}


void Game::render_HP_Score()
{
    scoreRun = std::max( scoreRun, player->getX()/TILE_SIZE );
    totalScore = scoreRun + scoreEnemy;
    hightScore = std::max( hightScore, totalScore );

    SDL_Texture* scoreTex = func::createText( "Score: " + std::to_string(totalScore), {0, 0, 0, 255} );
    SDL_Texture* hightScoreTex = func::createText( "Hight Score: " + std::to_string(hightScore), {255, 0, 0, 255} );

    int *w = new int, *h = new int;
    SDL_QueryTexture( scoreTex, nullptr, nullptr, w, h);
    func::renderTexture( scoreTex, nullptr, WIDTH - *w - 10, 10, *w, *h);

    SDL_QueryTexture( hightScoreTex, nullptr, nullptr, w, h);
    func::renderTexture( hightScoreTex, nullptr, WIDTH - *w - 10, 35, *w, *h );

    delete w, h;
    SDL_DestroyTexture( scoreTex );
    SDL_DestroyTexture( hightScoreTex );

    func::renderTexture( liveBar, nullptr, 0, 0, 132, 68 );
    for( int i = 0 ; i < player->getHP() ; i++){
        func::renderTexture( heart, nullptr, &HP_pos[i] );
    }
}

void Game::playMusic()
{
    if( menu->soundOn() ) Mix_Volume( -1, 100 );
    else Mix_Volume(-1, 0 );

    if( menu->musicOn() ){
        if( menu->isRunning() ){
            if( musicStatus != 1 ){
                Mix_HaltMusic();
                Mix_FadeInMusic( menuMus, -1, 1000 );
                musicStatus = 1;
            }
        }
        else if( musicStatus != 2 ) {
            Mix_HaltMusic();
            Mix_FadeInMusic( gameMus, -1, 1000 );
            musicStatus = 2;
        }
    }
    else{
        Mix_HaltMusic();
        musicStatus = 0;
    }
}

void Game::resetGame()
{
    while( !enemyList.empty() ){
        delete enemyList[0];
        enemyList[0] = nullptr;
        enemyList.erase( enemyList.begin() );
    }
    while( !eagleList.empty() ){
        delete eagleList[0];
        eagleList[0] = nullptr;
        eagleList.erase( eagleList.begin() );
    }

    for( int i=0 ; i<listM.size() ; i++){
        listM[i].clearMap();
    }
    if( !listM.empty() ) listM.clear();

    if( !createMap() ) std::cout << "Error reset list Map" ;
    if( !createEagle() ) std::cout << "Error reset Eagle";
    if( !createEnemy() ) std::cout << "Error reset Enemy" ;

    player->resetPlayer();
    camera.x = 0;
    camera.y = 0;

    setHightScore();
    scoreEnemy = 0;
    scoreRun = 0;
    totalScore = 0;
    getHightScore();
}

void Game::handleInputGame( SDL_Event& e )
{
    if( e.type == SDL_QUIT ) func::handleQuit();
    menu->handleInput( e, *player, runningGame );
    if( !menu->isRunning() ){
        player->handleEvent( e );
    }
}

void Game::runGame( SDL_Event& e )
{
    if( !menu->isMenu() ){
        if( !menu->isPause() && !menu->isEnd() ){
            update_Game();
        }
        render_Game();
        if( menu->isEnd() ) menu->renderEndMenu( totalScore );
    }
    else if( runningGame ){
        runningGame = false;
        resetGame();
    }

    playMusic();
    handleInputGame( e );
    if( menu->isRetry() ){
        std::cout << "isRetry" << std::endl;
        resetGame();
    }
    func::renderPresent();
}
