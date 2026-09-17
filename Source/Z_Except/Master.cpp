#include "Master.h"

#include "../E_Scene/BaseSceneManager.h"

#include "KeyState.h"
#include "EndManager.h"
#include "TimeManager.h"

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
