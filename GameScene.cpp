#include "GameScene.h"


GameScene::GameScene(char *mapfilePath)
{
    mMapPath = mapfilePath;

}

GameScene::~GameScene()
{

}

void GameScene::LoadContent()
{
    mTimeArrowLeftHolding = mTimeArrowRightHolding = mTimeSpaceHolding = 0;

    mGravity = D3DXVECTOR3(0, 3, 0);
    //load map

    mGameMap = new GameMap(GameGlobal::GetCurrentSpriteHandler(), mMapPath);
    mCamera = new Camera(GameGlobal::GetWidth() / 2, GameGlobal::GetHeight() / 2);
    mCamera->SetPosition(mCamera->GetWidth() / 2, mGameMap->GetHeight() - mCamera->GetHeight() / 2);
    //chỉnh tọa độ camera cho vị trí góc dưới map
    //vị trí của camera tương ứng với vị trí trong thế giới thực, tuy hiên position là vị trí tâm của camera
    mGameMap->SetCamera(mCamera);

    //quadtree cho map
    RECT quadBound;
    quadBound.left = quadBound.top = 0;
    quadBound.right = mGameMap->GetWidth();
    quadBound.bottom = mGameMap->GetHeight();
    mQuadTree = new QuadTree(1, quadBound);

    mMario = new Mario(mGameMap);
    mMario->SetPosition(60, mGameMap->GetHeight() - 200);
    mMario->SetCamera(mCamera);
    mMario->SetGravity(mGravity);
    mMario->Tag = Entity::EntityTypes::Mario;
    mMario->SetState(Mario::MarioStates::Standing);

    mCamera->SetPosition(mMario->GetPosition());

    RECT sourceRECT; //vien gach
    sourceRECT.left = 32; //toa do x trong tileset
    sourceRECT.top = 0; //toa do y trong tileset
    sourceRECT.right = sourceRECT.left + 32;
    sourceRECT.bottom = sourceRECT.top + 32;

    mSpriteBricks = new Sprite(GameGlobal::GetCurrentSpriteHandler(), L"mariotileset.png", &sourceRECT);

    //brick gold
    sourceRECT.left = 32 * 24; //toa do x trong tileset
    sourceRECT.top = 0; //toa do y trong tileset
    sourceRECT.right = sourceRECT.left + 32;
    sourceRECT.bottom = sourceRECT.top + 32;
    mSpriteBrickGold = new Sprite(GameGlobal::GetCurrentSpriteHandler(), L"mariotileset.png", &sourceRECT);

    mGameDebugDraw = new GameDebugDraw(GameGlobal::GetCurrentSpriteHandler());

    //mListEnemies.push_back(new EnemyMushRoom(758, mGameMap->GetHeight() - 80, mCamera));
    mListEnemies.push_back(new EnemyMushRoom(1312, mGameMap->GetHeight() - 80, mCamera));
    
    LoadMapData();
}

void GameScene::Update(float dt)
{
    HandleInput(dt);

    mMario->Update(dt);
    mGameMap->Update(dt);
    HandleEnemiesCollision();

    for (size_t i = 0; i < mListEnemies.size(); i++)
    {
        if (mListEnemies.at(i)->GetName() == Enemy::EnemyName::MushRoom)
            ((EnemyMushRoom*)mListEnemies.at(i))->Update(dt);
    }

    //ben duoi khong co vat nao
    if (!MarioCheckBottomCollide())
    {
        mMario->SetPosition(mMario->GetPosition() + mGravity);

        //truong hop dang tu tren cao roi xuong thap thi se khong cho bam phim space va doi trang thai
        if (mMario->GetState() == Mario::MarioStates::Running)
            mMario->SetState(Mario::MarioStates::DroppingDown);
    }
    else if (mMario->GetState() != Mario::MarioStates::Standing && mMario->GetState() != Mario::MarioStates::Running)
    {
        mMario->SetState(Mario::MarioStates::Standing);
    }

    //kiem tra vat can ben tren, neu gap vat can thi chuyen trang thai
    if (MarioCheckTopCollide())
        mMario->SetState(Mario::MarioStates::DroppingDown);

    mCamera->SetPosition(mMario->GetPosition());
    CheckCameraAndWorldMap();
}

