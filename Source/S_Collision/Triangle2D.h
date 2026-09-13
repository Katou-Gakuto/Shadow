#pragma once

#include "BaseCollision2D.h"

class Triangle2D : public BaseCollision2D
{
private:
    VECTOR2D mvVertexPos[3];

public:
    Triangle2D();
    Triangle2D(
        const VECTOR2D &pos0, const VECTOR2D &pos1, const VECTOR2D &pos2,
        GameObject *myObject, CollisionTag tag, CollisionNorm normNum,
        bool penetrate, bool hitMove, float moveLate);
    ~Triangle2D() override;

    int Draw(const Material2D &color) override;
    int SetPosToMoveVec();
    int SlideMove(const CollisionCheckResult2D &result, float moveLate) override;
    int SetBaseParamMySelf() override;
    int GetAABB(AABB2D &out) override;

    void SetShapeParameter(const VECTOR2D &pos0, const VECTOR2D &pos1, const VECTOR2D &pos2);

    VECTOR2D GetVertexPos(int index);
};