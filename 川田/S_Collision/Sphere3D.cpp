#include "Sphere3D.h"

Sphere3D::Sphere3D() :
    BaseCollision3D(nullptr, CollisionType::CollisionType_Sphere3D, CollisionTag_Checker, CollisionNorm_Out, true, false, 0.0f)
{

}

Sphere3D::Sphere3D(
    const VECTOR3D &pos, float radiusSqrt,
    GameObject *myObject, CollisionTag tag, CollisionNorm normNum,
    bool penetrate, bool hitMove, float moveLate) :
    BaseCollision3D(myObject, CollisionType::CollisionType_Sphere3D, tag, normNum, penetrate, hitMove, moveLate)
{
    this->SetBasePos(pos);
    this->SetBaseRadiusSqrt(radiusSqrt);
}

Sphere3D::~Sphere3D()
{

}

int Sphere3D::Draw(const Material2D &color)
{
    return 0;
}

int Sphere3D::SetPosToMoveVec()
{
    VECTOR3D moveVec = this->GetMoveVec();
    if (moveVec != VECTOR3D::GetZero())
    {
        this->SetBasePos(this->GetBasePos() + moveVec);

        this->SetMoveVec(VECTOR3D::GetZero());
    }

    return 0;
}

int Sphere3D::SlideMove(const CollisionCheckResult3D &result, float moveLate)
{
    return 0;
}

int Sphere3D::SetBaseParamMySelf()
{
    return 0;
}

int Sphere3D::GetAABB(AABB3D &out)
{
    float radiusSqrt = this->GetBaseRadius();
    out.mvMinPos.SetXYZ(
        this->GetBasePos().GetX() - radiusSqrt,
        this->GetBasePos().GetY() - radiusSqrt,
        this->GetBasePos().GetZ() - radiusSqrt);
    out.mvMaxPos.SetXYZ(
        this->GetBasePos().GetX() + radiusSqrt,
        this->GetBasePos().GetY() + radiusSqrt,
        this->GetBasePos().GetZ() + radiusSqrt);
    out.mvMoveVec = this->GetMoveVec();
    return 0;
}

void Sphere3D::SetShapeParameter(const VECTOR3D &pos, float radiusNoSqrt)
{
    this->SetBasePos(pos);
    this->SetBaseRadiusNoSqrt(radiusNoSqrt);
}