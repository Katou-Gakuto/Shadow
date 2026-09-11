#include "Capsule2D.h"

Capsule2D::Capsule2D() :
    mvCenterLineStartPos(VECTOR2D::GetZero()),
    mvCenterLineEndPos(VECTOR2D::GetZero()),
    mfCenterLineRadiusSqrt(0.0f),
    BaseCollision2D(nullptr, CollisionType::CollisionType_Capsule2D, CollisionTag_Checker, CollisionNorm::CollisionNorm_Out, true, false, 0.0f)
{

}

Capsule2D::Capsule2D(
    const VECTOR2D &startPos, const VECTOR2D &endPos, float radius,
    GameObject *myObject, CollisionTag tag, CollisionNorm normNum,
    bool penetrate, bool hitMove, float moveLate):
    mvCenterLineStartPos(startPos),
    mvCenterLineEndPos(endPos),
    mfCenterLineRadiusSqrt(radius),
    BaseCollision2D(myObject, CollisionType::CollisionType_Capsule2D, tag, normNum, penetrate, hitMove, moveLate)
{
    // BaseParamを更新する
    this->SetBaseParamMySelf();
}

Capsule2D::~Capsule2D()
{

}

int Capsule2D::Draw(const Material2D &color)
{
    return 0;
}

int Capsule2D::SetPosToMoveVec()
{
    VECTOR2D moveVec = this->GetMoveVec();
    if (moveVec != VECTOR2D::GetZero(), 0.0001f)
    {
        this->mvCenterLineStartPos += moveVec;
        this->mvCenterLineEndPos += moveVec;
        this->SetBasePosUpdateFlag(true);

        this->SetMoveVec(VECTOR2D::GetZero());
    }

    return 0;
}

int Capsule2D::SlideMove(const CollisionCheckResult2D &result, float moveLate)
{
    return 0;
}

int Capsule2D::SetBaseParamMySelf()
{
    // 円の中心座標を更新する必要がある場合のみ処理を行う
    if (this->GetBasePosUpdateFlag())
    {
        // カプセルの中心座標を取得する
        VECTOR2D centerPos = VECTOR2D::GetLerpPos(this->mvCenterLineStartPos, this->mvCenterLineEndPos, 0.5f);
        this->SetBasePos(centerPos);

        // フラグを整える
        this->SetBasePosUpdateFlag(false);
    }

    // 円の半径を更新する必要がある場合のみ処理を行う
    if (this->GetBaseRadiusUpdateFlag())
    {
        // カプセルを包む円の半径を取得する
        float radiusNoSqrt =
            ((this->mvCenterLineEndPos - this->mvCenterLineStartPos) * 0.5f).MagnitudeNoSqrt() +
            (this->mfCenterLineRadiusSqrt * this->mfCenterLineRadiusSqrt);
        this->SetBaseRadiusNoSqrt(radiusNoSqrt);

        // フラグを整える
        this->SetBaseRadiusUpdateFlag(false);
    }

    return 0;
}

int Capsule2D::GetAABB(AABB2D &out)
{
    if (this->mvCenterLineStartPos.GetX() < this->mvCenterLineEndPos.GetX())
    {
        out.mvMinPos.SetX(this->mvCenterLineStartPos.GetX() - this->mfCenterLineRadiusSqrt);
        out.mvMaxPos.SetX(this->mvCenterLineEndPos.GetX() + this->mfCenterLineRadiusSqrt);
    }
    else
    {
        out.mvMinPos.SetX(this->mvCenterLineEndPos.GetX() - this->mfCenterLineRadiusSqrt);
        out.mvMaxPos.SetX(this->mvCenterLineStartPos.GetX() + this->mfCenterLineRadiusSqrt);
    }
    if (this->mvCenterLineStartPos.GetY() < this->mvCenterLineEndPos.GetY())
    {
        out.mvMinPos.SetY(this->mvCenterLineStartPos.GetY() - this->mfCenterLineRadiusSqrt);
        out.mvMaxPos.SetY(this->mvCenterLineEndPos.GetY() + this->mfCenterLineRadiusSqrt);
    }
    else
    {
        out.mvMinPos.SetY(this->mvCenterLineEndPos.GetY() - this->mfCenterLineRadiusSqrt);
        out.mvMaxPos.SetY(this->mvCenterLineStartPos.GetY() + this->mfCenterLineRadiusSqrt);
    }
    out.mvMoveVec = this->GetMoveVec();
    return 0;
}

void Capsule2D::SetShapeParameter(const VECTOR2D &startPos, const VECTOR2D &endPos, float radiusNoSqrt)
{
    this->mvCenterLineEndPos = endPos;
    this->SetBasePos(startPos);
    this->SetBaseRadiusNoSqrt(radiusNoSqrt);

    // 値の変更があったことを記憶しておく
    this->SetBasePosUpdateFlag(true);
    this->SetBaseRadiusUpdateFlag(true);
}