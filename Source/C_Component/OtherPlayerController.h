#pragma once

#include "BaseComponent.h"

#include "../Y_Tool/CountDownSystem.h"
#include "../Y_Tool/VECTOR.h"

enum PlayerMoveMode : unsigned char;
class Sound3D;

class OtherPlayerController : public BaseComponent
{
private:
    Sound3D *mpFootstepsSound;
    CountDownSystem mnFootstepsCount;
    unsigned long mnPersonNumber;
    VECTOR3D mvNextDir;
    PlayerMoveMode mnOldMove;
    bool mbFootStepsNow;
    bool mbPowerCharge;

    int UpdateFootsteps(PlayerMoveMode newMove);

public:
    OtherPlayerController(GameObject *myObject);
    ~OtherPlayerController();

    int Create() override;
    int Initialize() override;
    int Finalize() override;
    int EarlyUpdate() override;
    int Update() override;
    int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision) override;
    int LateUpdate() override;
    int Draw() override;

};