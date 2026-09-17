#pragma once

#include "BaseComponent.h"

#include "../Y_Tool/VECTOR.h"

// 
class WallObjectController : public BaseComponent
{
public:
    WallObjectController(GameObject *myObject, const VECTOR2D &leftUp, const VECTOR2D &rightDown);
    virtual ~WallObjectController() override;

    virtual int Create() override;
    virtual int Initialize() override;
    virtual int Finalize() override;
    virtual int EarlyUpdate() override;
    virtual int Update() override;
    virtual int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision) override;
    virtual int LateUpdate() override;
    virtual int Draw() override;

protected:
    WallObjectController(GameObject *myObject, ComponentTagAndOrder ctao, const VECTOR2D &leftUp, const VECTOR2D &rightDown);
    VECTOR2D mvLeftUpInit;
    VECTOR2D mvRightDownInit;
    VECTOR2D mvLeftUp;
    VECTOR2D mvRightDown;
    CollisionHandle mdBody;
};
