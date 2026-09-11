#pragma once

#include "BaseCollision3D.h"

class Sphere3D : public BaseCollision3D
{
private:

public:
    Sphere3D();
    Sphere3D(
        const VECTOR3D &pos, float radiusSqrt,
        GameObject *myObject, CollisionTag tag, CollisionNorm normNum,
        bool penetrate, bool hitMove, float moveLate);
    ~Sphere3D() override;

    int Draw(const Material2D &color) override;
    int SetPosToMoveVec() override;
    int SlideMove(const CollisionCheckResult3D &result, float moveLate) override;
    int SetBaseParamMySelf() override;
    int GetAABB(AABB3D &out) override;

    void SetShapeParameter(const VECTOR3D &pos, float radiusNoSqrt);
};