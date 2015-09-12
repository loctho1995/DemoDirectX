#include "GameCollision.h"


GameCollision::GameCollision()
{

}


GameCollision::~GameCollision()
{
}

GameCollision::CollisionReturn GameCollision::RectangleAndRectangle(RECT rect1, RECT rect2)
{
    GameCollision::CollisionReturn result;

    if (rect1.left > rect2.right || rect1.right < rect2.left || rect1.top > rect2.bottom || rect1.bottom < rect2.top)
        result.IsCollided = false;

    //chon max Left
    result.RegionCollision.left = rect1.left > rect2.left ? rect1.left : rect2.left;
    //chon max right
    result.RegionCollision.right = rect1.right < rect2.right ? rect1.right : rect2.right;
    //chon min bottom
    result.RegionCollision.bottom = rect1.bottom < rect2.bottom ? rect1.bottom : rect2.bottom;
    //chon max top
    result.RegionCollision.top = rect1.top > rect2.top ? rect1.top : rect2.top;

    return result;
}

bool GameCollision::PointAndRectangle(float x, float y, RECT rect)
{
    if (x < rect.left || x > rect.right || y < rect.top || y > rect.bottom)
        return false;

    return true;
}

bool GameCollision::RectangleAndCircle(RECT rect, int circlex, int circley, int circleRadius)
{
    int px = circlex;
    int py = circley;

    if (px < rect.left)
        px = rect.left;
    else if (px > rect.right)
        px = rect.right;

    if (py > rect.bottom)
        py = rect.bottom;
    else if (py < rect.top)
        py = rect.top;

    int dx = px - circlex;
    int dy = py - circley;

    return (dx * dx + dy * dy) <= circleRadius * circleRadius;
}