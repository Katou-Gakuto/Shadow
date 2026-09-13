#include "Quadrangle3D.h"

#include "CollisionCheckResult.h"
#include "Triangle3D.h"
#include "BaseCollisionList.h"
#include "../A_GameObject/GameObject3D.h"
#include "../Y_Tool/MyFunctions.h"

Quadrangle3D::Quadrangle3D() :
    mvVertexPos(),
    BaseCollision3D(nullptr, CollisionType::CollisionType_Quadrangle3D, CollisionTag_Checker, CollisionNorm_Out, true, false, 0.0f)
{

}

Quadrangle3D::Quadrangle3D(
    const VECTOR3D &pos0, const VECTOR3D &pos1, const VECTOR3D &pos2, const VECTOR3D &pos3,
    GameObject *myObject, CollisionTag tag, CollisionNorm normNum,
    bool penetrate, bool hitMove, float moveLate) :
    mvVertexPos(),
    BaseCollision3D(myObject, CollisionType::CollisionType_Quadrangle3D, tag, normNum, penetrate, hitMove, moveLate)
{
    // 頂点の座標を設定する
    this->mvVertexPos[0] = pos0;
    this->mvVertexPos[1] = pos1;
    this->mvVertexPos[2] = pos2;
    this->mvVertexPos[3] = pos3;
}

Quadrangle3D::~Quadrangle3D()
{

}

int Quadrangle3D::GetTriangle3D(Triangle3D *dest, int index)
{
    if (dest == nullptr)
    {
        return -1;
    }
    if ((index == 0 || index == 1) == false)
    {
        return -1;
    }
    if (index == 0)
    {
        dest->SetShapeParameter(
            this->mvVertexPos[0],
            this->mvVertexPos[1],
            this->mvVertexPos[2]);
    }
    else
    {
        dest->SetShapeParameter(
            this->mvVertexPos[1],
            this->mvVertexPos[3],
            this->mvVertexPos[2]);
    }
    dest->SetMoveVec(this->GetMoveVec());
    return 0;
}

int Quadrangle3D::Draw(const Material2D &color)
{
    return 0;
}

int Quadrangle3D::SetPosToMoveVec()
{
    VECTOR3D moveVec = this->GetMoveVec();
    if (moveVec != VECTOR3D::GetZero())
    {
        this->mvVertexPos[0] += moveVec;
        this->mvVertexPos[1] += moveVec;
        this->mvVertexPos[2] += moveVec;
        this->mvVertexPos[3] += moveVec;
        this->SetBasePosUpdateFlag(true);

        this->SetMoveVec(VECTOR3D::GetZero());
    }

    return 0;
}

