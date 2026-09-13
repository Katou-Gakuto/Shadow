#include "Ray2D.h"

Ray2D::Ray2D() :
    mvStartPos(VECTOR2D::GetZero()),
    mvEndPos(VECTOR2D::GetZero()),
    BaseCollision2D(nullptr, CollisionType::CollisionType_Ray2D, CollisionTag_Checker, CollisionNorm_Out, true, false, 0.0f)
{

}

Ray2D::Ray2D(
    const VECTOR2D &startPos, const VECTOR2D &endPos,
    GameObject *myObject, CollisionTag tag,
    bool penetrate, bool hitMove, float moveLate) :
    mvStartPos(startPos),
    mvEndPos(endPos),
    BaseCollision2D(myObject, CollisionType::CollisionType_Ray2D, tag, CollisionNorm_Out, penetrate, hitMove, moveLate)
{

}

Ray2D::~Ray2D()
{

}

int Ray2D::Draw(const Material2D &color)
{
    return 0;
}

int Ray2D::SetPosToMoveVec()
{
    VECTOR2D moveVec = this->GetMoveVec();
    if (moveVec != VECTOR2D::GetZero())
    {
        this->mvStartPos = this->mvStartPos + moveVec;
        this->mvEndPos = this->mvEndPos + moveVec;
        this->SetBasePosUpdateFlag(true);

        this->SetMoveVec(VECTOR2D::GetZero());
    }

    return 0;
}

int Ray2D::SlideMove(const CollisionCheckResult2D &result, float moveLate)
{
    return 0;
}

int Ray2D::SetBaseParamMySelf()
{
    // 円の中心座標を更新する必要がある場合のみ処理を行う
    if (this->GetBasePosUpdateFlag())
    {
        // Rayの中心座標を取得する
        VECTOR2D centerPos = VECTOR2D::GetLerpPos(this->mvStartPos, this->mvEndPos, 0.5f);
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

int Ray2D::GetAABB(AABB2D &out)
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
    out.mvMoveVec = this->GetMoveVec();
    return 0;
}

void Ray2D::SetShapeParameter(const VECTOR2D &startPos, const VECTOR2D &endPos)
{
    this->mvStartPos = startPos;
    this->mvEndPos = endPos;

    // 値の変更があったことを記憶しておく
    this->SetBasePosUpdateFlag(true);
}