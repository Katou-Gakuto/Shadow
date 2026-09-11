#pragma once

#include "BaseCollision3D.h"

class Triangle3D;

class Quadrangle3D : public BaseCollision3D
{
private:
    VECTOR3D mvVertexPos[4];

public:
    Quadrangle3D();
    Quadrangle3D(
        const VECTOR3D &pos0, const VECTOR3D &pos1, const VECTOR3D &pos2, const VECTOR3D &pos3,
        GameObject *myObject, CollisionTag tag, CollisionNorm normNum,
        bool penetrate, bool hitMove, float moveLate);
    ~Quadrangle3D() override;

    int GetTriangle3D(Triangle3D *dest, int index);

    int Draw(const Material2D &color) override;
    int SetPosToMoveVec();
    int SlideMove(const CollisionCheckResult3D &result, float moveLate) override;
    int SetBaseParamMySelf() override;
    int GetAABB(AABB3D &out) override;

    void SetShapeParameter(const VECTOR3D &pos0, const VECTOR3D &pos1, const VECTOR3D &pos2, const VECTOR3D &pos3);

    VECTOR3D GetVertexPos(int index);
    VECTOR3D GetNormVec();
};