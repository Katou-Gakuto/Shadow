#include "Quadrangle2D.h"

#include "CollisionCheckResult.h"
#include "BaseCollisionList.h"
#include "../A_GameObject/GameObject2D.h"
#include "../Y_Tool/MyFunctions.h"

Quadrangle2D::Quadrangle2D() : 
    mvVertexPos(),
    BaseCollision2D(nullptr, CollisionType::CollisionType_Quadrangle2D, CollisionTag_Checker, CollisionNorm_Out, true, false, 0.0f)
{

}

Quadrangle2D::Quadrangle2D(
    const VECTOR2D &pos0, const VECTOR2D &pos1, const VECTOR2D &pos2, const VECTOR2D &pos3,
    GameObject *myObject, CollisionTag tag, CollisionNorm normNum,
    bool penetrate, bool hitMove, float moveLate) :
    mvVertexPos(),
    BaseCollision2D(myObject, CollisionType::CollisionType_Quadrangle2D, tag, normNum, penetrate, hitMove, moveLate)
{
    // 頂点の座標を設定する
    this->mvVertexPos[0] = pos0;
    this->mvVertexPos[1] = pos1;
    this->mvVertexPos[2] = pos2;
    this->mvVertexPos[3] = pos3;
}

Quadrangle2D::~Quadrangle2D()
{

}

int Quadrangle2D::Draw(const Material2D &color)
{
    return 0;
}

int Quadrangle2D::SetPosToMoveVec()
{
    VECTOR2D moveVec = this->GetMoveVec();
    if (moveVec != VECTOR2D::GetZero())
    {
        this->mvVertexPos[0] += moveVec;
        this->mvVertexPos[1] += moveVec;
        this->mvVertexPos[2] += moveVec;
        this->mvVertexPos[3] += moveVec;
        this->SetBasePosUpdateFlag(true);

        this->SetMoveVec(VECTOR2D::GetZero());
    }

    return 0;
}

int Quadrangle2D::SlideMove(const CollisionCheckResult2D &result, float moveLate)
{
        if (this == result.mpCollisionA)
        {
            VECTOR2D myMoveVec = this->GetMoveVec();
            if (result.mpCollisionB->GetCollisionType() == CollisionType::CollisionType_Quadrangle2D)
            {
                VECTOR2D sideNormVec = (result.mvRepulsionVecA).Normalize();
                if (1.0f < moveLate)
                {
                    // 自分じゃない方の当たり判定の移動ベクトルを取得する
                    VECTOR2D targetMoveVec = dynamic_cast<Quadrangle2D *>(result.mpCollisionB)->GetMoveVec();

                    // スライドの進行ベクトルを取得
                    VECTOR2D slideVec = VECTOR2D::GetSlide(myMoveVec, sideNormVec);

                    // 現在の移動の進行ベクトルにスライドに進行ベクトルと、相手の移動ベクトルを加えて、新しい移動の進行ベクトルを取得する
                    VECTOR2D newMoveVec = myMoveVec + slideVec +(targetMoveVec * (moveLate - 1.0f));

                    // 自身の移動ベクトルを更新する
                    this->SetMoveVec(newMoveVec);
                    if (this->GetMyObject2D() != nullptr)
                    {
                        // オブジェクトの移動ベクトルを更新する
                        this->GetMyObject2D()->SetMoveVec(newMoveVec);

                        // オブジェクトの当たり判定全ての移動ベクトルを更新する
                        this->GetMyObject2D()->GetBaseCollisionList()->SetCollisionMoveVec(CollisionDimension_2D, &newMoveVec);
                    }
                }
                else
                {
                    // 壁ずり移動の進行ベクトル保持する変数
                    VECTOR2D slideVec = VECTOR2D::GetSlide(myMoveVec, sideNormVec);

                    // 現在の移動の進行ベクトルから壁の法線成分を抜いた新しい進行ベクトルを取得する
                    VECTOR2D newMoveVec = myMoveVec + (slideVec * moveLate);

                    // 自身の移動ベクトルを更新する
                    this->SetMoveVec(newMoveVec);
                    if (this->GetMyObject2D() != nullptr)
                    {
                        // オブジェクトの移動ベクトルを更新する
                        this->GetMyObject2D()->SetMoveVec(newMoveVec);

                        // オブジェクトの当たり判定全ての移動ベクトルを更新する
                        this->GetMyObject2D()->GetBaseCollisionList()->SetCollisionMoveVec(CollisionDimension_2D, &newMoveVec);
                    }
                }
            }
        }
        else if (this == result.mpCollisionB)
        {
            VECTOR2D myMoveVec = this->GetMoveVec();
            if (result.mpCollisionA->GetCollisionType() == CollisionType::CollisionType_Quadrangle2D)
            {
                VECTOR2D sideNormVec = result.mvRepulsionVecB.Normalize();

                if (1.0f < moveLate)
                {
                    // 自分じゃない方の当たり判定の移動ベクトルを取得する
                    VECTOR2D targetMoveVec = dynamic_cast<Quadrangle2D *>(result.mpCollisionA)->GetMoveVec();

                    // スライドの進行ベクトルを取得
                    VECTOR2D slideVec = VECTOR2D::GetSlide(myMoveVec, sideNormVec);

                    // 現在の移動の進行ベクトルにスライドに進行ベクトルと、相手の移動ベクトルを加えて、新しい移動の進行ベクトルを取得する
                    VECTOR2D newMoveVec = myMoveVec + slideVec +(targetMoveVec * (moveLate - 1.0f));

                    // 自身の移動ベクトルを更新する
                    this->SetMoveVec(newMoveVec);
                    if (this->GetMyObject2D() != nullptr)
                    {
                        // オブジェクトの移動ベクトルを更新する
                        this->GetMyObject2D()->SetMoveVec(newMoveVec);

                        // オブジェクトの当たり判定全ての移動ベクトルを更新する
                        this->GetMyObject2D()->GetBaseCollisionList()->SetCollisionMoveVec(CollisionDimension_2D, &newMoveVec);
                    }
                }
                else
                {
                    // 壁ずり移動の進行ベクトル保持する変数
                    VECTOR2D slideVec = VECTOR2D::GetSlide(myMoveVec, sideNormVec);

                    // 現在の移動の進行ベクトルから壁の法線成分を抜いた新しい進行ベクトルを取得する
                    VECTOR2D newMoveVec = myMoveVec + (slideVec * moveLate);

                    // 自身の移動ベクトルを更新する
                    this->SetMoveVec(newMoveVec);
                    if (this->GetMyObject2D() != nullptr)
                    {
                        // オブジェクトの移動ベクトルを更新する
                        this->GetMyObject2D()->SetMoveVec(newMoveVec);

                        // オブジェクトの当たり判定全ての移動ベクトルを更新する
                        this->GetMyObject2D()->GetBaseCollisionList()->SetCollisionMoveVec(CollisionDimension_2D, &newMoveVec);
                    }
                }
            }
        }

    return 0;
}

