#include "QuadTree.h"

int QuadTree::TotalCallBackCall = 0;

Entity::Entity()
{
    Tag = 0;
}

D3DXVECTOR3 Entity::GetEntityPosition()
{
    return D3DXVECTOR3(Bound.left, Bound.top , 0);
}

RECT Entity::GetBoundOnScreen(Camera *camera)
{
    D3DXVECTOR2 trans = D3DXVECTOR2(GameGlobal::GetWidth() / 2 - camera->GetPosition().x,
                                    GameGlobal::GetHeight() / 2 - camera->GetPosition().y);

    RECT bound;
    bound.left = Bound.left + trans.x;
    bound.right = bound.left + (Bound.right - Bound.left);
    bound.top = Bound.top + trans.y;
    bound.bottom = bound.top + (Bound.bottom - Bound.top);

    return bound;
}

QuadTree::QuadTree()
{
}

QuadTree::~QuadTree()
{
}

QuadTree::QuadTree(int level, RECT bound)
{
    //chia lam 4

    this->Bound = bound;
    this->mLevel = level;

    if (mLevel < QUADTREE_MAX_LEVEL)
        split();
}

void QuadTree::Clear()
{
    if (Nodes)
    {
        for (size_t i = 0; i < 4; i++)
        {
            if (Nodes[i])
            {
                Nodes[i]->Clear();
                delete Nodes[i];
                Nodes[i] = nullptr;
            }
        }

        delete[] Nodes;
    }
}

void QuadTree::insertEntity(Entity *entity)
{
    if (Nodes)
    {
        for (size_t i = 0; i < 4; i++)
        {
            if (Nodes[i])
            {
                if (Nodes[i]->isContain(entity))
                    Nodes[i]->insertEntity(entity);
            }
        }
    }

    if (this->isContain(entity))
        ListEntity.push_back(entity);

    TotalCallBackCall++;

    //GAMELOG("Level: %d", mLevel);

    //khoi tao cac node con
    //if (mLevel < QUADTREE_MAX_LEVEL && Nodes == NULL)
    //{
    //    while (!ListEntity.empty())
    //    {
    //        for (size_t i = 0; i < 4; i++)
    //        {
    //            if (Nodes[i]->isContain(ListEntity.back()))
    //                Nodes[i]->insertEntity(ListEntity.back());
    //        }

    //        ListEntity.pop_back();
    //    }
    //}
}

bool QuadTree::isContain(Entity *entity)
{
    if (GameCollision::RectangleAndRectangle(entity->Bound, this->Bound).IsCollided)
        return true;

    return false;
}

void QuadTree::split()
{
    //cat phan region (ranh gioi) ra thanh 4 phan bang nhau

    Nodes = new QuadTree*[4];
    RECT bound;

    int width = (Bound.right - Bound.left) / 2;
    int height = (Bound.bottom - Bound.top) / 2;

    //phan goc trai tren
    bound.left = Bound.left;
    bound.right = bound.left + width;
    bound.top = Bound.top;
    bound.bottom = bound.top + height;
    Nodes[0] = new QuadTree(mLevel + 1, bound);

    //phan goc phai tren
    bound.left = Bound.left + width;
    bound.right = bound.left + width;
    bound.top = Bound.top;
    bound.bottom = bound.top + height;
    Nodes[1] = new QuadTree(mLevel + 1, bound);

    //phan goc trai duoi
    bound.left = Bound.left;
    bound.right = bound.left + width;
    bound.top = Bound.top + height;
    bound.bottom = bound.top + height;
    Nodes[2] = new QuadTree(mLevel + 1, bound);

    //phan goc phai duoi
    bound.left = Bound.left + width;
    bound.right = bound.left + width;
    bound.top = Bound.top + height;
    bound.bottom = bound.top + height;
    Nodes[3] = new QuadTree(mLevel + 1, bound);
}

int QuadTree::getTotalEntities()
{
    int total = ListEntity.size();

    if (Nodes)
    {
        for (QuadTree *node = 0; node != nullptr; node++)
        {
            total += node->getTotalEntities();
        }
    }

    return total;
}

std::vector<Entity*> QuadTree::getEntitiesCollideAble(std::vector<Entity*> &entitiesOut, Entity *entity)
{
    if(Nodes)
    {
        for (int i = 0; i < 4; i ++)
        {
            if (Nodes[i]->isContain(entity))
                Nodes[i]->getEntitiesCollideAble(entitiesOut, entity);
        }
    }
    else if (this->isContain(entity))
    {
        for (std::vector<Entity*>::iterator i = this->ListEntity.begin(); i != this->ListEntity.end(); i++)
        {
            if (entity != *i)
                entitiesOut.push_back(*i);
        }
    }

    return entitiesOut;
}