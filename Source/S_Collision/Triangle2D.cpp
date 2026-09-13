#include "Triangle2D.h"

Triangle2D::Triangle2D() :
    mvVertexPos(),
    BaseCollision2D(nullptr, CollisionType::CollisionType_Triangle2D, CollisionTag_Checker, CollisionNorm_Out, true, false, 0.0f)
{
    for (int i = 0; i < 3; i++)
    {
        this->mvVertexPos[i] = VECTOR2D::GetZero();
    }
}

Triangle2D::Triangle2D(
    const VECTOR2D &pos0, const VECTOR2D &pos1, const VECTOR2D &pos2,
    GameObject *myObject, CollisionTag tag, CollisionNorm normNum,
    bool penetrate, bool hitMove, float moveLate) :
    mvVertexPos(),
    BaseCollision2D(myObject, CollisionType::CollisionType_Triangle2D, tag, normNum, penetrate, hitMove, moveLate)
{
    // 頂点の座標を設定する
    this->mvVertexPos[0] = pos0;
    this->mvVertexPos[1] = pos1;
    this->mvVertexPos[2] = pos2;

    // BaseParamを設定する
    this->SetBaseParamMySelf();
}

Triangle2D::~Triangle2D()
{

}

int Triangle2D::Draw(const Material2D &color)
{
    return 0;
}

int Triangle2D::SetPosToMoveVec()
{

    VECTOR2D moveVec = this->GetMoveVec();
    if (moveVec != VECTOR2D::GetZero())
    {
        this->mvVertexPos[0] = (this->mvVertexPos[0] + moveVec);
        this->mvVertexPos[1] = (this->mvVertexPos[1] + moveVec);
        this->mvVertexPos[2] = (this->mvVertexPos[2] + moveVec);
        this->SetBasePosUpdateFlag(true);

        this->SetMoveVec(VECTOR2D::GetZero());
    }

    return 0;
}

int Triangle2D::SlideMove(const CollisionCheckResult2D &result, float moveLate)
{
    return 0;
}

int Triangle2D::SetBaseParamMySelf()
{
    // 値が変わっている可能性がある場合のみ処理を行う
    if (this->GetBasePosUpdateFlag() || this->GetBaseRadiusUpdateFlag())
    {
        VECTOR2D basePos = VECTOR2D::GetZero();
        VECTOR2D maxDistance = this->mvVertexPos[1] - this->mvVertexPos[0];
        float maxMagNoSqrt = maxDistance.MagnitudeNoSqrt();

        VECTOR2D currentDistance = VECTOR2D::GetZero();
        float currentMagNoSqrt = 0.0f;

        for (int i = 0; i < 3; i++)
        {
            for (int j = i + 1; j < 3; j++)
            {
                // 現在のベクトルとそのベクトルの大きさを取得する
                currentDistance = this->mvVertexPos[j] - this->mvVertexPos[i];
                currentMagNoSqrt = currentDistance.MagnitudeNoSqrt();

                // 今回のベクトルの大きさが前回のベクトルよりも大きかったら更新処理を行う
                if (maxMagNoSqrt < currentMagNoSqrt)
                {
                    // この図形がすっぽり埋まる円の半径を取得する
                    maxMagNoSqrt = currentMagNoSqrt;
                    
                    // この図形がすっぽり埋まる円の中心点を取得する
                    basePos = VECTOR2D::GetLerpPos((this->mvVertexPos[i]), (this->mvVertexPos[j]), 0.5f);
                }
            }
        }

        // 計算結果をBaseParamに反映する
        this->SetBasePos(basePos);
        this->SetBaseRadiusNoSqrt(maxMagNoSqrt);

        // フラグを整える
        this->SetBasePosUpdateFlag(false);
        this->SetBaseRadiusUpdateFlag(false);
    }

    return 0;
}

int Triangle2D::GetAABB(AABB2D &out)
{
    float minX = this->mvVertexPos[0].GetX();
    float minY = this->mvVertexPos[0].GetY();
    float maxX = this->mvVertexPos[0].GetX();
    float maxY = this->mvVertexPos[0].GetY();
    for (int i = 1; i < 3; i++)
    {
        if (this->mvVertexPos[i].GetX() < minX)
        {
            minX = this->mvVertexPos[i].GetX();
        }
        else if (maxX < this->mvVertexPos[i].GetX())
        {
            maxX = this->mvVertexPos[i].GetX();
        }
        if (this->mvVertexPos[i].GetY() < minY)
        {
            minY = this->mvVertexPos[i].GetY();
        }
        else if (maxY < this->mvVertexPos[i].GetY())
        {
            maxY = this->mvVertexPos[i].GetY();
        }
    }
    out.mvMinPos.SetXY(minX, minY);
    out.mvMaxPos.SetXY(maxX, maxY);
    out.mvMoveVec = this->GetMoveVec();
    return 0;
}

void Triangle2D::SetShapeParameter(const VECTOR2D &pos0, const VECTOR2D &pos1, const VECTOR2D &pos2)
{
    // 頂点の座標を設定する
    this->mvVertexPos[0] = pos0;
    this->mvVertexPos[1] = pos1;
    this->mvVertexPos[2] = pos2;

    // 値の変更があったことを記憶しておく
    this->SetBasePosUpdateFlag(true);
    this->SetBaseRadiusUpdateFlag(true);
}

VECTOR2D Triangle2D::GetVertexPos(int index)
{
    if (0 <= index && index < 3)
    {
        return this->mvVertexPos[index];
    }
    else
    {
        return VECTOR2D::GetZero();
    }
}