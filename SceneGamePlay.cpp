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
}

void SceneGamePlay::Update(float dt)
{
    //xu ly cac su kien ban phim
    mGameScene->Update(dt);
}

void SceneGamePlay::Draw()
{
    mGameScene->Draw();
}

void SceneGamePlay::OnKeyDown(int keyCode)
{
    mGameScene->OnKeyDown(keyCode);
}

void SceneGamePlay::OnKeyUp(int keyCode)
{
    mGameScene->OnKeyUp(keyCode);
}