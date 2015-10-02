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
#include "Shader.h"
#include "SceneGamePlay.h"

#define KEYBOARD_BUFFERD_SIZE 1024

class Game : public IInput
{
public:
    Game(int fps = 60, int width = 960, int height = 640);
    ~Game();

protected:

    PDIRECT3DSURFACE9       mBackground,
                            mBackBuffer;

    int                     mWidth,
                            mHeight;
    static int              mIsDone;
    float                   mFPS;

    void InitLoop();
    void InitInput();
    void Render();
    void LoadContent();  

    void Update(float dt);
    void OnKeyDown(int keyCode);
    void OnKeyUp(int keyCode);

    LPDIRECT3DSURFACE9 createSurfaceFromFile(LPDIRECT3DDEVICE9 device, LPWSTR filePath);
};

#endif