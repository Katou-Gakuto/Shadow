#pragma once

#include "BaseCollision2D.h"

class Ray2D : public BaseCollision2D
{
private:
    VECTOR2D mvStartPos;
    VECTOR2D mvEndPos;

public:
    Ray2D();
    Ray2D(
        const VECTOR2D &startPos, const VECTOR2D &endPos,
        GameObject *myObject, CollisionTag tag,
        bool penetrate, bool hitMove, float moveLate);
    ~Ray2D() override;

    int Draw(const Material2D &color) override;
    int SetPosToMoveVec() override;
    int SlideMove(const CollisionCheckResult2D &result, float moveLate) override;
    int SetBaseParamMySelf() override;
    int GetAABB(AABB2D &out) override;

    void SetShapeParameter(const VECTOR2D &startPos, const VECTOR2D &endPos);

    VECTOR2D GetStartPos() const { return this->mvStartPos; }
    VECTOR2D GetEndPos() const { return this->mvEndPos; }
    VECTOR2D GetLineVec() const { return this->mvEndPos - this->mvStartPos; }
};