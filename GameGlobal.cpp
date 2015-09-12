#include "GameGlobal.h"

HINSTANCE GameGlobal::mHInstance = NULL;
HWND GameGlobal::mHwnd = NULL;
LPD3DXSPRITE GameGlobal::mSpriteHandler = NULL;
int GameGlobal::mWidth = 960;
int GameGlobal::mHeight = 640;

GameGlobal::GameGlobal()
{

}


GameGlobal::~GameGlobal()
{
}

HINSTANCE GameGlobal::GetCurrentHINSTACE()
{
    return mHInstance;
}

HWND GameGlobal::getCurrentHWND()
{
    return mHwnd;
}

void GameGlobal::SetCurrentHINSTACE(HINSTANCE hInstance)
{
    mHInstance = hInstance;
}

void GameGlobal::SetCurrentHWND(HWND hWnd)
{
    mHwnd = hWnd;
}

void GameGlobal::SetCurrentSpriteHandler(LPD3DXSPRITE spriteHandler)
{
    mSpriteHandler = spriteHandler;
}

LPD3DXSPRITE GameGlobal::GetCurrentSpriteHandler()
{
    return mSpriteHandler;
}


void GameGlobal::SetWidth(int width)
{
    mWidth = width;
}

int GameGlobal::GetWidth()
{
    return mWidth;
}

void GameGlobal::SetHeight(int height)
{
    mHeight = height;
}

int GameGlobal::GetHeight()
{
    return mHeight;
}