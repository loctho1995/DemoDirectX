#include "PhysicWorld.h"

#pragma region -PHYSIC WORLD-
PhysicWorld::PhysicWorld(D3DXVECTOR2 gravity)
{
    mGravity = gravity;
}


PhysicWorld::~PhysicWorld()
{
}

void PhysicWorld::Update(float dt)
{

}

void PhysicWorld::SetGravity(D3DXVECTOR2 gravity)
{
    mGravity = gravity;
}

D3DXVECTOR2 PhysicWorld::GetGravity()
{
    return mGravity;
}

void PhysicWorld::OnContactBegin()
{

}
void PhysicWorld::AddPhysicBody(PhysicBody *body)
{

}

std::vector<PhysicBody*> PhysicWorld::GetListBody()
{
    return mListBody;
}

#pragma endregion

#pragma region -PHYSIC BODY-
PhysicBody::PhysicBody()
{
}

PhysicBody::~PhysicBody()
{

}

void PhysicBody::SetShape(Shape shape)
{

}

void PhysicBody::SetObject(void* object)
{
    mObject = object;
}
#pragma endregion

#pragma region SHAPE
Shape::Shape()
{

}

Shape::~Shape()
{

}

Rectangle::Rectangle()
{
    Top = Right = Bottom = Left = 0;
}

Shape::Type Rectangle::GetType()
{
    return Shape::Type::Rectangle;
}

Circle::Circle()
{
    Radius = 0;
}

Shape::Type Circle::GetType()
{
    return Shape::Type::Circle;
}
#pragma endregion