void GameScene::Draw()
{
    for (size_t i = 0; i < mListBrick.size(); i++)
    {
        D3DXVECTOR2 trans = D3DXVECTOR2(GameGlobal::GetWidth() / 2 - mCamera->GetPosition().x,
            GameGlobal::GetHeight() / 2 - mCamera->GetPosition().y);

        mSpriteBricks->Draw(mListBrick.at(i)->Position, NULL, D3DXVECTOR2(), trans);

        //mGameDebugDraw->DrawRect(mListBrick.at(i)->GetBoundOnScreen(mCamera));
    }

    for (size_t i = 0; i < mListBrickGold.size(); i++)
    {
        D3DXVECTOR2 trans = D3DXVECTOR2(GameGlobal::GetWidth() / 2 - mCamera->GetPosition().x,
            GameGlobal::GetHeight() / 2 - mCamera->GetPosition().y);

        mSpriteBrickGold->Draw(mListBrickGold.at(i)->Position, NULL, D3DXVECTOR2(), trans);
    }

    mGameMap->Draw();
    mMario->Draw();

    for (size_t i = 0; i < mListEnemies.size(); i++)
    {

        mListEnemies.at(i)->Draw();
    }
}

void GameScene::LoadMapData()
{
    //khoi tao cac khoi Brick (vien gach)
#pragma region -BRICK AND COIN LAYER-
    for (size_t i = 0; i < mGameMap->GetMap()->GetNumTileLayers(); i++)
    {
        const Tmx::TileLayer *layer = mGameMap->GetMap()->GetTileLayer(i);

        if (layer->IsVisible())
            continue;

        //xac dinh layer Brick bi an di de tu do tao ra cac vien gach trong game, nhung vien gach khong phai la 1 physic static nos co the bi pha huy duoc

        if (layer->GetName() == "brick" || layer->GetName() == "coin")
        {
            for (size_t j = 0; j < mGameMap->GetMap()->GetNumTilesets(); j++)
            {
                const Tmx::Tileset *tileSet = mGameMap->GetMap()->GetTileset(j);

                int tileWidth = mGameMap->GetMap()->GetTileWidth();
                int tileHeight = mGameMap->GetMap()->GetTileHeight();

                int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth;
                int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight;

                for (size_t m = 0; m < layer->GetHeight(); m++)
                {
                    for (size_t n = 0; n < layer->GetWidth(); n++)
                    {
                        if (layer->GetTileTilesetIndex(n, m) != -1)
                        {
                            int tileID = layer->GetTileId(n, m);

                            int y = tileID / tileSetWidth;
                            int x = tileID - y * tileSetWidth;

                            RECT sourceRECT;
                            sourceRECT.top = y * tileHeight;
                            sourceRECT.bottom = sourceRECT.top + tileHeight;
                            sourceRECT.left = x * tileWidth;
                            sourceRECT.right = sourceRECT.left + tileWidth;

                            RECT bound;
                            bound.left = n * tileWidth;
                            bound.top = m * tileHeight;
                            bound.right = bound.left + tileWidth;
                            bound.bottom = bound.top + tileHeight;

                            D3DXVECTOR3 position(n * tileWidth + tileWidth / 2, m * tileHeight + tileHeight / 2, 0);

                            Brick *brick = new Brick(bound, &sourceRECT, position);

                            if (layer->GetName() == "coin")
                            {
                                brick->Tag = Entity::EntityTypes::BrickGoldNormal;
                                mListBrickGold.push_back(brick);
                            }
                            else
                            {
                                brick->Tag = Entity::EntityTypes::Brick;
                                mListBrick.push_back(brick);
                            }

                            mQuadTree->insertEntity(brick);
                        }
                    }
                }
            }
        }
    }

#pragma endregion

#pragma region -OBJECTGROUP, STATIC OBJECT-

    for (size_t i = 0; i < mGameMap->GetMap()->GetNumObjectGroups(); i++)
    {
        const Tmx::ObjectGroup *objectGroup = mGameMap->GetMap()->GetObjectGroup(i);

        for (size_t j = 0; j < objectGroup->GetNumObjects(); j++)
        {
            Tmx::Object *object = objectGroup->GetObjects().at(j);

            Entity *entity = new Entity();
            entity->Bound.left = object->GetX();
            entity->Bound.right = entity->Bound.left + object->GetWidth();
            entity->Bound.top = object->GetY();
            entity->Bound.bottom = entity->Bound.top + object->GetHeight();
            entity->Tag = Entity::EntityTypes::Static;

            mQuadTree->insertEntity(entity);

        }
    }
#pragma endregion
}

