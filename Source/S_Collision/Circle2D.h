#pragma once

#include "BaseCollision2D.h"

class Circle2D : public BaseCollision2D
{
private:

public:
    Circle2D();
    Circle2D(
        const VECTOR2D &pos, float radiusSqrt, GameObject *myObject, CollisionTag tag, CollisionNorm normNum,
        bool penetrate, bool hitMove, float moveLate);
    ~Circle2D() override;

    int Draw(const Material2D &color) override;
    int SetPosToMoveVec() override;
    int SlideMove(const CollisionCheckResult2D &result, float moveLate) override;
    int SetBaseParamMySelf() override;
    int GetAABB(AABB2D &out) override;

    void SetShapeParameter(const VECTOR2D &pos, float radiusNoSqrt);
};