#pragma once
__interface IInput
{
    virtual void OnKeyDown(int keyCode);
    virtual void OnKeyUp(int keyCode);
    //virtual void ProcessInput(BYTE keyStatee[]);
}; 