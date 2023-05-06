#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <SDL.h>
#include <SDL_image.h>

class baseObject
{
protected:
    int x, y;
    SDL_Texture* texture;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    SDL_Rect collision;
public:

    baseObject( int _x=0, int _y=0, SDL_Texture* image=nullptr );

    SDL_Texture* getTexture() const;
    int getX() const;
    int getY() const;
    SDL_RendererFlip getFlip() const { return flip; }
    SDL_Rect getCollision() const { return collision; }
};

#endif // BASEOBJECT_H
