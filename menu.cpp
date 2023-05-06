#include "menu.h"

Menu::Menu( SDL_Texture* MenuTex[], Mix_Chunk* MenuSfx[] )
{
    sfx_menu_1 = MenuSfx[0];
    sfx_menu_2 = MenuSfx[1];
    menuBG = MenuTex[0];
    mark = MenuTex[1];

    playBtn.setButton( MenuTex[2], 540, 330, 200, 48 );
    settingBtn[0].setButton( MenuTex[3], 540, 400, 200, 48 );
    exitBtn.setButton( MenuTex[4], 612, 500, 56, 54);

    pauseBG.setButton( MenuTex[5], 494, 150, 292, 366);
    settingBtn[1].setButton( MenuTex[6], 560, 330, 165, 53 );
    menuBtn.setButton( MenuTex[7], 560, 250, 165, 53 );
    backBtn.setButton( MenuTex[8], 582, 425, 120, 53);

    endBG.setButton( MenuTex[9], 424, 250, 432, 246 );
    retryBtn.setButton( MenuTex[10], 582, 414, 120, 53);

    settingBG.setButton( MenuTex[11], 529, 150, 222, 400);
    okBtn.setButton( MenuTex[12], 600, 450, 86, 37);
    arrow.setButton( MenuTex[13], 0, 0, 12, 21 );
    checkBox.setButton( MenuTex[14], 0, 0, 106, 34 );

    soundTex = func::createText( "Sound", {255, 255, 255, 255} );
    musicTex = func::createText( "Music", {255, 255, 255, 255} );
    fullScrTex = func::createText( "FullScr", {255, 255, 255, 255} );
}

void Menu::handleInput( SDL_Event& e, Player& _player, bool& isRunning  )
{
    if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE ){
        if( paused ) paused = false;
        else paused = true;
    }

    if( _player.isDead() ) endG = true;
    else retry = false;

    if( isEnd() && !isPause() ){
        switch( e.type ){
        case SDL_MOUSEBUTTONDOWN:
            if( checkMouse(*retryBtn.getBtnRect()) ){
                Mix_PlayChannel( -1, sfx_menu_2, 0 );
                retry = true;
                endG = false;
                paused = false;
            }
            break;
        }
    }

    if( isMenu() ){
        paused = false;
        endG = false;
        renderMainMenu();
        if( !isSetting() ){
            switch( e.type ){
            case SDL_MOUSEBUTTONDOWN:
                if( checkMouse(*playBtn.getBtnRect()) ){
                    Mix_PlayChannel( -1, sfx_menu_2, 0 );
                    menu = false;
                    isRunning = true;
                }
                if( checkMouse(*settingBtn[0].getBtnRect()) ){
                    Mix_PlayChannel( -1, sfx_menu_2, 0 );
                    setting = true;
                }
                if( checkMouse(*exitBtn.getBtnRect()) ) menu = false;
                break;
            }
        }
    }
    if( isPause() && !isSetting() ){
        renderPauseMenu();
        switch( e.type ){
        case SDL_MOUSEBUTTONDOWN:
            if( checkMouse(*menuBtn.getBtnRect()) ){
                Mix_PlayChannel( -1, sfx_menu_2, 0 );
                menu = true;
            }
            if( checkMouse(*settingBtn[1].getBtnRect()) ){
                Mix_PlayChannel( -1, sfx_menu_2, 0 );
                setting = true;
            }
            if( checkMouse(*backBtn.getBtnRect()) ){
                Mix_PlayChannel( -1, sfx_menu_2, 0 );
                paused = false;
            }
            break;
        }
    }
    if( isSetting() ){
        renderSetting();
        switch( e.type ){
        case SDL_MOUSEBUTTONDOWN:
            if( checkMouse(*okBtn.getBtnRect()) ){
                Mix_PlayChannel( -1, sfx_menu_2, 0 );
                setting = false;
            }
            if( checkMouse( {675, 320, 12, 14} ) ){
                Mix_PlayChannel( -1, sfx_menu_2, 0 );
                if( tickM ) tickM = false;
                else tickM = true;
            }
            if( checkMouse( {675, 260, 12, 14} ) ){
                Mix_PlayChannel( -1, sfx_menu_2, 0 );
                if( tickS ) tickS = false;
                else tickS = true;
            }
            if( checkMouse( {675, 380, 12, 14} ) ){
                Mix_PlayChannel( -1, sfx_menu_2, 0 );
                if( fullScr ){
                    func::quitWindowFScr();   
                    fullScr = false;
                }
                else{
                    func::setWindowFScr();
                    fullScr = true;
                }
            }
        }
    }
}

