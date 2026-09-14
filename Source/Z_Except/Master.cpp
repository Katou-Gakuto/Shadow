#include "Master.h"

#include "../E_Scene/BaseSceneManager.h"

#include "KeyState.h"
#include "EndManager.h"
#include "TimeManager.h"

// 
BaseSceneManager *Master::mpBaseSceneManager = nullptr;

EndManager* Master::mpEndManager = nullptr;
TimeManager* Master::mpTimeManager = nullptr;
KeyState* Master::mpKeyState = nullptr;


// Masterの各メンバをnewする関数
int Master::Initialize()
{
    mpBaseSceneManager = new BaseSceneManager();

    mpEndManager = new EndManager();
    mpTimeManager = new TimeManager(17);
    mpTimeManager->Initilize();
    mpKeyState = new KeyState();
    // 
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
