#pragma once
#ifndef __GAME_COLLISION__
#define __GAME_COLLISION__

#include <d3dx9.h>
#include <d3d9.h>
#include "GameLog.h"

class GameCollision
{
public:
    struct CollisionReturn
    {
        bool IsCollided;
        RECT RegionCollision;
    };

    GameCollision();
    static CollisionReturn RectangleAndRectangle(RECT rect, RECT rect2);
    static bool PointAndRectangle(float x, float y, RECT rect);
    static bool RectangleAndCircle(RECT rect, int circlex, int circley, int circleRadius);
    ~GameCollision();
};

#endif
