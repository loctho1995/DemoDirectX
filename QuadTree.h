#ifndef __QUAD_TREE__
#define __QUAD_TREE__
#include "GameLog.h"
#include <d3dx9.h>
#include <d3d9.h>
#include "GameCollision.h"
#include <vector>
#include "Camera.h"
#include "GameGlobal.h"

#define QUADTREE_MAX_LEVEL 4 //so node con toi da


class Entity
{
public:
    enum EntityTypes
    {
        Brick, Enemy, Mario, Static, BrickGoldNormal, BrickGoldEated
    };

    Entity();
    RECT Bound;
    //vi tri tam
    D3DXVECTOR3 GetEntityPosition();
    int Tag; //Tag de nhan vien loai Entity
    RECT GetBoundOnScreen(Camera *camera);
    //tra ve ket qua cua viec xu ly va cham co bien bool la IsCollide = true neu xay ra va cham va nguoc lai
    GameCollision::CollisionReturn CollisionResult; 
    

protected:
};

class QuadTree
{
public:
    QuadTree();
    QuadTree(int level, RECT bound);
    ~QuadTree();
    void Clear();
    void insertEntity(Entity *entity);
    QuadTree **Nodes;
    std::vector<Entity*> ListEntity; //danh sach cac phan tu co trong vung va cham (Bound)
    
    //lay danh sach nhung Entity co kha nang xay ra va cham
    std::vector<Entity*> getEntitiesCollideAble(std::vector<Entity*> &entitiesOut,Entity *entity); // tra ve danh sach cac phan tu nam trong vung va cham
    int getTotalEntities();
    static int TotalCallBackCall;

    RECT Bound;

protected:
    void split(); //thuc hien chia ra cac node
    bool isContain(Entity *entity);
    int mLevel; //tuong ung voi so node
};

#endif