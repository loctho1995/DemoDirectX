#include "TestScene.h"

/*
bam phim d: de hien/an cac duong line noi cac diem (control points)
bam phim h: de an/hien tat ca (tru sprite arrow)
bam phim m: de dung chuot bam vao man hinh
bam phim b: de ve duong cong bezier

bam numberpad1: de ve bezier bac 2
bam numberpad2: de ve bezier bac 3
bam numberpad3: de ve bezier bac 4
bam numberpad4: de ve bezier bac 12
bam +: de tang toc do
bam -: de giam toc do


*/

#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZ | D3DFVF_TEX1 )

struct VertexData
{
    D3DXVECTOR3 Position;
    D3DCOLOR Color;
};

LPDIRECT3DVERTEXBUFFER9 vertexBuffer = NULL;

VertexData vertexData[] =
{
    //  x     y     z       color
    { D3DXVECTOR3(-1.0f, 1.0f, 0.0f), D3DCOLOR_XRGB(0, 0, 0)},
    { D3DXVECTOR3(1.0f, 1.0f, 0.0f), D3DCOLOR_XRGB(0, 0, 0) },
    { D3DXVECTOR3(-1.0f, -1.0f, 0.0f), D3DCOLOR_XRGB(0, 0, 0) },
    { D3DXVECTOR3(1.0f, -1.0f, 0.0f), D3DCOLOR_XRGB(0, 0, 0) }
};

//VertexData vertexData[] =
//{
//    //  x     y     z       color
//    { D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_XRGB(0, 0, 0) },
//    { D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DCOLOR_XRGB(0, 0, 0) },
//    { D3DXVECTOR3(1.0f, 1.0f, 0.0f), D3DCOLOR_XRGB(0, 0, 0) },
//    { D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DCOLOR_XRGB(0, 0, 0) }
//};

//
//LPDIRECT3DTEXTURE9 pTexture_0 = NULL;
//LPDIRECT3DTEXTURE9 pTexture_1 = NULL;

TestScene::TestScene()
{
    this->mBackColor = 0x54acd2;

}

TestScene::~TestScene()
{
}

void TestScene::LoadContent()
{
    debugDraw = new GameDebugDraw(GameGlobal::GetCurrentSpriteHandler());
    drawBezier = new GameDebugDraw(GameGlobal::GetCurrentSpriteHandler());
    drawBezier->setLineSize(1.6f);
    debugDraw->setLineSize(3);

    speed = 1.0;
    t = 0;
    frameDelay = 0;
    numSprite = 10;
    xDistance = yDistance = 0;
    timer = 0;
    posMove = 0;
    dotDistance = 0;
    dotDistanceCounter = 0;
    isPause = false;
    isDebugDraw = false;
    isUseMouse = false;
    isDrawBezier = false;

    dot = new Sprite("dot.png");
    dot2 = new Sprite("dot2.png");

    //khoi tao cac control point - bezier bac 2
    points.push_back(D3DXVECTOR2(100, 100));
    points.push_back(D3DXVECTOR2(400, 300));
    points.push_back(D3DXVECTOR2(200, 600));

    updateFullPoints();

    for (size_t i = 0; i < numSprite; i++)
    {
        sprites.push_back(new Sprite("test.png"));
    }

    previousPoint = CalculateSinglePoint(0, points);

    initEffect();
}

void TestScene::initEffect()
{
    LPD3DXBUFFER bufferErrors = NULL;
    HRESULT hr;

    //D3DXMatrixIdentity(&world);
    //D3DXMatrixIdentity(&view);
    //D3DXMatrixIdentity(&proj);

    //D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(40.0f), (float)GameGlobal::GetWidth() / (float)GameGlobal::GetHeight(), 0.0f, 100.0f);

    unsigned int vertexDataSize = 4 * sizeof(VertexData);

    //create vertex buffer
    GameGlobal::GetCurrentDevice()->CreateVertexBuffer(vertexDataSize, D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX, 
                                                        D3DPOOL_DEFAULT, &vertexBuffer, 0);

    void *pVertices = NULL;
    vertexBuffer->Lock(0, vertexDataSize, (void**)&pVertices, 0);
    memcpy(pVertices, vertexData, vertexDataSize);
    vertexBuffer->Unlock();

    hr = D3DXCreateEffectFromFileA(GameGlobal::GetCurrentDevice(), "Empty.fx", 0, 0, 0, 0, &effect, &bufferErrors);

    //D3DXMatrixTranslation(&view, 0, 0, 2.75);
    //D3DXMATRIX matrix = world * view * proj;
    //effect->SetMatrix("WorldViewProj", &(world * view * proj));

    //D3DXCreateTexture(GameGlobal::GetCurrentDevice(), GameGlobal::GetWidth(), GameGlobal::GetHeight(), D3DX_DEFAULT,
    //                        D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &texture0);

    //D3DXCreateTexture(GameGlobal::GetCurrentDevice(), GameGlobal::GetWidth(), GameGlobal::GetHeight(), D3DX_DEFAULT,
    //                        D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &texture1);

    D3DXCreateTextureFromFileA(GameGlobal::GetCurrentDevice(), "valve.png", &texture0);
    D3DXCreateTextureFromFileA(GameGlobal::GetCurrentDevice(), "hungtrung.png", &texture1);  
}

