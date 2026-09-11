#pragma once

#include "BaseCollision3D.h"

class Capsule3D : public BaseCollision3D
{
private:
    VECTOR3D mvCenterLineStartPos;
    VECTOR3D mvCenterLineEndPos;
    float mfCenterLineRadiusSqrt;

public:
    Capsule3D();
    Capsule3D(
        const VECTOR3D &startPos, const VECTOR3D &endPos, float radius,
        GameObject *myObject, CollisionTag tag, CollisionNorm normNum,
        bool penetrate, bool hitMove, float moveLate);
    ~Capsule3D() override;

    int Draw(const Material2D &color) override;
    int SetPosToMoveVec() override;
    int SlideMove(const CollisionCheckResult3D &result, float moveLate) override;
    int SetBaseParamMySelf() override;
    int GetAABB(AABB3D &out) override;

    void SetShapeParameter(const VECTOR3D &startPos, const VECTOR3D &endPos, float radiusNoSqrt);

    float GetCenterRadiusSqrt() { return this->mfCenterLineRadiusSqrt; }
    float GetCenterRadiusNoSqrt() { return this->mfCenterLineRadiusSqrt * this->mfCenterLineRadiusSqrt; }
    VECTOR3D GetStartPos() { return this->mvCenterLineStartPos; }
    VECTOR3D GetEndPos() { return this->mvCenterLineEndPos; }
    VECTOR3D GetLineVec() { return this->mvCenterLineEndPos - this->GetBasePos(); }
};