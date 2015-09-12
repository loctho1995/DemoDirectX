#pragma once
#ifndef __CAMERA__
#define __CAMERA__

#include <d3dx9.h>
#include <d3d9.h>
#include "GameLog.h"

class Camera
{
public:
    Camera(int width, int height);

    void SetPosition(float x, float y);
    void SetPosition(D3DXVECTOR3 pos);

    //camera position se phu thuoc vao toa do cua pos nay
    void SetPositionFollow(D3DXVECTOR3 *pos);

    int GetWidth();
    int GetHeight();
    void SetAllowChangePosition(bool flag);

    D3DXVECTOR3 GetPosition();    
    RECT GetBound();

    ~Camera();

private:
    int             mWidth,
                    mHeight;

    float           mPositionX,
                    mPositionY;

    bool            mAllowChangePosition;

    D3DXVECTOR3     *mFixedPos;
};

#endif

