 #include "baseObject.h"

baseObject::baseObject( int _x, int _y, SDL_Texture* image )
{
    x = _x;
    y = _y;
    texture = image;
}

SDL_Texture* baseObject::getTexture() const
{
    return texture;
}

int baseObject::getX() const
{
    return x;
}

int baseObject::getY() const
{
    return y;
}
