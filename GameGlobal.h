#pragma once
#ifndef __GAME_GLOBAL__
#define __GAME_GLOBAL__
#include <d3dx9.h>
#include <d3d9.h>
#include <Windows.h>
#include "GameLog.h"

class GameGlobal
{
public:
    GameGlobal();
    ~GameGlobal();

    static HINSTANCE GetCurrentHINSTACE();
    static HWND getCurrentHWND();

    static void SetCurrentHINSTACE(HINSTANCE hInstance);
    static void SetCurrentHWND(HWND hWnd);

    static void SetCurrentSpriteHandler(LPD3DXSPRITE spriteHandler);
    static LPD3DXSPRITE GetCurrentSpriteHandler();

    static void SetWidth(int width);
    static int GetWidth();

    static void SetHeight(int height);
    static int GetHeight();

private:
    static HINSTANCE mHInstance;
    static HWND mHwnd;
    static LPD3DXSPRITE mSpriteHandler;
    static int mWidth, mHeight;

};

#endif
