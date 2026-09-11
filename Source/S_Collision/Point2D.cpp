#include "Point2D.h"

Point2D::Point2D() :
    BaseCollision2D(nullptr, CollisionType::CollisionType_Point2D, CollisionTag_Checker, CollisionNorm_Out, true, false, 0.0f)
{

}

Point2D::Point2D(const VECTOR2D &pos, GameObject *myObject, CollisionTag tag, bool penetrate, bool hitMove, float moveLate) :
    BaseCollision2D(myObject, CollisionType::CollisionType_Point2D, tag, CollisionNorm_Out, penetrate, hitMove, moveLate)
{

}

Point2D::~Point2D()
{

}

int Point2D::Draw(const Material2D &color)
{
    return 0;
}

int Point2D::SetPosToMoveVec()
{
    VECTOR2D moveVec = this->GetMoveVec();
    if (moveVec != VECTOR2D::GetZero())
    {
        this->SetBasePos(this->GetBasePos() + moveVec);

        this->SetMoveVec(VECTOR2D::GetZero());
    }

    return 0;
}

int Point2D::SlideMove(const CollisionCheckResult2D &result, float moveLate)
{
    return 0;
}

int Point2D::SetBaseParamMySelf()
{
    return 0;
}

int Point2D::GetAABB(AABB2D &out)
{
    out.mvMinPos = this->GetBasePos();
    out.mvMaxPos = this->GetBasePos();
    out.mvMoveVec = this->GetMoveVec();
    return 0;
}

void Point2D::SetShapeParameter(const VECTOR2D &pos)
{
    this->SetBasePos(pos);
}