void TestScene::Update(float dt)
{
    posMove += 0.008f;

    if (isPause)
        return;

    if (currentFrame > frameDelay)
    {
        currentFrame = 0;
    }
    else
    {
        currentFrame++;
        return;
    }

    if (t > 1)
    {
        t = 0;
        savePoints.clear();
    } 
    else
    {
        t += 0.01f * speed;

        D3DXVECTOR2 vec = CalculateSinglePoint(t, points);

        //tinh toan de xoay sprite
        D3DXVECTOR2 dir = vec - previousPoint;     
        D3DXVec2Normalize(&dir, &dir);

        for (size_t i = 0; i < numSprite; i++)
        {
            sprites.at(i)->SetRotation(acos(dir.x) * (abs(dir.y) / dir.y));
        }        

        //GAMELOG("x: %f ; y: %f ; rotation: %f", dir.x, dir.y, D3DXToDegree(sprite->GetRotation()));

        for (size_t i = 0; i < numSprite; i++)
        {
            sprites.at(i)->SetPosition(D3DXVECTOR3(vec.x + i * xDistance, vec.y + i * yDistance, 0));
        }

        previousPoint = vec;

        //them vi tri ve dot tren man hinh
        if (dotDistanceCounter > dotDistance)
        {
            dotDistanceCounter = 0;
            savePoints.push_back(vec);
        }
        else
        {
            dotDistanceCounter++;
        }
        //GAMELOG("x: %f ; y: %f", sprite->GetPosition().x, sprite->GetPosition().y);
    }    

    timer += 0.01f;
}

void TestScene::Draw()
{

    if (isDebugDraw)
        debugDraw->DrawLine(&points[0], points.size());

    for (size_t i = 0; i < numSprite; i++)
    {
        sprites.at(i)->Draw();
        //GAMELOG("x: %f; y: %f", sprites.at(i)->GetPosition().x, sprites.at(i)->GetPosition().y);
    }

    if (isDrawBezier)
    {
        if (fullPoints.size() >= 2)
            drawBezier->DrawLine(&fullPoints[0], fullPoints.size());
    }

    if (!isHideAll)
    {
        for (size_t i = 0; i < savePoints.size(); i++)
        {
            D3DXVECTOR3 pos = D3DXVECTOR3(savePoints.at(i).x, savePoints.at(i).y, 0);
            dot->Draw(pos);
        }
    }

    if (!isHideAll)
    {
        for (size_t i = 0; i < points.size(); i++)
        {
            D3DXVECTOR3 pos = D3DXVECTOR3(points.at(i).x, points.at(i).y, 0);
            dot2->Draw(pos);
        }
    }
}

void TestScene::DoEndScene()
{
    //IDirect3DSurface9 *surface;
    //texture0->GetSurfaceLevel(0, &surface);
    //D3DXLoadSurfaceFromSurface(surface, NULL, NULL, GameGlobal::backSurface, NULL, NULL, D3DX_FILTER_NONE, 0);
    //GameGlobal::GetCurrentDevice()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0, 0);
    //surface->Release();

    D3DXMATRIX mtxViewProj;
    D3DXMatrixIdentity(&mtxViewProj);
    mtxViewProj._11 = 2.0;
    mtxViewProj._22 = -2.0;
    mtxViewProj._41 = 1.0;
    mtxViewProj._42 = 1.0;

    //D3DXMatrixOrthoOffCenterLH(&mtxViewProj, 0.5f, GameGlobal::GetWidth() + 0.5f, 0.5f, GameGlobal::GetHeight() + 0.5f, 0, 0);


    UINT numpasses = 0;
    effect->Begin(&numpasses, 0);
    {
        for (UINT i = 0; i < numpasses; ++i)
        {
            effect->BeginPass(i);
            effect->SetMatrix("WorldViewProj", &(mtxViewProj));
            effect->SetTexture("texture0", texture0);
            effect->SetTexture("texture1", texture1);
            effect->SetFloat("timer", timer);
            effect->SetFloat("posMove", posMove);

            GameGlobal::GetCurrentDevice()->SetStreamSource(0, vertexBuffer, 0, sizeof(VertexData));
            GameGlobal::GetCurrentDevice()->SetFVF(D3DFVF_CUSTOMVERTEX);
            GameGlobal::GetCurrentDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
            effect->EndPass();
        }
    }

    effect->End();
}

