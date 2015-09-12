#include "Animation.h"

Animation::Animation()
{

}

Animation::Animation(LPD3DXSPRITE spriteHandler,LPWSTR filePath, int totalFrame, int rows, int columns, float timePerFrame, D3DCOLOR colorKey)
{
    InitWithAnimation(spriteHandler, filePath, totalFrame, rows, columns, timePerFrame, colorKey);
}

void Animation::InitWithAnimation(LPD3DXSPRITE spriteHandler, LPWSTR filePath, int totalFrame, int rows, int columns, float timePerFrame, D3DCOLOR colorKey)
{
    mSprite = new Sprite(spriteHandler, filePath, NULL, 0, 0, colorKey);
    mCurrentColumn = 0;
    mCurrentRow = 0;
    mTimePerFrame = timePerFrame;
    mTotalFrame = totalFrame;
    mRows = rows;
    mColumns = columns;

    //width - height luc nay la cua spritesheet
    mFrameWidth = mSprite->GetWidth() / mColumns;
    mFrameHeight = mSprite->GetHeight() / mRows;

    //set lai cho size cua sprite bang dung size cua frame de tranh bi lech vi tri
    mSprite->SetWidth(mFrameWidth);
    mSprite->SetHeight(mFrameHeight);

    mRect = new RECT();
    mRect->top = 0;
    mRect->left = 0;
    mRect->right = mSprite->GetWidth() / mColumns;
    mRect->bottom = mSprite->GetHeight() / mRows;
    mSprite->SetSourceRect(mRect);
}

Animation::~Animation()
{

}

D3DXVECTOR3 Animation::GetPosition()
{
    return mSprite->GetPosition();
}

void Animation::SetPosition(D3DXVECTOR3 pos)
{
    mSprite->SetPosition(pos);
}

void Animation::SetPosition(float x, float y)
{
    mSprite->SetPosition(D3DXVECTOR3(x, y, 0));
}

void Animation::SetFlipVertical(bool flag)
{
    mSprite->FlipVertical(flag);
}

void Animation::SetFlipHorizontal(bool flag)
{
    mSprite->FlipHorizontal(flag);
}

D3DXVECTOR2 Animation::GetScale()
{
    return mSprite->GetScale();
}

void Animation::SetScale(D3DXVECTOR2 scale)
{
    mSprite->SetScale(scale);
}

float Animation::GetRotation()
{
    return mSprite->GetRotation();    
}

void Animation::SetRotation(float rotation) // by radian
{
    mSprite->SetRotation(rotation);
}

D3DXVECTOR2 Animation::GetRotationCenter()
{
    return mSprite->GetRotationCenter();
}

void Animation::SetRotationCenter(D3DXVECTOR2 rotationCenter)
{
    mSprite->SetRotationCenter(rotationCenter);
}

D3DXVECTOR2 Animation::GetTranslation()
{
    return mSprite->GetTranslation();
}

void Animation::SetTranslation(D3DXVECTOR2 translation)
{
    mSprite->SetTranslation(translation);
}

int Animation::getWidth()
{
    return mSprite->GetWidth();
}

int Animation::getHeight()
{
    return mSprite->GetHeight();
}

Sprite* Animation::GetSprite()
{
    return mSprite;
}

void Animation::Update(float dt)
{
    if (mCurrentTotalTime >= mTimePerFrame)
    {
        mCurrentTotalTime = 0;
        mCurrentIndex++;
        mCurrentColumn++;

        if (mCurrentIndex >= mTotalFrame)
        {
            mCurrentIndex = 0;
            mCurrentColumn = 0;
            mCurrentRow = 0;
        }            

        if (mCurrentColumn >= mColumns)
        {
            mCurrentColumn = 0;
            mCurrentRow++;

            if (mCurrentRow >= mRows)
                mCurrentRow = 0;
        }

        mRect->left = mCurrentColumn * mFrameWidth;
        mRect->right = mRect->left + mFrameWidth;
        mRect->top = mCurrentRow * mFrameHeight;
        mRect->bottom = mRect->top + mFrameHeight;
    }
    else
    {
        mCurrentTotalTime += dt;
    }
}

void Animation::Draw(D3DXVECTOR3 position, RECT *sourceRect, D3DXVECTOR2 scale ,
    D3DXVECTOR2 transform , float angle, D3DXVECTOR2 rotationCenter, D3DXVECTOR2 scalingCenter, D3DXCOLOR colorKey)
{
    mSprite->Draw(position, sourceRect, scale, transform, angle, rotationCenter, scalingCenter, colorKey);
}
