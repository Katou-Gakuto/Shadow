#pragma once

#include "BaseScene.h"

class StageSelectScene : public BaseScene
{
public:
    // 
    StageSelectScene();

    // 
    ~StageSelectScene();

    // 
    int Create() override;

    // 
    int Initialize() override;

    // 
    int Finalize() override;

    // 
    int Update() override;

    // 
    int Draw() override;

private:
    // 
    unsigned long mnCurrentStage;

    // 
    unsigned long  mnUnlockStageCount;
};