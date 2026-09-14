#pragma once

#include "WallObjectController.h"

// 
class MirrorObjectController : public WallObjectController
{
public:
    MirrorObjectController(GameObject *myObject, const VECTOR2D &leftUp, const VECTOR2D &rightDown, unsigned char mirrorFace);
    ~MirrorObjectController() override;

    int Create() override;
    int Initialize() override;
    int Finalize() override;
    int EarlyUpdate() override;
    int Update() override;
    int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision) override;
    int LateUpdate() override;
    int Draw() override;

private:
    unsigned char mnMirrorFace;
};