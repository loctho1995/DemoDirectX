#include "Scene.h"


Scene::Scene()
{
    mSpriteHandler = GameGlobal::GetCurrentSpriteHandler();
    mBackColor = 0x4866ff;
    LoadContent();
}


Scene::~Scene()
{

}

void Scene::Update(float dt)
{

}

void Scene::LoadContent()
{
    if (!mSpriteHandler)
        mSpriteHandler = GameGlobal::GetCurrentSpriteHandler();
}

void Scene::Draw()
{

}

void Scene::OnKeyDown(int keyCode)
{

}

void Scene::OnKeyUp(int keyCode)
{
    
}

bool Scene::IsKeyPress(int keyCode)
{
    return GameInput::GetInstance()->IsKeyDown(keyCode);
}

D3DCOLOR Scene::GetBackcolor()
{
    return mBackColor;
}