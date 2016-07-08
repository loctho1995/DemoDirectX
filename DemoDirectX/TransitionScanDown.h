#pragma once
class TransitionScanDown
{
public:
    //speed: toc do quet xuong don vi percent / frame
    //Width: 1.0f - Height: 1.0f 
    TransitionScanDown(float speed);
    ~TransitionScanDown();

    void update();

    bool isFinish();

private:
    float scanSpeed, currentPosScan;
};

