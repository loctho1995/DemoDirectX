#include "Enemy.h"


Enemy::Enemy()
{

}

Enemy::~Enemy()
{
}

void Enemy::Update(float dt)
{
    Animation::Update(dt);
}

void Enemy::Draw(D3DXVECTOR3 position, RECT *sourceRect, D3DXVECTOR2 scale, D3DXVECTOR2 transform, float angle, D3DXVECTOR2 rotationCenter, D3DXVECTOR2 scalingCenter, D3DXCOLOR colorKey)
{
    if (mCamera)
    {
        D3DXVECTOR2 trans = D3DXVECTOR2(GameGlobal::GetWidth() / 2 - mCamera->GetPosition().x,
                                        GameGlobal::GetHeight() / 2 - mCamera->GetPosition().y);

        Animation::Draw(position, sourceRect, scale, trans, angle, rotationCenter, scalingCenter, colorKey);
    }
    else
        Animation::Draw(position, sourceRect, scale, transform, angle, rotationCenter, scalingCenter, colorKey);
}

void Enemy::SetGravity(D3DXVECTOR3 gravity)
{
    mGravity = gravity;
}

Enemy::EnemyName Enemy::GetName()
{
    return mName;
}

void Enemy::InitBound()
{
    Bound.left = this->GetPosition().x - this->getWidth() / 2;
    Bound.top = this->GetPosition().y - this->getHeight() / 2;
    Bound.right = Bound.left + this->getWidth();
    Bound.bottom = Bound.top + this->getHeight();

}