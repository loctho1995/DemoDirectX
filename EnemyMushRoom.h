#ifndef __ENMEMY_MUSH_ROOM__
#define __ENMEMY_MUSH_ROOM__

#include <d3d9.h>
#include <d3dx9.h>

#include "Enemy.h"
#include "GameGlobal.h"
#include "Animation.h"
#include "Camera.h"

#define MUSH_ROOM_SPEED 1 //pixel / (1/ 60s)
#define MUSH_ROOM_ANIMATION_TIME 0.4

class EnemyMushRoom : public Enemy
{
public:
    enum MushroomStates
    {
        RunningLeft, //chay qua ben trai
        RunningRight, //chay qua ben phai
        Died //chet
    };

    MushroomStates State;

    void Update(float dt);

    EnemyMushRoom(float x, float y, Camera *camera);
    EnemyMushRoom(D3DXVECTOR3 position,Camera *camera);
    void ChangeState();
    ~EnemyMushRoom();

private:
};

#endif
