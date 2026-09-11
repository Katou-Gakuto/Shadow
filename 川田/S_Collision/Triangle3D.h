#pragma once

#include "BaseCollision3D.h"

class Triangle3D : public BaseCollision3D
{
private:
    VECTOR3D mvVertexPos[3];

public:
    Triangle3D();
    Triangle3D(
        const VECTOR3D &pos0, const VECTOR3D &pos1, const VECTOR3D &pos2,
        GameObject *myObject, CollisionTag tag, CollisionNorm normNum,
        bool penetrate, bool hitMove, float moveLate);
    ~Triangle3D() override;

    int Draw(const Material2D &color) override;
    int SetPosToMoveVec();
    int SlideMove(const CollisionCheckResult3D &result, float moveLate) override;
    int SetBaseParamMySelf() override;
    int GetAABB(AABB3D &out) override;

    void SetShapeParameter(const VECTOR3D &pos0, const VECTOR3D &pos1, const VECTOR3D &pos2);

    VECTOR3D GetVertexPos(int index);
    VECTOR3D GetNormVec();                  // この三角形の法線ベクトルを返す関数
};