int Quadrangle3D::SlideMove(const CollisionCheckResult3D &result, float moveLate)
{
    if (this == result.mpCollisionA)
    {
        VECTOR3D myMoveVec = this->GetMoveVec();
        if (result.mpCollisionB->GetCollisionType() == CollisionType::CollisionType_Quadrangle2D)
        {
            VECTOR3D sideNormVec = (result.mvRepulsionVecA).Normalize();
            if (1.0f < moveLate)
            {
                // 自分じゃない方の当たり判定の移動ベクトルを取得する
                VECTOR3D targetMoveVec = dynamic_cast<Quadrangle3D *>(result.mpCollisionB)->GetMoveVec();

                // スライドの進行ベクトルを取得
                VECTOR3D slideVec = VECTOR3D::GetSlide(myMoveVec, sideNormVec);

                // 現在の移動の進行ベクトルにスライドに進行ベクトルと、相手の移動ベクトルを加えて、新しい移動の進行ベクトルを取得する
                VECTOR3D newMoveVec = myMoveVec + slideVec + (targetMoveVec * (moveLate - 1.0f));

                // 自身の移動ベクトルを更新する
                this->SetMoveVec(newMoveVec);
                if (this->GetMyObject() != nullptr)
                {
                    // オブジェクトの移動ベクトルを更新する
                    dynamic_cast<GameObject3D *>(this->GetMyObject())->SetMoveVec3D(newMoveVec);

                    // オブジェクトの当たり判定全ての移動ベクトルを更新する
                    dynamic_cast<GameObject3D *>(this->GetMyObject())->GetBaseCollisionList()->SetCollisionMoveVec(CollisionDimension_3D, &newMoveVec);
                }
            }
            else
            {
                // 壁ずり移動の進行ベクトル保持する変数
                VECTOR3D slideVec = VECTOR3D::GetSlide(myMoveVec, sideNormVec);

                // 現在の移動の進行ベクトルから壁の法線成分を抜いた新しい進行ベクトルを取得する
                VECTOR3D newMoveVec = myMoveVec + (slideVec * moveLate);

                // 自身の移動ベクトルを更新する
                this->SetMoveVec(newMoveVec);
                if (this->GetMyObject() != nullptr)
                {
                    // オブジェクトの移動ベクトルを更新する
                    dynamic_cast<GameObject3D *>(this->GetMyObject())->SetMoveVec3D(newMoveVec);

                    // オブジェクトの当たり判定全ての移動ベクトルを更新する
                    dynamic_cast<GameObject3D *>(this->GetMyObject())->GetBaseCollisionList()->SetCollisionMoveVec(CollisionDimension_3D, &newMoveVec);
                }
            }
        }
    }
    else if (this == result.mpCollisionB)
    {
        VECTOR3D myMoveVec = this->GetMoveVec();
        if (result.mpCollisionA->GetCollisionType() == CollisionType::CollisionType_Quadrangle2D)
        {
            VECTOR3D sideNormVec = result.mvRepulsionVecB.Normalize();

            if (1.0f < moveLate)
            {
                // 自分じゃない方の当たり判定の移動ベクトルを取得する
                VECTOR3D targetMoveVec = dynamic_cast<Quadrangle3D *>(result.mpCollisionA)->GetMoveVec();

                // スライドの進行ベクトルを取得
                VECTOR3D slideVec = VECTOR3D::GetSlide(myMoveVec, sideNormVec);

                // 現在の移動の進行ベクトルにスライドに進行ベクトルと、相手の移動ベクトルを加えて、新しい移動の進行ベクトルを取得する
                VECTOR3D newMoveVec = myMoveVec + slideVec + (targetMoveVec * (moveLate - 1.0f));

                // 自身の移動ベクトルを更新する
                this->SetMoveVec(newMoveVec);
                if (this->GetMyObject() != nullptr)
                {
                    // オブジェクトの移動ベクトルを更新する
                    dynamic_cast<GameObject3D *>(this->GetMyObject())->SetMoveVec3D(newMoveVec);

                    // オブジェクトの当たり判定全ての移動ベクトルを更新する
                    dynamic_cast<GameObject3D *>(this->GetMyObject())->GetBaseCollisionList()->SetCollisionMoveVec(CollisionDimension_3D, &newMoveVec);
                }

            }
            else
            {
                // 壁ずり移動の進行ベクトル保持する変数
                VECTOR3D slideVec = VECTOR3D::GetSlide(myMoveVec, sideNormVec);

                // 現在の移動の進行ベクトルから壁の法線成分を抜いた新しい進行ベクトルを取得する
                VECTOR3D newMoveVec = myMoveVec + (slideVec * moveLate);

                // 自身の移動ベクトルを更新する
                this->SetMoveVec(newMoveVec);
                if (this->GetMyObject() != nullptr)
                {
                    // オブジェクトの移動ベクトルを更新する
                    dynamic_cast<GameObject3D *>(this->GetMyObject())->SetMoveVec3D(newMoveVec);

                    // オブジェクトの当たり判定全ての移動ベクトルを更新する
                    dynamic_cast<GameObject3D *>(this->GetMyObject())->GetBaseCollisionList()->SetCollisionMoveVec(CollisionDimension_3D, &newMoveVec);
                }
            }
        }
    }

    return 0;
}

int Quadrangle3D::SetBaseParamMySelf()
{
    // 値が変わっている可能性がある場合のみ処理を行う
    if (this->GetBasePosUpdateFlag() || this->GetBaseRadiusUpdateFlag())
    {
        VECTOR3D basePos = VECTOR3D::GetZero();
        VECTOR3D maxDistance = this->mvVertexPos[1] - this->mvVertexPos[0];
        float maxMagNoSqrt = maxDistance.MagnitudeNoSqrt();

        VECTOR3D currentDistance = VECTOR3D::GetZero();
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
                    basePos = VECTOR3D::GetLerpPos(this->mvVertexPos[i], this->mvVertexPos[j], 0.5f);
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

int Quadrangle3D::GetAABB(AABB3D &out)
{
    float minX = this->mvVertexPos[0].GetX();
    float minY = this->mvVertexPos[0].GetY();
    float minZ = this->mvVertexPos[0].GetZ();
    float maxX = this->mvVertexPos[0].GetX();
    float maxY = this->mvVertexPos[0].GetY();
    float maxZ = this->mvVertexPos[0].GetZ();
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
        if (this->mvVertexPos[i].GetZ() < minZ)
        {
            minZ = this->mvVertexPos[i].GetZ();
        }
        else if (maxZ < this->mvVertexPos[i].GetZ())
        {
            maxZ = this->mvVertexPos[i].GetZ();
        }
    }
    out.mvMinPos.SetXYZ(minX, minY, minZ);
    out.mvMaxPos.SetXYZ(maxX, maxY, maxZ);
    out.mvMoveVec = this->GetMoveVec();
    return 0;
}

void Quadrangle3D::SetShapeParameter(const VECTOR3D &pos0, const VECTOR3D &pos1, const VECTOR3D &pos2, const VECTOR3D &pos3)
{
    // 頂点の座標を設定する
    this->mvVertexPos[0] = pos0;
    this->mvVertexPos[1] = pos1;
    this->mvVertexPos[2] = pos2;
    this->mvVertexPos[3] = pos3;

    // 値の変更があったことを記憶しておく
    this->SetBasePosUpdateFlag(true);
}

VECTOR3D Quadrangle3D::GetVertexPos(int index)
{
    if (0 <= index && index < 4)
    {
        return this->mvVertexPos[index];
    }
    else
    {
        return VECTOR3D::GetZero();
    }
}

VECTOR3D Quadrangle3D::GetNormVec()
{
    return VECTOR3D::Cross((this->mvVertexPos[1] - this->mvVertexPos[0]), (this->mvVertexPos[2] - this->mvVertexPos[0])).Normalize();
}