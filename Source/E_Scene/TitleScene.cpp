#include "TitleScene.h"

#include "../H_Katou/Singleton/KeyState.h"

#include "../Z_Except/Master.h"

// 
TitleScene::TitleScene() :
    mbLogoPhase(true),
    mnNowMode(TitleSelectMode::TitleSelectMode_ModeSelect),
    mnCurrentMode(TitleSelectMode::TitleSelectMode_StageSelect),
    BaseScene(SceneTag::ST_Title)
{
}

// 
TitleScene::~TitleScene()
{
}

// 
int TitleScene::Create()
{
}

// 
int TitleScene::Initialize()
{
    // 
    this->mbLogoPhase = true;
    this->mnNowMode = TitleSelectMode::TitleSelectMode_ModeSelect;
    this->mnCurrentMode = TitleSelectMode::TitleSelectMode_StageSelect;

    // 
    return 0;
}

// 
int TitleScene::Finalize()
{
}

// 
int TitleScene::Update()
{
    // 
    Master::mpKeyState;
}

// 
int TitleScene::Draw()
{
}
