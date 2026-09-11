#pragma once

#include "BaseCollision2D.h"

class Quadrangle2D : public BaseCollision2D
{
private:
    VECTOR2D mvVertexPos[4];

public:
    Quadrangle2D();
    Quadrangle2D(
        const VECTOR2D &pos0, const VECTOR2D &pos1, const VECTOR2D &pos2, const VECTOR2D &pos3,
        GameObject *myObject, CollisionTag tag, CollisionNorm normNum,
        bool penetrate, bool hitMove, float moveLate);
    ~Quadrangle2D() override;

    int Draw(const Material2D &color) override;
    int SetPosToMoveVec();
    int SlideMove(const CollisionCheckResult2D &result, float moveLate) override;
    int SetBaseParamMySelf() override;
    int GetAABB(AABB2D &out) override;

    void SetShapeParameter(const VECTOR2D &pos0, const VECTOR2D &pos1, const VECTOR2D &pos2, const VECTOR2D &pos3);

    VECTOR2D GetVertexPos(int index);
};