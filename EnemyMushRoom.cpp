#include "EnemyMushRoom.h"

EnemyMushRoom::EnemyMushRoom(float x, float y, Camera *camera)
{
    this->InitWithAnimation("mario/enemies/mushroom.png", 2, 1, 2, MUSH_ROOM_ANIMATION_TIME);
    this->SetPosition(x, y);
    State = MushroomStates::RunningLeft;
    mName = Enemy::EnemyName::MushRoom;
    mCamera = camera;

    InitBound();
}

EnemyMushRoom::EnemyMushRoom(D3DXVECTOR3 position, Camera *camera)
{
    EnemyMushRoom(position.x, position.y, camera);
}

EnemyMushRoom::~EnemyMushRoom()
{
    
}

void EnemyMushRoom::Update(float dt)
{
    switch (State)
    {
        case MushroomStates::RunningLeft:
            this->SetPosition(this->GetPosition() + D3DXVECTOR3(-MUSH_ROOM_SPEED, 0, 0));
            break;

        case MushroomStates::RunningRight:
            this->SetPosition(this->GetPosition() + D3DXVECTOR3(MUSH_ROOM_SPEED, 0, 0));
            break;

        case Died:
            break;

        default:
            break;
    }

    InitBound();
    Enemy::Update(dt);
}

void EnemyMushRoom::ChangeState()
{
    if (State == RunningLeft)
        State = RunningRight;
    else
        State = RunningLeft;
}