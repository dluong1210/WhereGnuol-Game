#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "func.h"
#include "player.h"

struct button{
    SDL_Texture* btnTex = nullptr;
    SDL_Rect* btnRect = nullptr;
    bool selected = false;

    button(){;}
    void setButton( SDL_Texture* image, int _x, int _y, int _w, int _h ){
        btnTex = image;

        btnRect = new SDL_Rect;
        btnRect->x = _x;
        btnRect->y = _y;
        btnRect->w = _w;
        btnRect->h = _h;
    }
    SDL_Texture* getBtnTex() { return btnTex; }
    SDL_Rect* getBtnRect() { return btnRect; }
};

class Menu
{
private:
    button playBtn;
    button settingBtn[2];
    button exitBtn;
    button okBtn;
    button menuBtn;
    button backBtn;
    button retryBtn;

    button settingBG;
    button pauseBG;
    button endBG;
    button arrow;
    button checkBox;

    SDL_Texture* menuBG = nullptr;
    SDL_Texture* mark = nullptr;
    SDL_Texture* soundTex = nullptr;
    SDL_Texture* musicTex = nullptr;
    SDL_Texture* fullScrTex = nullptr; 

    Mix_Chunk* sfx_menu_1 = nullptr;
    Mix_Chunk* sfx_menu_2 = nullptr;

    bool menu = true, paused = false, endG = false, retry = false, setting = false, tickS = true, tickM = true, fullScr = false;

public:
    Menu( SDL_Texture* MenuTex[] , Mix_Chunk* MenuSfx[] );
    void handleInput( SDL_Event& e, Player& _player, bool& isGameRunning );

    void renderArrow( button& check );
    void renderMainMenu();
    void renderPauseMenu();
    void renderSetting();
    void renderEndMenu( int score );

    void selectButton( button& check );
    bool checkMouse( const SDL_Rect& check );

    bool isMenu() { return menu; }
    bool isPause() { return paused; }
    bool isEnd() { return endG; }
    bool isSetting() { return setting; }
    bool isRetry() { return retry; }
    bool soundOn() { return tickS; }
    bool musicOn() { return tickM; }
    bool isFullScr() { return fullScr; }

    bool isRunning() { return menu || paused || endG || setting; }
};

#endif // MENU_H
