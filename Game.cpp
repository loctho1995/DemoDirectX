#include "Game.h"
int Game::mIsDone = 0;

Game::Game(HINSTANCE hInstance, LPWSTR name, int cmdShow, Scene *newScene, int fps, int width, int height)
{
    mHInstance = hInstance;
    mWidth = width;
    mHeight = height;
    mName = name;
    mCmdShow = cmdShow;
    mFPS = fps;

    SceneManager::GetInstance()->ReplaceScene(newScene);

    InitWindow();
}

Game::~Game()
{
    if (mDirect3D9)
        mDirect3D9->Release();

    if (mSpriteHandler)
        mSpriteHandler->Release();

    if (mDevice)
        mDevice->Release();

    if (mBackground)
        mBackground->Release();
        
    if (mBackBuffer)
        mBackBuffer->Release();
}

LRESULT CALLBACK Game::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        mIsDone = 1;
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
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
    mDevice->CreateOffscreenPlainSurface(info.Width, info.Height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);

    result = D3DXLoadSurfaceFromFile(surface, NULL, NULL, filePath, NULL, D3DX_DEFAULT, 0, NULL);
    if (result != D3D_OK)
    {
        GAMELOG("[Error] Failed to load image from %s", filePath);
        return NULL;
    }

    return surface;
}

void Game::InitWindow()
{
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hInstance = mHInstance;

    wc.lpfnWndProc = (WNDPROC)this->WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIcon = NULL;

    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = mName;
    wc.hIconSm = NULL;

    RegisterClassEx(&wc);

    //WS_OVERLAPPEDWINDOW <=> WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE
    HWND hWnd = CreateWindow(
        mName,
        mName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        mWidth,
        mHeight,
        NULL,
        NULL,
        mHInstance,
        NULL);

    if (!hWnd)
    {
        DWORD errCode = GetLastError();
        GAMELOG("[Error] Create Windows failed");
    }

    mHwnd = hWnd;

    GameGlobal::SetCurrentHINSTACE(mHInstance);
    GameGlobal::SetCurrentHWND(mHwnd);

    ShowWindow(hWnd, mCmdShow);
    UpdateWindow(hWnd);
    InitDevice();
    InitInput();

    LoadContent();
    InitLoop();
}

void Game::InitLoop()
{
    MSG msg;
    mIsDone = 0;
    float tickPerFrame = 1.0f / mFPS, delta = 0;

    while (!mIsDone)
    {
        GameTime::GetInstance()->StartCounter();

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        GameInput::GetInstance()->UpdateInput();

        delta += GameTime::GetInstance()->GetCouter();
        //GAMELOG("counter: %f", delta);

        if (delta >= tickPerFrame)
        {
            Update((delta));
            delta = 0;
        }
        else
        {
            Sleep(tickPerFrame - delta);
            delta = tickPerFrame;
        }
    }
}

void Game::InitDevice()
{      
    mD3d = Direct3DCreate9(D3D_SDK_VERSION);
    D3DPRESENT_PARAMETERS d3dpp;

    ZeroMemory(&d3dpp, sizeof(d3dpp));

    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferCount = 1;
    d3dpp.BackBufferWidth = mWidth;
    d3dpp.BackBufferHeight = mHeight;

    HRESULT dvresult = mD3d->CreateDevice(  D3DADAPTER_DEFAULT, 
                                            D3DDEVTYPE_HAL, 
                                            mHwnd, 
                                            D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
                                            &d3dpp, 
                                            &mDevice);
    
#if _DEBUG
        switch (dvresult)
        {
        case D3DERR_INVALIDCALL:
            GAMELOG("[Error] Direct3DCreate9 parameters are wrong");
            return;


        case D3DERR_NOTAVAILABLE:
            GAMELOG("[Error] Direct3DCreate9 doesn't support this call");
            return;

        case D3DERR_OUTOFVIDEOMEMORY:
            GAMELOG("[Error] Direct3DCreate9 video card isn't enough memory");
            return;

        case D3D_OK:

            GAMELOG("Create Device successed");
            return;

        default:
            break;
        }    
#endif    


    //mDevice->CreateOffscreenPlainSurface(100, 100, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &mBackground, NULL);
}

void Game::InitInput()
{
    GameInput::GetInstance()->SetKeyboardUser(this);
}

void Game::LoadContent()
{
    HRESULT rs = D3DXCreateSprite(mDevice, &mSpriteHandler);
    mDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &mBackBuffer);
    GameGlobal::SetCurrentSpriteHandler(mSpriteHandler);

#if _DEBUG
    if (rs != D3D_OK)
    {
        GAMELOG("[Error] Create sprite handler failed");
    }
#endif

    SceneManager::GetInstance()->GetCurrentScene()->LoadContent();
    mBackground = createSurfaceFromFile(mDevice, L"good land1.png");
}

void Game::OnKeyDown(int keyCode)
{
    SceneManager::GetInstance()->GetCurrentScene()->OnKeyDown(keyCode);
}

void Game::OnKeyUp(int keyCode)
{
    SceneManager::GetInstance()->GetCurrentScene()->OnKeyUp(keyCode);
}

void Game::Update(float dt)
{
    //GAMELOG("fps: %f", 1 / dt);

    SceneManager::GetInstance()->GetCurrentScene()->Update(dt);
    Render();
}

void Game::Render()
{
    if (mDevice->BeginScene())
    {
        mDevice->ColorFill(mBackground, NULL, SceneManager::GetInstance()->GetCurrentScene()->GetBackcolor());

        mDevice->StretchRect(   mBackground,			// from 
                                NULL,				// which portion?
                                mBackBuffer,		// to 
                                NULL,				// which portion?
                                D3DTEXF_NONE);

        //GAMELOG("ground: %d - buffer: %d" , mBackground, mBackBuffer);
        mSpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
        SceneManager::GetInstance()->GetCurrentScene()->Draw();
        mSpriteHandler->End();
        mDevice->EndScene();
    }

    mDevice->Present(0, 0, 0, 0);
}