std::vector<Enemy*> GameScene::GetListEnemies()
{
    return mListEnemies;
}

std::vector<Brick*> GameScene::GetListBrick()
{
    return mListBrick;
}

std::vector<Brick*> GameScene::GetListBrickGold()
{
    return mListBrickGold; 
}

GameMap* GameScene::GetMap()
{
    return mGameMap;
}

void GameScene::SetCamera(Camera *camera)
{
    mCamera = camera;
}

Camera* GameScene::GetCamera()
{
    return mCamera;
}

std::vector<Entity*> GameScene::MarioGetImpactor()
{
    mMario->IsCollideBottom = mMario->IsCollideTop = mMario->IsCollideLeft = mMario->IsCollideRight = false;
    //81 entities
    std::vector<Entity*> collideAble;
    std::vector<Entity*> out;

    mQuadTree->getEntitiesCollideAble(collideAble, mMario);

    for (size_t i = 0; i < collideAble.size(); i++)
    {
        collideAble.at(i)->CollisionResult = GameCollision::RectangleAndRectangle(collideAble.at(i)->Bound, mMario->Bound);

        if (collideAble.at(i)->CollisionResult.IsCollided)
        {
            out.push_back(collideAble.at(i));
        }
    }

    return out;
}

void GameScene::HandleInput(float dt)
{
    //GAMELOG("Mario state: %d", mMario->GetState());

    //if (mCamera->GetPosition().x - mCamera->GetWidth() / 2 <=)
    if (IsKeyPress(DIK_LEFTARROW) && IsKeyPress(DIK_RIGHTARROW)
        && mTimeArrowLeftHolding + mTimeArrowRightHolding >= TIME_KEY_HOLDING * 2)
    {
        mMario->SetState(Mario::MarioStates::Standing);
        return;
    }

#pragma region - HANLE ARROW RIGHT -

    if (IsKeyPress(DIK_RIGHTARROW))
    {
        if (mTimeArrowRightHolding >= TIME_KEY_HOLDING)
            MarioArrowRightHolding();
        else
            mTimeArrowRightHolding += dt;
    }
    else
        mTimeArrowRightHolding = 0;

#pragma endregion

#pragma region - HANDLE ARROW LEFT -

    if (IsKeyPress(DIK_LEFTARROW))
    {
        if (mTimeArrowLeftHolding >= TIME_KEY_HOLDING)
            MarioArrowLeftHolding();
        else
            mTimeArrowLeftHolding += dt;
    }
    else mTimeArrowLeftHolding = 0;

#pragma endregion

#pragma region - HANDLE UPARROW -
    if (IsKeyPress(DIK_UPARROW))
    {
        MarioArrowUpHolding();
    }
#pragma endregion

#pragma region - HANDLE DOWNARROW -
    if (IsKeyPress(DIK_DOWNARROW))
    {
        MarioArrowDownHolding();
    }
#pragma endregion

#pragma region - STANDING STATE -

    if (!IsKeyPress(DIK_LEFTARROW) && !IsKeyPress(DIK_RIGHTARROW) && !IsKeyPress(DIK_SPACE))
    {
        if (mMario->GetState() != Mario::MarioStates::Standing && mMario->GetState() != Mario::MarioStates::FallingDown
            && mMario->GetState() != Mario::MarioStates::DroppingDown
            && mMario->GetState() != Mario::MarioStates::DroppingDownAndMoving)
            mMario->SetState(Mario::MarioStates::Standing);
    }
#pragma endregion

}

bool GameScene::IsKeyPress(int keyCode)
{
    return (GameInput::GetInstance()->IsKeyDown(keyCode));
}

