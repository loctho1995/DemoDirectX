#include "Mario.h"

Mario::Mario(GameMap *gameMap)
{
    mCurrentAnimation = nullptr; //  new Animation();
    //mCurrentAnimation->SetFlipVertical(true);
    mIsFlipHorizontal = mIsFlipVertical = false;
    mPosition = D3DXVECTOR3(0, 0, 0); //mCurrentAnimation->GetPosition();


    //khoi tao dung ham initwithanimation
    this->mMap = gameMap;

    mCurrentVelocity = D3DXVECTOR3(0, 0, 0);
    mGravity = D3DXVECTOR3(0, 5, 0);
    mSpeedX = RUN_VELOCITY_KEY_PRESS; //RUN_VELOCITY_KEY_HOLDING;
    mCurrentJumpVelocity = JUMP_VELOCITY;
    mJumpHigh = 350; //cho phep nhay cao them 80 pixel
    mJumpVelocity = 8;
    mState = Mario::MarioStates::Standing;
    mSide = Mario::MarioSides::Right;

    LoadAnimations();

    IsCollideTop = IsCollideBottom = IsCollideLeft = IsCollideRight = false;
    this->Bound.left = (mCurrentAnimation)->GetPosition().x - (mCurrentAnimation)->getWidth() / 2;
    this->Bound.right = this->Bound.left + (mCurrentAnimation)->getWidth();
    this->Bound.top = (mCurrentAnimation)->GetPosition().y - (mCurrentAnimation)->getHeight() / 2;
    this->Bound.bottom = this->Bound.top + (mCurrentAnimation)->getHeight();
}

Mario::~Mario()
{
    
}

void Mario::SetGravity(D3DXVECTOR3 gravity)
{
    this->mGravity = gravity;
}

D3DXVECTOR3 Mario::GetGravity()
{
    return mGravity;
}

void Mario::SetPosition(D3DXVECTOR3 pos)
{
    mPosition = pos;
    mCurrentAnimation->SetPosition(mPosition);

    Bound = GetBoundInWorld();
}

void Mario::SetPosition(float x, float y)
{
    mPosition = D3DXVECTOR3(x, y, 0);
    (mCurrentAnimation)->SetPosition(mPosition);
    Bound = GetBoundInWorld();
}

void Mario::SetFlipVertical(bool flag)
{
    mIsFlipVertical = flag;
}

void Mario::SetAllowMove(bool flag)
{
    mIsAllowMove = flag;
}

void Mario::SetFlipHorizontal(bool flag)
{
    mIsFlipHorizontal = flag;
}

int Mario::GetWidth()
{
    return (mCurrentAnimation)->getWidth();
}

int Mario::GetHeight()
{
    return (mCurrentAnimation)->getHeight();
}

void Mario::SetCurrentVelocity(D3DXVECTOR3 velocity)
{
    mCurrentVelocity = velocity;
}

D3DXVECTOR3 Mario::GetCurrentVelocity()
{
    return mCurrentVelocity;
}

void Mario::SetCurrentJumpVelocity(float velocity)
{
    mCurrentJumpVelocity = velocity;
}

float Mario::GetCurrentJumpVelocity()
{
    return mCurrentJumpVelocity;
}

float Mario::GetSpeedX()
{
    return mSpeedX;
}

void Mario::SetSpeedX(float x)
{
    mSpeedX = x;
}

void Mario::ActiveSpeedKeyPress()
{
    this->SetSpeedX(RUN_VELOCITY_KEY_PRESS);
}

void Mario::SetGameMap(GameMap *map)
{
    mMap = map;
}

GameMap* Mario::GetGameMap()
{
    return mMap;
}

float Mario::GetSpeedYDefault()
{
    return mSpeedYDefault;
}

void Mario::SetState(MarioStates state)
{
    mState = state;

    switch (state)
    {
    case MarioStates::Running:
        //this->SetSpeedX(RUN_VELOCITY_KEY_HOLDING);
        if (this->GetSpeedX() > 0)
            mCurrentAnimation = mAnimationRunning;
        break;

    case MarioStates::Jumping:case MarioStates::JumpingAndMoving:
        mCurrentJumpVelocity = JUMP_VELOCITY;
        mCurrentAnimation = mAnimationJumping;
        break;

    case MarioStates::Standing: 
        mCurrentAnimation = mAnimationStanding;
        mCurrentJumpVelocity = 0;
        break;

    case MarioStates::FallingDown:
        mCurrentAnimation = mAnimationJumping;
        //mSpeedX = 0;
        break;

    case MarioStates::FallingDownAndMoving:
        mSpeedX = RUN_VELOCITY_KEY_HOLDING;
        mCurrentAnimation = mAnimationJumping;
        break;

    case MarioStates::DroppingDown: case MarioStates::DroppingDownAndMoving:
        mCurrentAnimation = mAnimationJumping;
        break;

    case MarioStates::TurningBack:
        this->SetSpeedX(TURNING_BACK_VELOCITY);
        mCurrentAnimation = mAnimmationTurningBack;
        break;
    }
}