void Menu::selectButton( button& check )
{
    if( checkMouse( *check.getBtnRect() ) ){
        if( !check.selected ) Mix_PlayChannel( -1, sfx_menu_1, 0 );
        check.selected = true;
        renderArrow( check );
    }
    else check.selected = false;
}

void Menu::renderArrow( button& check )
{
    SDL_Rect* dst = new SDL_Rect;
    dst->x = check.getBtnRect()->x - 25;
    dst->y = (check.getBtnRect()->y + check.getBtnRect()->h/2) - arrow.getBtnRect()->h/2;
    dst->w = arrow.getBtnRect()->w;
    dst->h = arrow.getBtnRect()->h;
    func::renderTexture( arrow.getBtnTex(), nullptr, dst );
}

void Menu::renderMainMenu()
{
    func::renderTexture( menuBG, nullptr, nullptr );
    func::renderTexture( playBtn.getBtnTex(), nullptr, playBtn.getBtnRect() );
    func::renderTexture( settingBtn[0].getBtnTex(), nullptr, settingBtn[0].getBtnRect() );
    func::renderTexture( exitBtn.getBtnTex(), nullptr, exitBtn.getBtnRect() );

    if( !isSetting() ){
        selectButton( playBtn );
        selectButton( settingBtn[0] );
        selectButton( exitBtn );
    }
}

void Menu::renderPauseMenu()
{
    func::renderTexture( pauseBG.getBtnTex(), nullptr, pauseBG.getBtnRect() );
    func::renderTexture( menuBtn.getBtnTex(), nullptr, menuBtn.getBtnRect() );
    func::renderTexture( settingBtn[1].getBtnTex(), nullptr, settingBtn[1].getBtnRect() );
    func::renderTexture( backBtn.getBtnTex(), nullptr, backBtn.getBtnRect() );

    if( !isSetting() ){
        selectButton( menuBtn );
        selectButton( settingBtn[1] );
        selectButton( backBtn );
    }
}

void Menu::renderEndMenu( int score )
{
    int *w = new int, *h = new int;
    SDL_Color color = { 255, 255, 255, 255};
    SDL_Texture* text = func::createText( std::to_string(score), color );
    SDL_QueryTexture( text, nullptr, nullptr, w, h );

    func::renderTexture( endBG.getBtnTex(), nullptr, endBG.getBtnRect() );
    func::renderTexture( retryBtn.getBtnTex(), nullptr, retryBtn.getBtnRect() );
    func::renderTexture( text, nullptr, 650-*w*3/4, 339, *w*1.25, *h*1.25 );

    delete w, h;
    SDL_DestroyTexture( text );
    if( !isPause() ) selectButton( retryBtn );
}

void Menu::renderSetting()
{
    func::renderTexture( settingBG.getBtnTex(), nullptr, settingBG.getBtnRect() );
    func::renderTexture( settingBtn[0].getBtnTex(), nullptr, 538, 167, 200, 48 );
    func::renderTexture( checkBox.getBtnTex(), nullptr, 590, 310, 106, 34 );
    func::renderTexture( musicTex, nullptr, 599, 317, 65, 24);
    func::renderTexture( checkBox.getBtnTex(), nullptr, 590, 250, 106, 34 );
    func::renderTexture( soundTex, nullptr, 598, 257, 68, 24);
    func::renderTexture( checkBox.getBtnTex(), nullptr, 590, 370, 106, 34 );
    func::renderTexture( fullScrTex, nullptr, 598, 377, 68, 24);
    func::renderTexture( okBtn.getBtnTex(), nullptr, okBtn.getBtnRect() );

    selectButton( okBtn );
    if( tickS ) func::renderTexture( mark, nullptr, 675, 260, 12, 14);
    if( tickM ) func::renderTexture( mark, nullptr, 675, 320, 12, 14);
    if( fullScr ) func::renderTexture( mark, nullptr, 675, 380, 12, 14);

}

bool Menu::checkMouse( const SDL_Rect& check )
{
    int mouseX, mouseY;
    SDL_GetMouseState( &mouseX, &mouseY );
    return (mouseX>=check.x && mouseX<=check.x+check.w) && (mouseY>=check.y && mouseY<=check.y+check.h) ;
}
