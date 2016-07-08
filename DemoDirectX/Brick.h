#pragma once
#ifndef __GAME_OBJECT_BRICK__
#define __GAME_OBJECT_BRICK__

#include "QuadTree.h"
#include "Sprite.h"
#include "GameGlobal.h"
#include "Camera.h"

//vien gach
class Brick :
    public Entity
{
public:
    enum BrickType
    {
        BrickNormal, // nhung vien gach thuong 
        BrickGold //nhung o vuong an vang
    };

    Brick(RECT rectBound, RECT *sourceRect, D3DXVECTOR3 position);

    //bound thuc su o trong world map
    //RECT getRealBound(); 

    //lay Bound tuong ung voi vi tri nhin thay tren screen (toa do (0,0) la goc trai tren), vung ranh gioi trong khoang Width, Height cua Screen
    //RECT getBoundOnScreen(Camera *camera);

    //vi tri tuong ung trong sprite ve hinh anh
    RECT *SourceRect; 

    D3DXVECTOR3 Position;
    BrickType Type;

    ~Brick();
};

#endif
