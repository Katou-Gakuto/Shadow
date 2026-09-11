#pragma once

#pragma once

#include "BaseComponent.h"

#include "../Y_Tool/CountDownSystem.h"
#include "../Y_Tool/VECTOR.h"

enum PlayerMoveMode : unsigned char;
class Sound3D;

class TestEmitterController : public BaseComponent
{
private:

public:
    TestEmitterController(GameObject *myObject);
    ~TestEmitterController();

    int Create() override;
    int Initialize() override;
    int Finalize() override;
    int EarlyUpdate() override;
    int Update() override;
    int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision) override;
    int LateUpdate() override;
    int Draw() override;

};