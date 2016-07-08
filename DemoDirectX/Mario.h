#ifndef __MARIO__
#define __MARIO__

#include "GameLog.h"
#include "Animation.h"
#include "QuadTree.h"
#include "GameMap.h"

#define JUMP_VELOCITY 8
#define JUMP_DELTA_VELOCITY 0.1 // do bien thien van toc
#define RUN_VELOCITY_KEY_PRESS 0.15
#define RUN_VELOCITY_KEY_HOLDING 2.0
#define TURNING_BACK_VELOCITY -0.8
#define SPEEDING_COUNT 40 //dung de cho viec tang toc mario, khi giu phim trai phai thi toc do mario se duoc tang dan


class Mario : public Entity
{

public:
    enum MarioStates
    {
        Running, //khi dang chay qua ben trai hoac phai
        Jumping, //khi o trang thai nhay len tu luc dung yen
        JumpingAndMoving, //dang nhay va vi chuyen (khi bam phim mui ten trai / phai)
        Standing, // dung yen khong dieu khien
        FallingDown, // bi roi xuong
        FallingDownAndMoving, //bi roi xuong va dieu khien boi 2 phim qua lai
        DroppingDown, //bi roi xuong khi gap vat can hoac la tu tren vi tri cao roi xuong
        DroppingDownAndMoving, //bi roi xuongva di chuyen
        TurningBack, //khi dang ben trai thi lap tuc queo phai va nguoc lai
        Blocked, //bi gap vat can luc nay animation van giu nguyen chi la khong cho di chuyen
    };

    //huong mat cua mario
    enum MarioSides
    {
        Left,  
        Right
    };

    Mario(GameMap *gameMap);
    ~Mario();

    void Update(float dt);
    void Draw();
    void LoadAnimations();

    void SetCamera(Camera *camera);
    Camera* GetCamera();

    void SetGameMap(GameMap *map);
    GameMap* GetGameMap();

    void SetGravity(D3DXVECTOR3 gravity);
    D3DXVECTOR3 GetGravity();

    float GetSpeedX();
    void SetSpeedX(float x);

    void SetAllowMove(bool flag);

    void ActiveSpeedKeyPress(); //toc do cham

    int GetWidth();
    int GetHeight();

    float GetSpeedXDefault();
    float GetSpeedYDefault();

    void SetCurrentVelocity(D3DXVECTOR3 velocity);
    D3DXVECTOR3 GetCurrentVelocity();

    void SetCurrentJumpVelocity(float velocity);
    float GetCurrentJumpVelocity();

    D3DXVECTOR3 GetPosition();

    void SetPosition(D3DXVECTOR3 pos);
    void SetPosition(float x, float y);

    void SetFlipVertical(bool flag);
    void SetFlipHorizontal(bool flag);

    void SetState(MarioStates status);
    MarioStates GetState();

    void SetSide(MarioSides side);
    MarioSides GetSide();

    void SetJumpHigh(float high);
    float GetJumpHigh();

    RECT GetBoundOnScreen();
    RECT GetBoundInWorld(); //vi tri thuc te cua mario

    void OnArrowLeftHolding();
    void OnArrowRightHolding();
    void OnSpaceHolding();
   
    void OnSpaceReleased();

    void DoMarioRunningAction();
    void DoMarioJumpingAction();
    void DoMarioFallingDownAction();

    bool IsCollideTop; //va cham phia ben tren Mario
    bool IsCollideBottom; //va cham phia duoi Mario
    bool IsCollideLeft; //va cham ben trai Mario
    bool IsCollideRight; //va cham ben phai mario

    //void SetCurrentVelocity();

private:
    D3DXVECTOR3                 mCurrentVelocity, mPosition;

    float                       mSpeedX, 
                                mSpeedYDefault,
                                mJumpVelocity, // van toc nhay len
                                mCurrentJumpVelocity, //luu do bien thien chieu cao hien tai
                                mJumpHigh; // chieu cao khi jump

    bool                        mIsFlipVertical,
                                mIsFlipHorizontal,
                                mIsAllowMove;      

    Camera                      *mCamera;
    GameMap                     *mMap;

    MarioStates                 mState;
    MarioSides                  mSide;

    Animation                   *mCurrentAnimation, 
                                *mAnimationRunning, 
                                *mAnimationStanding, 
                                *mAnimationJumping,                                               
                                *mAnimmationTurningBack;

    D3DXVECTOR3                 mGravity;
};

#endif
