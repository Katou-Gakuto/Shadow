#pragma once

#include "BaseCollision2D.h"

class Point2D : public BaseCollision2D
{
private:

public:
    Point2D();
    Point2D(const VECTOR2D &pos, GameObject *myObject, CollisionTag tag, bool penetrate, bool hitMove, float moveLate);
    ~Point2D() override;

    int Draw(const Material2D &color) override;
    int SetPosToMoveVec() override;
    int SlideMove(const CollisionCheckResult2D &result, float moveLate) override;
    int SetBaseParamMySelf() override;
    int GetAABB(AABB2D &out) override;

    void SetShapeParameter(const VECTOR2D &pos);
};