int Quadrangle2D::SetBaseParamMySelf()
{
    // 値が変わっている可能性がある場合のみ処理を行う
    if (this->GetBasePosUpdateFlag() || this->GetBaseRadiusUpdateFlag())
    {
        VECTOR2D basePos = VECTOR2D::GetZero();
        VECTOR2D maxDistance = this->mvVertexPos[1] - this->mvVertexPos[0];
        float maxMagNoSqrt = maxDistance.MagnitudeNoSqrt();

        VECTOR2D currentDistance = VECTOR2D::GetZero();
        float currentMagNoSqrt = 0.0f;

        for (int i = 0; i < 4; i++)
        {
            for (int j = i + 1; j < 4; j++)
            {
                if (i == 0 && j == 1)
                {
                    continue;
                }

                // 現在のベクトルとそのベクトルの大きさを取得する
                currentDistance = this->mvVertexPos[j] - this->mvVertexPos[i];
                currentMagNoSqrt = currentDistance.MagnitudeNoSqrt();

                // 今回のベクトルの大きさが前回のベクトルよりも大きかったら更新処理を行う
                if (maxMagNoSqrt < currentMagNoSqrt)
                {
                    // この図形がすっぽり埋まる円の半径を取得する
                    maxMagNoSqrt = currentMagNoSqrt;

                    // この図形がすっぽり埋まる円の中心点を取得する
                    basePos = VECTOR2D::GetLerpPos(this->mvVertexPos[i], this->mvVertexPos[j], 0.5f);
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

int Quadrangle2D::GetAABB(AABB2D &out)
{
    float minX = this->mvVertexPos[0].GetX();
    float minY = this->mvVertexPos[0].GetY();
    float maxX = this->mvVertexPos[0].GetX();
    float maxY = this->mvVertexPos[0].GetY();
    for (int i = 1; i < 4; i++)
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

void Quadrangle2D::SetShapeParameter(const VECTOR2D &pos0, const VECTOR2D &pos1, const VECTOR2D &pos2, const VECTOR2D &pos3)
{
    // 頂点の座標を設定する
    this->mvVertexPos[0] = pos0;
    this->mvVertexPos[1] = pos1;
    this->mvVertexPos[2] = pos2;
    this->mvVertexPos[3] = pos3;

    // 値の変更があったことを記憶しておく
    this->SetBasePosUpdateFlag(true);
}

VECTOR2D Quadrangle2D::GetVertexPos(int index)
{
    if (0 <= index && index < 4)
    {
        return this->mvVertexPos[index];
    }
    else
    {
        return VECTOR2D::GetZero();
    }
}