#pragma once

#include "BaseCollision3D.h"

class Point3D : public BaseCollision3D
{
private:

public:
    Point3D();
    Point3D(const VECTOR3D &pos, GameObject *myObject, CollisionTag tag, bool penetrate, bool hitMove, float moveLate);
    ~Point3D() override;

    int Draw(const Material2D &color) override;
    int SetPosToMoveVec() override;
    int SlideMove(const CollisionCheckResult3D &result, float moveLate) override;
    int SetBaseParamMySelf() override;
    int GetAABB(AABB3D &out) override;

    void SetShapeParameter(const VECTOR3D &pos);
};