Mario::MarioStates Mario::GetState()
{
    return mState;
}

void Mario::SetSide(MarioSides side)
{
    mSide = side;
}

Mario::MarioSides Mario::GetSide()
{
    return mSide;
}

void Mario::SetJumpHigh(float high)
{
    mJumpHigh = high;
}

float Mario::GetJumpHigh()
{
    return mJumpHigh;
}

RECT Mario::GetBoundOnScreen()
{
    D3DXVECTOR2 trans = D3DXVECTOR2(GameGlobal::GetWidth() / 2 - mCamera->GetPosition().x,
                                    GameGlobal::GetHeight() / 2 - mCamera->GetPosition().y);

    RECT bound;

    bound.left = mPosition.x - (mCurrentAnimation)->getWidth() / 2 + trans.x;
    bound.right = bound.left + (mCurrentAnimation)->getWidth();
    bound.top = mPosition.y - (mCurrentAnimation)->getHeight() / 2 + trans.y;
    bound.bottom = bound.top + (mCurrentAnimation)->getHeight();

    return bound;
}

RECT Mario::GetBoundInWorld()
{
    RECT bound;
    bound.left = mPosition.x - (mCurrentAnimation)->getWidth() / 2;
    bound.right = bound.left + (mCurrentAnimation)->getWidth();
    bound.top = mPosition.y - (mCurrentAnimation)->getHeight() / 2;
    bound.bottom = bound.top + (mCurrentAnimation)->getHeight();

    return bound;
}

void Mario::SetCamera(Camera *camera)
{
    mCamera = camera;
}

Camera* Mario::GetCamera()
{
    return mCamera;
}

D3DXVECTOR3 Mario::GetPosition()
{
    return mPosition;
}

void Mario::Draw()
{
    mCurrentAnimation->SetFlipHorizontal(mIsFlipHorizontal);
    mCurrentAnimation->SetFlipVertical(mIsFlipVertical);
    mCurrentAnimation->SetPosition(mPosition);

    if (mCamera)
    {
        //doi hinh mario tu vi tri trong real world sang man hinh
        D3DXVECTOR2 trans = D3DXVECTOR2(GameGlobal::GetWidth() / 2 - this->mCamera->GetPosition().x,
            GameGlobal::GetHeight() / 2 - this->mCamera->GetPosition().y);

        (mCurrentAnimation)->Draw(mPosition, NULL, D3DXVECTOR2(), trans);
    }
    else
        (mCurrentAnimation)->Draw();
}

void Mario::Update(float dt)
{
    //GAMELOG("State: %d", mState);

    switch (mState)
    {
    case MarioStates::Running:
        DoMarioRunningAction();
        break;


    case MarioStates::Jumping:
        DoMarioJumpingAction();
        break;

    case MarioStates::JumpingAndMoving:
        DoMarioRunningAction();
        DoMarioJumpingAction();          
        break;

    case DroppingDownAndMoving:
        DoMarioRunningAction();
        break;

    case MarioStates::Standing:
        break;

    case MarioStates::FallingDown: 
        DoMarioFallingDownAction();
        break;

    case MarioStates::FallingDownAndMoving:
        DoMarioFallingDownAction();
        DoMarioRunningAction();
        break;

    case MarioStates::TurningBack:
        DoMarioRunningAction();
        break;
    }


    (mCurrentAnimation)->Update(dt);
}

void Mario::LoadAnimations()
{
    mAnimationStanding = new Animation(GameGlobal::GetCurrentSpriteHandler(), L"mario/standingright.png", 1, 1, 1, 0);
    mAnimationJumping = new Animation(GameGlobal::GetCurrentSpriteHandler(), L"mario/jumpingright.png", 1, 1, 1, 0);
    mAnimationRunning = new Animation(GameGlobal::GetCurrentSpriteHandler(), L"mario/runningright.png", 2, 1, 2, 0.15f);
    mAnimmationTurningBack = new Animation(GameGlobal::GetCurrentSpriteHandler(), L"mario/turnback.png", 1, 1, 1, 0);

    mCurrentAnimation = mAnimationStanding;
}

