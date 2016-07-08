#ifndef __PHYSIC_WORLD__
#define __PHYSIC_WORLD__

#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "GameLog.h"
#include "QuadTree.h"

//quy dinh hinh dang cho cac body trong physicworld
#pragma region -SHAPE-
class Shape
{
public:
    enum Type
    {
        Rectangle, Circle
    };

    Shape();
    ~Shape();
    Type GetType();
protected:
};
#pragma endregion

//các doi tuong de set va cham trong physicworld ke thua tu lop Node trong quadtree 

#pragma region -PHYSIC BODY-
class PhysicBody : Entity
{
public:
    PhysicBody();
    ~PhysicBody();
    void SetShape(Shape shape);
    void SetObject(void* object);
protected:
    bool        mIsOneWay; //true: cho phep di xuyen qua 1 lan tu duoi len nhung khong cho qua tu tren xuong, false: khong                       cho xuyen qua bat ki huong nao
    Shape       mShape;
    void*       mObject;
};
#pragma endregion

//ho tro hinh chu nhat va hinh tron
#pragma region -RECTANGGLE AND CIRCLE SHAPE-
class Rectangle : public Shape
{
public:
    Rectangle();
    Type GetType();
    int Top, Left, Right, Bottom;
};

class Circle : public Shape
{
public:
    Circle();
    Type GetType();
    int Radius;
};
#pragma endregion

//physic world tao 1 the gioi chi co xu ly physic va cham
//su dung quadtree de toi uu hoa va cham
#pragma region -PHYSIC WORLD-
class PhysicWorld
{
public:
    PhysicWorld(D3DXVECTOR2 gravity = D3DXVECTOR2(0, 10));
    ~PhysicWorld();
    void Update(float dt);
    void OnContactBegin();
    void AddPhysicBody(PhysicBody *body);
    void SetGravity(D3DXVECTOR2 gravity);
    D3DXVECTOR2 GetGravity();
    std::vector<PhysicBody*> GetListBody();

protected:
    std::vector<PhysicBody*>    mListBody;
    D3DXVECTOR2                 mGravity;

};
#pragma endregion

#endif