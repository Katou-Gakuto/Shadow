#pragma once

#include "BaseCollision2D.h"

class Capsule2D : public BaseCollision2D
{
private:
    VECTOR2D mvCenterLineStartPos;
    VECTOR2D mvCenterLineEndPos;
    float mfCenterLineRadiusSqrt;

public:
    Capsule2D();
    Capsule2D(
        const VECTOR2D &startPos, const VECTOR2D &endPos, float radius,
        GameObject *myObject, CollisionTag tag, CollisionNorm normNum,
        bool penetrate, bool hitMove, float moveLate);
    ~Capsule2D() override;

    int Draw(const Material2D &color) override;
    int SetPosToMoveVec() override;
    int SlideMove(const CollisionCheckResult2D &result, float moveLate) override;
    int SetBaseParamMySelf() override;
    int GetAABB(AABB2D &out) override;

    void SetShapeParameter(const VECTOR2D &startPos, const VECTOR2D &endPos, float radiusNoSqrt);

    VECTOR2D GetStartPos() const { return this->mvCenterLineStartPos; }
    VECTOR2D GetEndPos() const { return this->mvCenterLineEndPos; }
    VECTOR2D GetLineVec() const { return this->mvCenterLineEndPos - this->GetBasePos(); }
};