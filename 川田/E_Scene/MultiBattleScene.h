#pragma once

#include "BaseScene.h"

#include "../T_Model/Model2D.h"
#include "../Y_Tool/MyTimer.h"

class GameObject;
class Player;
class OtherPlayer;
class MultiBattleScene : public BaseScene
{
private:
    Player *mpPlayer;
    OtherPlayer *mpOtherPlayer;
    GameObject *mpPowerSpot;

    Model2D *mpIconVisual;
    Model2D *mpIconSound;
    Model2D *mpIconAttack;
    Model2D *mpIconSkill;

    TimeParam mdStartTime;
    MyAlarm mdAlarm;

    bool mbFirstOver;

public:
    MultiBattleScene();
    ~MultiBattleScene();

    int SetLoseObject(GameObject *loseObject);

    int Create() override;
    int Initialize() override;
    int Finalize() override;
    int Update() override;
    int Draw() override;
};