void GameScene::OnKeyDown(int keyCode)
{
    switch (keyCode)
    {
    case DIK_UPARROW:

        break;

    case DIK_DOWNARROW:

        break;

    case DIK_LEFTARROW:
        //khi bam phim thi cho mario di chuyen 1 doan nho

        if (mMario->GetState() == Mario::MarioStates::Standing)
        {
            mMario->SetState(Mario::MarioStates::Running);
            mMario->ActiveSpeedKeyPress();
            mMario->SetSide(Mario::MarioSides::Left);
        }

        break;

    case DIK_RIGHTARROW:

        if (mMario->GetState() == Mario::MarioStates::Standing)
        {
            mMario->SetState(Mario::MarioStates::Running);
            mMario->ActiveSpeedKeyPress();
            mMario->SetSide(Mario::MarioSides::Right);
        }

        break;

    case DIK_SPACE:
        //vua bam phim space vua co bam phim qua lai
        if (IsKeyPress(DIK_LEFTARROW))
        {
            mMario->SetFlipVertical(true);
            mMario->SetSide(Mario::MarioSides::Left);

            if (!MarioCheckLeftCollide() && !MarioCheckTopCollide())
            {
                if (mMario->GetState() == Mario::MarioStates::Jumping || mMario->GetState() == Mario::MarioStates::Running)
                    mMario->SetState(Mario::MarioStates::JumpingAndMoving);
            }
        }
        else if (IsKeyPress(DIK_RIGHTARROW))
        {
            mMario->SetFlipVertical(false);
            mMario->SetSide(Mario::MarioSides::Right);

            if (!MarioCheckRightCollide() && !MarioCheckTopCollide())
            {
                if (mMario->GetState() == Mario::MarioStates::Jumping || mMario->GetState() == Mario::MarioStates::Running)
                    mMario->SetState(Mario::MarioStates::JumpingAndMoving);
            }
        }

        mMario->OnSpaceHolding();

        break;

    default:
        break;
    }
}

void GameScene::OnKeyUp(int keyCode)
{
    if (keyCode == DIK_SPACE)
        mMario->OnSpaceReleased();
}

bool GameScene::MarioCheckLeftCollide()
{
    std::vector<Entity*> collides = MarioGetImpactor();

    if (collides.size() > 0)
    {
        for (std::vector<Entity*>::iterator entity = collides.begin(); entity != collides.end(); entity++)
        {
            if ((*entity)->Tag == Entity::EntityTypes::Brick || (*entity)->Tag == Entity::EntityTypes::Static)
            {
                RECT result = (*entity)->CollisionResult.RegionCollision;
                int width = result.right - result.left;
                int height = result.bottom - result.top;

                if (height >= mMario->GetHeight() / 3 && result.right < mMario->Bound.right)
                    return true;
            }
        }
    }

    return false;
}

bool GameScene::MarioCheckRightCollide()
{
    std::vector<Entity*> collides = MarioGetImpactor();

    if (collides.size() > 0)
    {
        for (std::vector<Entity*>::iterator entity = collides.begin(); entity != collides.end(); entity++)
        {
            if ((*entity)->Tag == Entity::EntityTypes::Brick || (*entity)->Tag == Entity::EntityTypes::Static)
            {
                RECT result = (*entity)->CollisionResult.RegionCollision;
                int width = result.right - result.left;
                int height = result.bottom - result.top;

                if (height >= mMario->GetHeight() / 3 && result.left > mMario->Bound.left)
                    return true;
            }
        }
    }

    return false;
}

bool GameScene::MarioCheckBottomCollide()
{
    std::vector<Entity*> collides = MarioGetImpactor();

    if (collides.size() > 0)
    {
        for (std::vector<Entity*>::iterator entity = collides.begin(); entity != collides.end(); entity++)
        {
            if ((*entity)->Tag == Entity::EntityTypes::Brick
                || (*entity)->Tag == Entity::EntityTypes::Static
                || (*entity)->Tag == Entity::EntityTypes::BrickGoldNormal
                || (*entity)->Tag == Entity::EntityTypes::BrickGoldEated)
            {
                RECT result = (*entity)->CollisionResult.RegionCollision;
                int width = result.right - result.left;
                int height = result.bottom - result.top;

                if (height <= 2 && width >= mMario->GetWidth() / 3 && result.top > mMario->Bound.top)
                    return true;
            }
        }
    }

    return false;
}

bool GameScene::MarioCheckTopCollide()
{
    std::vector<Entity*> collides = MarioGetImpactor();

    if (collides.size() > 0)
    {
        for (std::vector<Entity*>::iterator entity = collides.begin(); entity != collides.end(); entity++)
        {
            if ((*entity)->Tag == Entity::EntityTypes::Brick
                || (*entity)->Tag == Entity::EntityTypes::Static
                || (*entity)->Tag == Entity::EntityTypes::BrickGoldNormal
                || (*entity)->Tag == Entity::EntityTypes::BrickGoldEated)
            {
                RECT result = (*entity)->CollisionResult.RegionCollision;
                int width = result.right - result.left;
                int height = result.bottom - result.top;

                if (height <= 5 && width >= mMario->GetWidth() / 3 && result.bottom < mMario->Bound.bottom)
                    return true;
            }
        }
    }

    return false;
}

