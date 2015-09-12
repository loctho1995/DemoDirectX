#include "Camera.h"


Camera::Camera(int width, int height)
{
    mWidth = width ;
    mHeight = height ;

    mPositionX = 0;
    mPositionY = 0;

    mFixedPos = NULL;
    mAllowChangePosition = true;
}


Camera::~Camera()
{
}

void Camera::SetPositionFollow(D3DXVECTOR3 *pos)
{
    mFixedPos = pos;
}

void Camera::SetPosition(float x, float y)
{
    mPositionX = x;
    mPositionY = y;
}

void Camera::SetPosition(D3DXVECTOR3 pos)
{
    if (mAllowChangePosition)
    {
        mPositionX = pos.x;
        mPositionY = pos.y;
    }
}

D3DXVECTOR3 Camera::GetPosition()
{
    return D3DXVECTOR3(mPositionX, mPositionY, 1);
}

RECT Camera::GetBound()
{
    RECT bound; 

    bound.left = mPositionX - mWidth / 2;
    bound.right = bound.left + mWidth;;
    bound.top = mPositionY - mHeight / 2;
    bound.bottom = bound.top + mHeight;

    return bound;
}

int Camera::GetWidth()
{
    return mWidth;
}

int Camera::GetHeight()
{
    return mHeight;
}

void Camera::SetAllowChangePosition(bool flag)
{
    mAllowChangePosition = flag;
}