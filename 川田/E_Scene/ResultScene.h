#pragma once

#include "BaseScene.h"

enum ResultScenePhase : unsigned char
{
    ResultScenePhase_Solo_Result,

    ResultScenePhase_Multi_Result,
    ResultScenePhase_Multi_Wait_Battle,
};

class ResultScene : public BaseScene
{
private:
    bool mbPlayerWin;
    ResultScenePhase mnPhase;


    int UpdateSolo();
    int UpdateMulti();

    int DrawSolo(unsigned long myNumber);
    int DrawMulti(unsigned long myNumber);






public:
    ResultScene();
    ~ResultScene() override;

    int Create() override;
    int Initialize() override;
    int Finalize() override;
    int Update() override;
    int Draw() override;

    void SetResult(bool playerWin, SceneTag prevScene);
};