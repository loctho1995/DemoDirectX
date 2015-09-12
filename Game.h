#ifndef __GAME__
#define __GAME__
#include <d3dx9.h>
#include <d3d9.h>
#include <Windows.h>
#include <dinput.h>

#include "Sprite.h"
#include "GameLog.h"
#include "IInput.h"
#include "GameInput.h"
#include "GameTime.h"
#include "Animation.h"
#include "GameMap.h"
#include "SceneManager.h"

#define KEYBOARD_BUFFERD_SIZE 1024

class Game : public IInput
{
public:
    Game(HINSTANCE hInstance, LPWSTR name,int cmdShow, Scene *newScene,int fps = 60, int width = 960, int height = 640);
    ~Game();

protected:

    LPDIRECT3D9             mDirect3D9;
    LPD3DXSPRITE            mSpriteHandler;
    PDIRECT3D9              mD3d;
    LPDIRECT3DDEVICE9       mDevice;
    HINSTANCE               mHInstance;
    HWND                    mHwnd;
    PDIRECT3DSURFACE9       mBackground,
                            mBackBuffer;

    int                     mWidth, 
                            mHeight, 
                            mCmdShow;
    static int              mIsDone;
    float                   mFPS;
    LPWSTR                  mName;

    void InitWindow();
    void InitDevice();
    void InitInput();
    void InitLoop();
    void Render();
    void LoadContent();  

    void Update(float dt);
    void OnKeyDown(int keyCode);
    void OnKeyUp(int keyCode);

    LPDIRECT3DSURFACE9 createSurfaceFromFile(LPDIRECT3DDEVICE9 device, LPWSTR filePath);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

#endif