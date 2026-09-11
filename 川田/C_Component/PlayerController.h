#pragma once

#include "BaseComponent.h"

#include "../Y_Tool/CountDownSystem.h"

class FPSCamera;
class Sound3D;

enum PlayerMoveMode : unsigned char
{
    PMM_NotMove,
    PMM_Tiptoe,
    PMM_Walk,
    PMM_Dash,
};

enum PlayerFeelMode : unsigned char
{
    PFM_Visual = 0,
    PFM_Sound,
    PFM_All,
};

class PlayerController : public BaseComponent
{
private:
    Sound3D *mpFootstepsSound;
    FPSCamera *mpCamera;
    CountDownSystem mnFootstepsCount;
    CountDownSystem mnAttackCount;
    CountDownSystem mnSleepCount;
    CountDownSystem mnModeChangeCount;
    CountDownSystem mnSkillCount;

    PlayerMoveMode mnMoveOld;
    bool mbFootStepsNow;
    bool mbPowerSpotFirst;
    
    static PlayerFeelMode mbVisualModeSave;
    static PlayerFeelMode mbVisualModeNow;

    int UpdateFootsteps(PlayerMoveMode newMove);
    int UpdateModeChangeCount(bool changeResult);
    int UpdateSleep(bool move);
    int UpdateSkill(bool change);
    bool CheckUpdateAttack();

public:
    PlayerController(GameObject *myObject);
    ~PlayerController();

    int Create() override;
    int Initialize() override;
    int Finalize() override;
    int EarlyUpdate() override;
    int Update() override;
    int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision) override;
    int LateUpdate() override;
    int Draw() override;

    CountDownSystem &GetFootstepsCount() { return this->mnFootstepsCount; }
    CountDownSystem &GetAttackCount() { return this->mnAttackCount; }
    CountDownSystem &GetSleepCount() { return this->mnSleepCount; }
    CountDownSystem &GetModeChangeCount() { return this->mnModeChangeCount; }
    CountDownSystem &GetSkillCount() { return this->mnSkillCount; }

    static PlayerFeelMode GetFeelModeNow();
    static PlayerFeelMode GetFeelModeSave();
};