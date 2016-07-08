#include "SceneGamePlay.h"


SceneGamePlay::SceneGamePlay()
{
    
}

SceneGamePlay::~SceneGamePlay()
{
}

void SceneGamePlay::LoadContent()
{
    Scene::LoadContent();
    mGameScene = new GameScene("marioworld1-1.tmx");
    mGameScene->LoadContent();

    sprite = new Sprite("dot2.png");
    sprite->SetPosition(0, 0);
    angle = 0;

}

void SceneGamePlay::Update(float dt)
{
    //xu ly cac su kien ban phim
    mGameScene->Update(dt);

    angle += D3DXToRadian(5);


    float x = cos(angle);
    float y = sin(angle);

    sprite->SetPosition(D3DXVECTOR2(300 + x * 100,300 + y * 100));
   
}

void SceneGamePlay::Draw()
{
    mGameScene->Draw();
    sprite->Draw();
}

void SceneGamePlay::OnKeyDown(int keyCode)
{
    mGameScene->OnKeyDown(keyCode);
	//GAMELOG("%s", keyCode);
}

void SceneGamePlay::OnKeyUp(int keyCode)
{
    mGameScene->OnKeyUp(keyCode);
	//GAMELOG("%s", keyCode);
}