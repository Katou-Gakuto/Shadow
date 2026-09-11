#pragma once

#include "BaseComponent.h"

class PowerSpot;
class PowerSpotController : public BaseComponent
{
private:
    PowerSpot *GetPowerSpot() const;
    unsigned long repopACK;

public:
    PowerSpotController(GameObject *myObject);
    ~PowerSpotController();

    int Create() override;
    int Initialize() override;
    int Finalize() override;
    int EarlyUpdate() override;
    int Update() override;
    int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision) override;
    int LateUpdate() override;
    int Draw() override;
};