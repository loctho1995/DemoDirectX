#ifndef __GAME_OBJECT_BRICK_GOLD__
#define __GAME_OBJECT_BRICK_GOLD__
#include "QuadTree.h"
#include "Sprite.h"
#include "GameGlobal.h"
#include "Camera.h"

class BrickGold : public Entity
{
public:
    BrickGold(RECT rectBound, RECT *sourceRect, D3DXVECTOR3 position);
    ~BrickGold();
};

#endif