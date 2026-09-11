#pragma once

#include "BaseCollision3D.h"

class Ray3D : public BaseCollision3D
{
private:
    VECTOR3D mvStartPos;
    VECTOR3D mvEndPos;

public:
    Ray3D();
    Ray3D(
        const VECTOR3D &startPos, const VECTOR3D &endPos,
        GameObject *myObject, CollisionTag tag,
        bool penetrate, bool hitMove, float moveLate);
    ~Ray3D() override;

    int Draw(const Material2D &color) override;
    int SetPosToMoveVec() override;
    int SlideMove(const CollisionCheckResult3D &result, float moveLate) override;
    int SetBaseParamMySelf() override;
    int GetAABB(AABB3D &out) override;

    void SetShapeParameter(const VECTOR3D &startPos, const VECTOR3D &endPos);

    VECTOR3D GetStartPos() const { return this->mvStartPos; }
    VECTOR3D GetEndPos() const { return this->mvEndPos; }
    VECTOR3D GetLineVec() const { return this->mvEndPos - this->mvStartPos; }
};