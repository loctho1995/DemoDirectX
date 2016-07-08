#include "TransitionScanDown.h"

TransitionScanDown::TransitionScanDown(float speed)
{
    scanSpeed = speed;
}

TransitionScanDown::~TransitionScanDown()
{
}

void TransitionScanDown::update()
{
    currentPosScan += scanSpeed;
}

bool TransitionScanDown::isFinish()
{
    return currentPosScan >= 1.0;
}