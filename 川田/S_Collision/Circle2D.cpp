#include "Circle2D.h"

Circle2D::Circle2D() :
    BaseCollision2D(nullptr, CollisionType::CollisionType_Circle2D, CollisionTag_Checker, CollisionNorm::CollisionNorm_Out, true, false, 0.0f)
{

}

Circle2D::Circle2D(
    const VECTOR2D &pos, float radiusSqrt, GameObject *myObject, CollisionTag tag, CollisionNorm normNum,
    bool penetrate, bool hitMove, float moveLate) :
    BaseCollision2D(myObject, CollisionType::CollisionType_Circle2D, tag, normNum, penetrate, hitMove, moveLate)
{
    this->SetBasePos(pos);
    this->SetBaseRadiusSqrt(radiusSqrt);
}

Circle2D::~Circle2D()
{

}

int Circle2D::Draw(const Material2D &color)
{
    return 0;
}

int Circle2D::SetPosToMoveVec()
{
    VECTOR2D moveVec = this->GetMoveVec();
    if (moveVec!= VECTOR2D::GetZero())
    {
        this->SetBasePos(this->GetBasePos() + moveVec);

        this->SetMoveVec(VECTOR2D::GetZero());
    }

    return 0;
}

int Circle2D::SlideMove(const CollisionCheckResult2D &result, float moveLate)
{
    return 0;
}

int Circle2D::SetBaseParamMySelf()
{
    return 0;
}

int Circle2D::GetAABB(AABB2D &out)
{
    float radiusSqrt = this->GetBaseRadius();
    out.mvMinPos.SetXY(
        this->GetBasePos().GetX() - radiusSqrt,
        this->GetBasePos().GetY() - radiusSqrt);
    out.mvMaxPos.SetXY(
        this->GetBasePos().GetX() + radiusSqrt,
        this->GetBasePos().GetY() + radiusSqrt);
    out.mvMoveVec = this->GetMoveVec();
    return 0;
}

void Circle2D::SetShapeParameter(const VECTOR2D &pos, float radiusNoSqrt)
{
    this->SetBasePos(pos);
    this->SetBaseRadiusNoSqrt(radiusNoSqrt);
}