void GameScene::CheckCameraAndWorldMap()
{
    if (mCamera->GetBound().left < 0)
    {
        //vi position cua camera ma chinh giua camera
        //luc nay o vi tri goc ben trai cua the gioi thuc
        mCamera->SetPosition(mCamera->GetWidth() / 2, mCamera->GetPosition().y);
    }

    if (mCamera->GetBound().right > mGameMap->GetWidth())
    {
        //luc nay cham goc ben phai cua the gioi thuc
        mCamera->SetPosition(mGameMap->GetWidth() - mCamera->GetWidth() / 2, mCamera->GetPosition().y);
    }

    if (mCamera->GetBound().top < 0)
    {
        //luc nay cham goc tren the gioi thuc
        mCamera->SetPosition(mCamera->GetPosition().x, mCamera->GetHeight() / 2);
    }

    if (mCamera->GetBound().bottom > mGameMap->GetHeight())
    {
        //luc nay cham day cua the gioi thuc
        mCamera->SetPosition(mCamera->GetPosition().x, mGameMap->GetHeight() - mCamera->GetHeight() / 2);
    }
}

void GameScene::MarioArrowRightHolding()
{
    mMario->SetFlipVertical(false);

    if (MarioCheckRightCollide())
    {
        mMario->SetAllowMove(false);
        return;
    }

    mMario->SetAllowMove(true);
    mMario->OnArrowRightHolding();
}

void GameScene::MarioArrowLeftHolding()
{
    mMario->SetFlipVertical(true);

    if (MarioCheckLeftCollide())
    {
        mMario->SetAllowMove(false);
        return;
    }

    mMario->SetAllowMove(true);
    mMario->OnArrowLeftHolding();
}

void GameScene::MarioArrowUpHolding()
{
    //ham su ly mario khi phim mui ten len duoc bam
    //if (mMario->GetPosition().y < GameGlobal::GetHeight() / 2)
    //    mMario->SetPosition(mMario->GetPosition() + D3DXVECTOR3(0, -mMario->GetSpeedY(), 0));
    //else
    //    mCamera->SetPosition(mCamera->GetPosition() + D3DXVECTOR3(0, -mMario->GetSpeedY(), 0));
}

void GameScene::MarioArrowDownHolding()
{
    //ham su ly mario khi phim mui ten xuong duoc bam
    //if (mMario->GetPosition().y < GameGlobal::GetHeight() / 2)
    //    mCamera->SetPosition(mCamera->GetPosition() + D3DXVECTOR3(0, mMario->GetSpeedY(), 0));
    //else
    //mMario->SetPosition(mMario->GetPosition() + D3DXVECTOR3(0, mMario->GetSpeedY(), 0));
}

void GameScene::MarioSpaceHolding()
{
    mMario->OnSpaceHolding();
}

void GameScene::HandleEnemiesCollision()
{
    for (size_t i = 0; i < mListEnemies.size(); i++)
    {
        Enemy *enemy = mListEnemies.at(i);
        std::vector<Entity*> out;

        switch (enemy->GetName())
        {
        case Enemy::EnemyName::MushRoom:
            mQuadTree->getEntitiesCollideAble(out, enemy);

            for (size_t j = 0; j < out.size(); j++)
            {
                GameCollision::CollisionReturn CollisionResult = GameCollision::RectangleAndRectangle(enemy->Bound, out.at(j)->Bound);

                if (CollisionResult.IsCollided)
                {
                    int height = CollisionResult.RegionCollision.bottom - CollisionResult.RegionCollision.top;
                    int width = CollisionResult.RegionCollision.right - CollisionResult.RegionCollision.left;

                    if (height >= enemy->getHeight() / 2)
                    {
                        ((EnemyMushRoom*)enemy)->ChangeState();

                        //GAMELOG("State: %d", ((EnemyMushRoom*)enemy)->State);
                    }
                }
            }
            break;

        default:
            break;
        }
    }
}

void GameScene::drawQuadTree(QuadTree *quadtree)
{
    if (quadtree->Nodes)
    {
        for (size_t i = 0; i < 4; i++)
        {
            drawQuadTree(quadtree->Nodes[i]);
        }
    }

    //mGameDebugDraw->DrawRect(quadtree->Bound);

    for (size_t i = 0; i < quadtree->ListEntity.size(); i++)
    {
        mGameDebugDraw->DrawRect(quadtree->ListEntity.at(i)->GetBoundOnScreen(mCamera));
    }
}