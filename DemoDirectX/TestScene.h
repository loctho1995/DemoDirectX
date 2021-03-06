#pragma once

#include <math.h>
#include <vector>
#include <d3dx9.h>
#include <d3d9.h>

#include "Scene.h"
#include "Sprite.h"
#include "GameDebugDraw.h"
#include "SceneManager.h"
#include "TransitionScanDown.h"

class TestScene : public Scene
{
public:
    TestScene();
    ~TestScene();

    void Update(float dt);
    void LoadContent();
    void Draw();

    void OnKeyDown(int keyCode);
    void OnKeyUp(int keyCode);
    void OnMouseDown(float x, float y);
    void updateFullPoints();
    void initEffect();
    void DoEndScene();

    //tinh n giao thua
    unsigned int Factorial(int n);

    //tinh phuong trinh duong cong bezier
    //dung cong thuc tinh he so cua tam giac pascal
    unsigned int CalculateCoefficient(int n, int i);

    //lay diem tuong ung de ve len
    D3DXVECTOR2 CalculateSinglePoint(float t, std::vector< D3DXVECTOR2 > pointVector);

    std::vector<Sprite*> sprites;
    std::vector<D3DXVECTOR2> points;
    std::vector<D3DXVECTOR2> savePoints;
    std::vector<D3DXVECTOR2> fullPoints;
    float t;
    float speed; //toc do t de ve bezier (t = 0.01f / speed)
    int frameDelay; //delay frame
    int currentFrame; //dem frame hien tai
    int numSprite; //so luong sprite arrow
    int dotDistance; // khoang cach cua cac dau cham tren duong di
    int dotDistanceCounter;
    float xDistance, yDistance;
    Sprite *dot, *dot2; //sprite ve 1 cham len man hinh
    D3DXVECTOR2 previousPoint;    
    bool isPause, isDebugDraw, isHideAll, isUseMouse, isDrawBezier;
    GameDebugDraw *debugDraw, *drawBezier;

    float timer, posMove;
    LPDIRECT3DTEXTURE9 texture0, texture1;
    LPD3DXEFFECT effect;
    LPDIRECT3DVERTEXBUFFER9 vertexBuffer = NULL;
    LPDIRECT3DVERTEXDECLARATION9 vertexDeclaration = NULL;
    LPDIRECT3DINDEXBUFFER9 indexBuffer = NULL;

    D3DXMATRIX world, view, proj;
};

