#include "GameMap.h"


GameMap::GameMap(LPD3DXSPRITE spriteHandler,char* filePath)
{
    mCamera = new Camera(GameGlobal::GetWidth(), GameGlobal::GetHeight());
    mSpriteHandler = spriteHandler;
    LoadMap(filePath);
}

GameMap::~GameMap()
{
}

void GameMap::LoadMap(char* filePath)
{
    mMap = new Tmx::Map();
    mMap->ParseFile(filePath);

    for (size_t i = 0; i < mMap->GetNumTilesets(); i++)
    {
        const Tmx::Tileset *tileset = mMap->GetTileset(i);
        
        //wchar_t *unicodePath = GameConverter::StringToWSTR(tileset->GetImage()->GetSource());

        Sprite *sprite = new Sprite(tileset->GetImage()->GetSource().c_str());
        mListTileset.insert(pair<int, Sprite*>(i, sprite));
    }
}

void GameMap::SetCamera(Camera* camera)
{
    mCamera = camera;
}

Tmx::Map* GameMap::GetMap()
{
    return mMap;
}

RECT GameMap::GetWorldMapBound()
{
    RECT bound;
    bound.left = bound.top = 0;
    bound.right = mMap->GetWidth() * mMap->GetTileWidth();
    bound.bottom = mMap->GetHeight() * mMap->GetTileHeight();

    return bound;
}

int GameMap::GetWidth()
{
    return mMap->GetWidth() * mMap->GetTileWidth();
}

int GameMap::GetHeight()
{
    return mMap->GetHeight() * mMap->GetTileHeight();
}

int GameMap::GetTileWidth()
{
    return mMap->GetTileWidth();
}

int GameMap::GetTileHeight()
{
    return mMap->GetTileHeight();
}

bool GameMap::IsBoundLeft()
{
    return (mCamera->GetBound().left == 0);
}

bool GameMap::IsBoundRight()
{
    return (mCamera->GetBound().right == this->GetWidth());
}

bool GameMap::IsBoundTop()
{
    return (mCamera->GetBound().top == 0);
}

bool GameMap::IsBoundBottom()
{
    return (mCamera->GetBound().bottom == this->GetHeight());
}

void GameMap::Update(float dt)
{

}

void GameMap::Draw()
{
    for (size_t i = 0; i < mMap->GetNumTileLayers(); i++)
    {
        const Tmx::TileLayer *layer = mMap->GetTileLayer(i);

        if (!layer->IsVisible())
            continue;
        
        for (size_t j = 0; j < mMap->GetNumTilesets(); j++)
        {
            const Tmx::Tileset *tileSet = mMap->GetTileset(j);

            RECT sourceRECT;

            int tileWidth = mMap->GetTileWidth();
            int tileHeight = mMap->GetTileHeight();

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

                        sourceRECT.top = y * tileHeight;
                        sourceRECT.bottom = sourceRECT.top + tileHeight;
                        sourceRECT.left = x * tileWidth;
                        sourceRECT.right = sourceRECT.left + tileWidth;

                        Sprite* sprite = mListTileset[j];

                        //tru tilewidth/2 va tileheight/2 vi Sprite ve o vi tri giua hinh anh cho nen doi hinh de cho
                        //dung toa do (0,0) cua the gioi thuc la (0,0) neu khong thi se la (-tilewidth/2, -tileheigth/2);
                        D3DXVECTOR3 position(n * tileWidth + tileWidth / 2, m * tileHeight + tileHeight / 2, 0);

                        if (mCamera != NULL)
                        {
                            RECT objRECT;
                            objRECT.left = position.x - tileWidth / 2;
                            objRECT.top = position.y - tileHeight / 2;
                            objRECT.right = objRECT.left + tileWidth;
                            objRECT.bottom = objRECT.top + tileHeight;

                            if (!GameCollision::RectangleAndRectangle(mCamera->GetBound(), objRECT).IsCollided)
                                continue;
                        }

                        sprite->SetWidth(tileWidth);
                        sprite->SetHeight(tileHeight);

                        sprite->Draw(position, &sourceRECT, D3DXVECTOR2(),
                                        D3DXVECTOR2(GameGlobal::GetWidth() / 2 - mCamera->GetPosition().x,
                                                    GameGlobal::GetHeight() / 2 - mCamera->GetPosition().y));
                    }
                }
            }
        }
    }
}

std::map<int, Sprite*> GameMap::getListTileSet()
{
    return mListTileset;
}