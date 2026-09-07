#include "Master.h"

#include "DrawManager.h"
#include "EndManager.h"

#include "ImguiManager.h"
#include "KeyState.h"
#include "LoadingManager.h"
#include "ResourceManager.h"
#include "StopManager.h"
#include "ThreadManager.h"
#include "TimeManager.h"

// マスター所持物全削除処理
void Master::AllDelete()
{
	// 終了処理
	mpDrawManager->Finalize();
	mpImguiManager->Finalize();
	mpResourceManager->Finalize();
	mpThreadManager->Finalize();
	
	// 削除
	delete mpDrawManager;
	delete mpEndManager;
	delete mpImguiManager;
	delete mpKeyState;
	delete mpLoadingManager;
	delete mpResourceManager;
	delete mpStopManager;
	delete mpThreadManager;
	delete mpTimeManager;
}