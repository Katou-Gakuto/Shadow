#include "Point3D.h"

Point3D::Point3D() :
    BaseCollision3D(nullptr, CollisionType::CollisionType_Point3D, CollisionTag_Checker, CollisionNorm_Out, true, false, 0.0f)
{

}

Point3D::Point3D(const VECTOR3D &pos, GameObject *myObject, CollisionTag tag, bool penetrate, bool hitMove, float moveLate) :
    BaseCollision3D(myObject, CollisionType::CollisionType_Point3D, tag, CollisionNorm_Out, penetrate, hitMove, moveLate)
{

}

Point3D::~Point3D()
{

}


int Point3D::Draw(const Material2D &color)
{
    return 0;
}

int Point3D::SetPosToMoveVec()
{
    VECTOR3D moveVec = this->GetMoveVec();
    if (moveVec != VECTOR3D::GetZero())
    {
        this->SetBasePos(this->GetBasePos() + moveVec);

        this->SetMoveVec(VECTOR3D::GetZero());
    }

    return 0;
}

int Point3D::SlideMove(const CollisionCheckResult3D &result, float moveLate)
{
    return 0;
}

int Point3D::SetBaseParamMySelf()
{
    return 0;
}

int Point3D::GetAABB(AABB3D &out)
{
    out.mvMinPos = this->GetBasePos();
    out.mvMaxPos = this->GetBasePos();
    out.mvMoveVec = this->GetMoveVec();
    return 0;
}

void Point3D::SetShapeParameter(const VECTOR3D &pos)
{
    this->SetBasePos(pos);
}