void TestScene::OnKeyDown(int keyCode)
{
    if (keyCode == DIK_SPACE)
        isPause = !isPause;

    if (keyCode == DIK_D)
        isDebugDraw = !isDebugDraw;

    if (keyCode == DIK_H)
        isHideAll = !isHideAll;

    if (keyCode == DIK_M)
    {
        isUseMouse = !isUseMouse;
        points.clear();
    }

    if (keyCode == DIK_B)
    {
        isDrawBezier = !isDrawBezier;
    }
        

    //chay bezier bac 2
    if (keyCode == DIK_NUMPAD1)
    {
        points.clear();
        savePoints.clear();
        t = 0;

        points.push_back(D3DXVECTOR2(100, 100));
        points.push_back(D3DXVECTOR2(400, 300));
        points.push_back(D3DXVECTOR2(200, 600));

        updateFullPoints();
    }

    //chay bezier bac 3
    if (keyCode == DIK_NUMPAD2)
    {
        points.clear();
        savePoints.clear();
        t = 0;

        points.push_back(D3DXVECTOR2(100, 100));
        points.push_back(D3DXVECTOR2(400, 200));
        points.push_back(D3DXVECTOR2(200, 400));
        points.push_back(D3DXVECTOR2(500, 600));

        updateFullPoints();
    }

    //chay bezier bac 4
    if (keyCode == DIK_NUMPAD3)
    {
        points.clear();
        savePoints.clear();
        t = 0;

        points.push_back(D3DXVECTOR2(100, 100));
        points.push_back(D3DXVECTOR2(400, 200));
        points.push_back(D3DXVECTOR2(200, 400));
        points.push_back(D3DXVECTOR2(400, 600));
        points.push_back(D3DXVECTOR2(650, 250));

        updateFullPoints();
    }

    //bac 12
    if (keyCode == DIK_NUMPAD4)
    {
        points.clear();
        savePoints.clear();
        t = 0;

        points.push_back(D3DXVECTOR2(0, 0));
        points.push_back(D3DXVECTOR2(300, 80));
        points.push_back(D3DXVECTOR2(450, 150));
        points.push_back(D3DXVECTOR2(200, 200));
        points.push_back(D3DXVECTOR2(10, 250));
        points.push_back(D3DXVECTOR2(50, 350));
        points.push_back(D3DXVECTOR2(300, 450));
        points.push_back(D3DXVECTOR2(350, 250));
        points.push_back(D3DXVECTOR2(850, 150));
        points.push_back(D3DXVECTOR2(600, 350));
        points.push_back(D3DXVECTOR2(400, 450));
        points.push_back(D3DXVECTOR2(150, 650));
        points.push_back(D3DXVECTOR2(900, 680));

        updateFullPoints();
    }

    if (keyCode == DIK_ADD)
    {
        speed += 0.1f;         
        
    }

    if (keyCode == DIK_SUBTRACT)
    {
        speed -= 0.1f;

        if (speed <= 0)
            speed = 0.1f;
    }

    GAMELOG("speed: %f", speed);
}

void TestScene::OnKeyUp(int keyCode)
{

}

void TestScene::OnMouseDown(float x, float y)
{
    if (!isUseMouse)
        return;

    points.push_back(D3DXVECTOR2(x, y));
    savePoints.clear();

    for (size_t i = 0; i < numSprite; i++)
    {
        sprites.at(i)->SetPosition(points.at(0));
    }

    updateFullPoints();
}

void TestScene::updateFullPoints()
{
    fullPoints.clear();

    for (float x = 0.01; x <= 1; x += 0.01f)
    {
        fullPoints.push_back(CalculateSinglePoint(x, points));
    }
}

//tinh giai thua
unsigned int TestScene::Factorial(int n)
{
    unsigned int retval = 1;

    for (int i = n; i > 1; --i)
        retval *= i;

    return retval;
}

//tinh toan he so (cua tam giac pascal)
unsigned int TestScene::CalculateCoefficient(int n, int i)
{
    return Factorial(n) / (Factorial(i) * Factorial(n - i));
}

//lay diem tuong ung
D3DXVECTOR2 TestScene::CalculateSinglePoint(float t, std::vector< D3DXVECTOR2 > pointVector)
{
    int n = pointVector.size() - 1;
    D3DXVECTOR2 result(0, 0);

    for (int i = 0; i <= n; i++)
    {
        D3DXVECTOR2 temp = pointVector.at(i);
        float coefficient = CalculateCoefficient(n, i) * pow(1 - t, n - i) * pow(t, i);
        float x = coefficient * temp.x;
        float y = coefficient * temp.y;
        result = result + D3DXVECTOR2(x, y);
    }

    return result;
}
