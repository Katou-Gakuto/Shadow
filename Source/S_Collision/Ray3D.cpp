#include "Ray3D.h"

Ray3D::Ray3D() :
    mvStartPos(VECTOR3D::GetZero()),
    mvEndPos(VECTOR3D::GetZero()),
    BaseCollision3D(nullptr, CollisionType::CollisionType_Ray3D, CollisionTag_Checker, CollisionNorm_Out, true, false, 0.0f)
{

}

Ray3D::Ray3D(
    const VECTOR3D &startPos, const VECTOR3D &endPos,
    GameObject *myObject, CollisionTag tag,
    bool penetrate, bool hitMove, float moveLate) :
    mvStartPos(startPos),
    mvEndPos(endPos),
    BaseCollision3D(myObject, CollisionType::CollisionType_Ray3D, tag, CollisionNorm_Out, penetrate, hitMove, moveLate)
{

}

Ray3D::~Ray3D()
{

}

int Ray3D::Draw(const Material2D &color)
{
    return 0;
}

int Ray3D::SetPosToMoveVec()
{
    VECTOR3D moveVec = this->GetMoveVec();
    if (moveVec != VECTOR3D::GetZero())
    {
        this->mvStartPos = this->mvStartPos + moveVec;
        this->mvEndPos = this->mvEndPos + moveVec;
        this->SetBasePosUpdateFlag(true);

        this->SetMoveVec(VECTOR3D::GetZero());
    }

    return 0;
}

int Ray3D::SlideMove(const CollisionCheckResult3D &result, float moveLate)
{
    return 0;
}

int Ray3D::SetBaseParamMySelf()
{
    // 円の中心座標を更新する必要がある場合のみ処理を行う
    if (this->GetBasePosUpdateFlag())
    {
        // Rayの中心座標を取得する
        VECTOR3D centerPos = VECTOR3D::GetLerpPos(this->mvStartPos, this->mvEndPos, 0.5f);
        this->SetBasePos(centerPos);

        // フラグを整える
        this->SetBasePosUpdateFlag(false);
    }

    // 円の半径を更新する必要がある場合のみ処理を行う
    if (this->GetBaseRadiusUpdateFlag())
    {
        // Rayの長さの半分を取得する
        float radiusNoSqrt = (this->mvEndPos - this->mvStartPos).MagnitudeNoSqrt() * 0.5f;
        this->SetBaseRadiusNoSqrt(radiusNoSqrt);

        // フラグを整える
        this->SetBaseRadiusUpdateFlag(false);
    }

    return 0;
}

int Ray3D::GetAABB(AABB3D &out)
{
    if (this->mvStartPos.GetX() < this->mvEndPos.GetX())
    {
        out.mvMinPos.SetX(this->mvStartPos.GetX());
        out.mvMaxPos.SetX(this->mvEndPos.GetX());
    }
    else
    {
        out.mvMinPos.SetX(this->mvEndPos.GetX());
        out.mvMaxPos.SetX(this->mvStartPos.GetX());
    }
    if (this->mvStartPos.GetY() < this->mvEndPos.GetY())
    {
        out.mvMinPos.SetY(this->mvStartPos.GetY());
        out.mvMaxPos.SetY(this->mvEndPos.GetY());
    }
    else
    {
        out.mvMinPos.SetY(this->mvEndPos.GetY());
        out.mvMaxPos.SetY(this->mvStartPos.GetY());
    }
    if (this->mvStartPos.GetZ() < this->mvEndPos.GetZ())
    {
        out.mvMinPos.SetZ(this->mvStartPos.GetZ());
        out.mvMaxPos.SetZ(this->mvEndPos.GetZ());
    }
    else
    {
        out.mvMinPos.SetZ(this->mvEndPos.GetZ());
        out.mvMaxPos.SetZ(this->mvStartPos.GetZ());
    }
    out.mvMoveVec = this->GetMoveVec();
    return 0;
}


void Ray3D::SetShapeParameter(const VECTOR3D &startPos, const VECTOR3D &endPos)
{
    this->mvStartPos = startPos;
    this->mvEndPos = endPos;

    // 値の変更があったことを記憶しておく
    this->SetBasePosUpdateFlag(true);
}