void Mario::OnArrowLeftHolding()
{
    if (this->GetState() == MarioStates::FallingDown)
        this->SetState(MarioStates::FallingDownAndMoving);
    else if (this->GetState() == MarioStates::Standing)
        this->SetState(MarioStates::Running);
    else if (this->GetState() == MarioStates::DroppingDown)
        this->SetState(MarioStates::DroppingDownAndMoving);

    if (this->GetSide() == MarioSides::Right)
    {
        if (this->GetState() == MarioStates::Running)
            this->SetState(MarioStates::TurningBack);
    }

    this->SetSide(MarioSides::Left);
}

void Mario::OnArrowRightHolding()
{
    //tuong tu voi arrowleft nhung nguoc lai
    if (this->GetState() == MarioStates::FallingDown) //dang roi xuong thi cho phep roi xuong va di chuyen
        this->SetState(MarioStates::FallingDownAndMoving);
    else if (this->GetState() == MarioStates::Standing) //dang dung yen thi cho phep chay
        this->SetState(MarioStates::Running);
    else if (this->GetState() == MarioStates::DroppingDown) //dang bi drop xuong thi cho phep di chuyen theo
        this->SetState(MarioStates::DroppingDownAndMoving);

    if (this->GetSide() == MarioSides::Left)
    {
        if (this->GetState() == MarioStates::Running)
            this->SetState(MarioStates::TurningBack);
    }

    this->SetSide(MarioSides::Right);
}

void Mario::OnSpaceHolding()
{    
    if (mState == MarioStates::Standing || mState == MarioStates::Running)
        this->SetState(MarioStates::Jumping);
}

void Mario::OnSpaceReleased()
{
    this->SetState(MarioStates::FallingDown);
}

void Mario::DoMarioRunningAction()
{
    if (!mIsAllowMove)
        return;

    if (this->GetSide() == MarioSides::Left)
    {
#pragma region - HANDLE RUNNING LEFT-
        //ham su ly mario khi phim mui ten trai duoc bam
        if (this->GetPosition().x - this->GetWidth() / 2 > 0)
        {
            //truoc hop nay khi camera phia goc ben trai
            //luc nay camera se dung yen va chi co mario di chuyen
            this->SetPosition(this->GetPosition() + D3DXVECTOR3(-this->GetSpeedX(), 0, 0));
        }
        else
        {
            //truoc hop nay khi nhan nam o vi tri tu giua man hinh ben trai cho den phia ben trai
            //mario se dung giua man hinh khi den vi tri giua man hinh, luc nay camera se di chuyen theo toa do mario
            if (this->GetPosition().x - this->GetWidth() / 2 < 0)
                this->SetPosition(this->GetWidth() / 2, this->GetPosition().y);
        }
#pragma endregion
    }
    else
    {
#pragma region - HANDLE RUNNING RIGHT -

        if (this->GetPosition().x + this->GetWidth() / 2 <= mMap->GetWidth())
        {
            this->SetPosition(this->GetPosition() + D3DXVECTOR3(this->GetSpeedX(), 0, 0));
            mCamera->SetPosition(this->GetPosition());
        }
        else
        {
            if (this->GetPosition().x + this->GetWidth() / 2 > mMap->GetWidth())
            {
                this->SetPosition(mMap->GetWidth() - this->GetWidth() / 2, this->GetPosition().y);
            }
        }
#pragma endregion

    }

    if (this->GetSpeedX() < RUN_VELOCITY_KEY_HOLDING)
        this->SetSpeedX(this->GetSpeedX() + RUN_VELOCITY_KEY_HOLDING / SPEEDING_COUNT);

    if (this->GetState() == MarioStates::Running)
    {
        if (this->GetSpeedX() > 0)
            mCurrentAnimation = mAnimationRunning;
        else
            mCurrentAnimation = mAnimmationTurningBack;
    }
}

void Mario::DoMarioJumpingAction()
{
#pragma region - HANDLE JUMPING -

    if (mCurrentJumpVelocity > 0)
    {
        this->SetPosition(this->GetPosition() + D3DXVECTOR3(0, -mCurrentJumpVelocity, 0));
        mCurrentJumpVelocity -= JUMP_DELTA_VELOCITY;
    }

    if (mCurrentJumpVelocity <= mGravity.y)
        this->SetState(FallingDown);
#pragma endregion
}

void Mario::DoMarioFallingDownAction()
{
#pragma region - HANDLE FALLING DOWN - 
    if (mCurrentJumpVelocity > 0)
    {
        this->SetPosition(this->GetPosition() + D3DXVECTOR3(0, -mCurrentJumpVelocity, 0));

        mCurrentJumpVelocity -= JUMP_DELTA_VELOCITY * 2;
    }

    //truong hop roi xuong va bam phim trai phai
#pragma endregion
}