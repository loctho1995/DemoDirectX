#include "Game.h"

Game::Game(int fps, int width, int height)
{
    mWidth = width;
    mHeight = height;
    mFPS = fps;
    GameGlobal::SetWidth(width);
    GameGlobal::SetHeight(height);
    InitInput();

	Scene *newScene = new SceneGamePlay();
    SceneManager::GetInstance()->ReplaceScene(newScene);

    LoadContent();
    InitLoop();
}

Game::~Game()
{

}

LPDIRECT3DSURFACE9 Game::createSurfaceFromFile(LPDIRECT3DDEVICE9 device, LPWSTR filePath)
{
    D3DXIMAGE_INFO info;

    HRESULT result = D3DXGetImageInfoFromFile(filePath, &info);

    if (result != D3D_OK)
    {
        GAMELOG("[Error] Failed to get image info %s", filePath);
        return NULL;
    }

    LPDIRECT3DSURFACE9 surface;
    GameGlobal::GetCurrentDevice()->CreateOffscreenPlainSurface(info.Width, info.Height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);

    result = D3DXLoadSurfaceFromFile(surface, NULL, NULL, filePath, NULL, D3DX_DEFAULT, 0, NULL);
    if (result != D3D_OK)
    {
        GAMELOG("[Error] Failed to load image from %s", filePath);
        return NULL;
    }

    return surface;
}

void Game::LoadContent()
{
    //GameGlobal::GetCurrentDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &surface);
    GameGlobal::GetCurrentDevice()->GetRenderTarget(0, &GameGlobal::backSurface);

    SceneManager::GetInstance()->GetCurrentScene()->LoadContent();
}

void Game::OnKeyDown(int keyCode)
{
    SceneManager::GetInstance()->GetCurrentScene()->OnKeyDown(keyCode);
}

void Game::OnKeyUp(int keyCode)
{
    SceneManager::GetInstance()->GetCurrentScene()->OnKeyUp(keyCode);
}

void Game::OnMouseDown(float x, float y)
{
    SceneManager::GetInstance()->GetCurrentScene()->OnMouseDown(x, y);
}

void Game::Update(float dt)
{
    //GAMELOG("fps: %f", 1 / dt);

    SceneManager::GetInstance()->GetCurrentScene()->Update(dt);
    Render();
}

void Game::Render()
{
    if (GameGlobal::GetCurrentDevice()->BeginScene())
    {        
        //GameGlobal::GetCurrentDevice()->Clear(0, NULL, D3DCLEAR_TARGET, SceneManager::GetInstance()->GetCurrentScene()->GetBackcolor(), 0.0, 0);
        GameGlobal::GetCurrentDevice()->ColorFill(GameGlobal::backSurface, NULL, SceneManager::GetInstance()->GetCurrentScene()->GetBackcolor());

        GameGlobal::GetCurrentSpriteHandler()->Begin(D3DXSPRITE_ALPHABLEND);
        SceneManager::GetInstance()->GetCurrentScene()->Draw();
        GameGlobal::GetCurrentSpriteHandler()->End();

        SceneManager::GetInstance()->GetCurrentScene()->DoEndScene();
        GameGlobal::GetCurrentDevice()->EndScene();
    }

    GameGlobal::GetCurrentDevice()->Present(0, 0, 0, 0);
}

void Game::InitLoop()
{
    MSG msg;

    float tickPerFrame = 1.0f / mFPS, delta = 0;

    while (GameGlobal::isGameRunning)
    {
        GameTime::GetInstance()->StartCounter();

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        GameInput::GetInstance()->UpdateInput();

        delta += GameTime::GetInstance()->GetCouter();

        if (delta >= tickPerFrame)
        {
            Update((delta));
            //GAMELOG("FPS: %f", 1.0 / delta);
            delta = 0;
        }
        else
        {
            Sleep(tickPerFrame - delta);
            delta = tickPerFrame;
        }
    }
}

void Game::InitInput()
{
    GameInput::GetInstance()->SetKeyboardUser(this);
}