#include "Master.h"

#include "../E_Scene/BaseSceneManager.h"

#include "KeyState.h"
#include "EndManager.h"
#include "TimeManager.h"
#include "VECTOR.h"

#include "../C_Component/Player.h"
#include "../G_LightArea/LightAreaManager.h"

// 
BaseSceneManager *Master::mpBaseSceneManager = nullptr;

EndManager* Master::mpEndManager = nullptr;
TimeManager* Master::mpTimeManager = nullptr;
KeyState* Master::mpKeyState = nullptr;

Player* Master::mpPlayerLight = nullptr;
Player* Master::mpPlayerShadow = nullptr;
LightAreaManager* Master::mpLightManager = nullptr;


// Masterの各メンバをnewする関数
int Master::Initialize()
{

    mpEndManager = new EndManager();
    mpTimeManager = new TimeManager(/*/1/*/17/**/);
    mpTimeManager->Initilize();
    mpKeyState = new KeyState();
    // 

    mpBaseSceneManager = new BaseSceneManager();
    mpBaseSceneManager->Initialize();

    mpLightManager = new LightAreaManager();


    return 0;
}

// Masterの各メンバをdeleteする関数
int Master::Finalize()
{
    mpBaseSceneManager->Finalize();

    // 
    delete mpBaseSceneManager;
    delete mpEndManager;
    delete mpTimeManager;
    delete mpKeyState;

    return 0;
}

// 画像を拡大/縮小し、UV座標を指定して描画する関数
int Master::DrawGraphAnim(
	const VECTOR2D &posLeftUp, const VECTOR2D &posRightDown,
	const VECTOR2D &uvLeftUp, const VECTOR2D &uvRightDown,
	int graphHandle)
{
	// 
    DrawRectExtendGraphF(
        posLeftUp.GetX(), posLeftUp.GetY(), posRightDown.GetX(), posRightDown.GetY(),
        uvLeftUp.GetX(), uvLeftUp.GetY(), uvRightDown.GetX(), uvRightDown.GetY(), graphHandle, false);

	return 0;
}
