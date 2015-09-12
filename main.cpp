#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <dinput.h>
#include "Sprite.h"
#include "Game.h"
#include "GameLog.h"
#include "MapReader\tmx.h.in"
#include "SceneManager.h"
#include "SceneGamePlay.h"

using namespace std;

#define WIN_NAME L"Rockman"
#define WIN_TITLE L"Rockman"
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 640
#define FPS 60
#define KEYBOARD_BUFFERD_SIZE 1024

/*

*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
    Scene *newScene = new SceneGamePlay();

    Game *game = new Game(hInstance, L"Owl's Destiny", cmdShow, newScene);

    return 0;
}
