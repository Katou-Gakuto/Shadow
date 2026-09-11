#include "Capsule3D.h"

#include "CollisionCheckResult.h"
#include "Cube3D.h"

#include "../A_GameObject/GameObject3D.h"
#include "BaseCollisionList.h"

Capsule3D::Capsule3D() :
    mvCenterLineStartPos(VECTOR3D::GetZero()),
    mvCenterLineEndPos(VECTOR3D::GetZero()),
    mfCenterLineRadiusSqrt(0.0f),
    BaseCollision3D(nullptr, CollisionType::CollisionType_Capsule3D, CollisionTag_Checker, CollisionNorm::CollisionNorm_Out, true, false, 0.0f)
{

}

Capsule3D::Capsule3D(
    const VECTOR3D &startPos, const VECTOR3D &endPos, float radius,
    GameObject *myObject, CollisionTag tag, CollisionNorm normNum,
    bool penetrate, bool hitMove, float moveLate) :
    mvCenterLineStartPos(startPos),
    mvCenterLineEndPos(endPos),
    mfCenterLineRadiusSqrt(radius),
    BaseCollision3D(myObject, CollisionType::CollisionType_Capsule3D, tag, normNum, penetrate, hitMove, moveLate)
{
    this->SetBaseParamMySelf();
}

Capsule3D::~Capsule3D()
{

}

int Capsule3D::Draw(const Material2D &color)
{
    return 0;
}

int Capsule3D::SetPosToMoveVec()
{
    VECTOR3D moveVec = this->GetMoveVec();
    if (moveVec != VECTOR3D::GetZero())
    {
        this->mvCenterLineStartPos += moveVec;
        this->mvCenterLineEndPos += moveVec;
        this->SetBasePosUpdateFlag(true);

        this->SetMoveVec(VECTOR3D::GetZero());
    }

    return 0;
}

int Capsule3D::SlideMove(const CollisionCheckResult3D &result, float moveLate)
{
    if (this == result.mpCollisionA)
    {
        VECTOR3D myMoveVec = this->GetMoveVec();
        if (result.mpCollisionB->GetCollisionType() == CollisionType::CollisionType_Cube3D)
        {
            VECTOR3D sideNormVec = (result.mvRepulsionVecA).Normalize();
            if (result.mnResultParam == (signed long)(CubeFaceNumber::CubeFace_Max + 2))
            {
                VECTOR3D newMoveVec = VECTOR3D::GetZero();

                // 自身の移動ベクトルを更新する
                this->SetMoveVec(newMoveVec);
                if (this->GetMyObject() != nullptr)
                {
                    // オブジェクトの移動ベクトルを更新する
                    this->GetMyObject3D()->SetMoveVec3D(newMoveVec);

                    // オブジェクトの当たり判定全ての移動ベクトルを更新する
                    this->GetMyObject()->GetBaseCollisionList()->SetCollisionMoveVec(CollisionDimension_3D, &newMoveVec);
                }
            }
            else if (1.0f < moveLate)
            {
                // 自分じゃない方の当たり判定の移動ベクトルを取得する
                VECTOR3D targetMoveVec = result.mpCollisionB->GetMoveVec();

                // スライドの進行ベクトルを取得
                VECTOR3D slideVec = VECTOR3D::GetSlide(myMoveVec, sideNormVec);

                // 現在の移動の進行ベクトルにスライドに進行ベクトルと、相手の移動ベクトルを加えて、新しい移動の進行ベクトルを取得する
                VECTOR3D newMoveVec = myMoveVec + slideVec + (targetMoveVec * (moveLate - 1.0f));

                // 自身の移動ベクトルを更新する
                this->SetMoveVec(newMoveVec);
                if (this->GetMyObject() != nullptr)
                {
                    // オブジェクトの移動ベクトルを更新する
                    this->GetMyObject3D()->SetMoveVec3D(newMoveVec);

                    // オブジェクトの当たり判定全ての移動ベクトルを更新する
                    this->GetMyObject()->GetBaseCollisionList()->SetCollisionMoveVec(CollisionDimension_3D, &newMoveVec);
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
                    this->GetMyObject3D()->SetMoveVec3D(newMoveVec);

                    // オブジェクトの当たり判定全ての移動ベクトルを更新する
                    this->GetMyObject()->GetBaseCollisionList()->SetCollisionMoveVec(CollisionDimension_3D, &newMoveVec);
                }
            }
        }
    }
    else if (this == result.mpCollisionB)
    {
        VECTOR3D myMoveVec = this->GetMoveVec();
        if (result.mpCollisionA->GetCollisionType() == CollisionType::CollisionType_Cube3D)
        {
            VECTOR3D sideNormVec = result.mvRepulsionVecB.Normalize();
            if (1.0f < moveLate)
            {
                // 自分じゃない方の当たり判定の移動ベクトルを取得する
                VECTOR3D targetMoveVec = result.mpCollisionA->GetMoveVec();

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

int Capsule3D::SetBaseParamMySelf()
{
    // 円の中心座標を更新する必要がある場合のみ処理を行う
    if (this->GetBasePosUpdateFlag())
    {
        // カプセルの中心座標を取得する
        VECTOR3D centerPos = VECTOR3D::GetLerpPos(this->mvCenterLineStartPos, this->mvCenterLineEndPos, 0.5f);
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

int Capsule3D::GetAABB(AABB3D &out)
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
    if (this->mvCenterLineStartPos.GetZ() < this->mvCenterLineEndPos.GetZ())
    {
        out.mvMinPos.SetZ(this->mvCenterLineStartPos.GetZ() - this->mfCenterLineRadiusSqrt);
        out.mvMaxPos.SetZ(this->mvCenterLineEndPos.GetZ() + this->mfCenterLineRadiusSqrt);
    }
    else
    {
        out.mvMinPos.SetZ(this->mvCenterLineEndPos.GetZ() - this->mfCenterLineRadiusSqrt);
        out.mvMaxPos.SetZ(this->mvCenterLineStartPos.GetZ() + this->mfCenterLineRadiusSqrt);
    }
    out.mvMoveVec = this->GetMoveVec();
    return 0;
}

void Capsule3D::SetShapeParameter(const VECTOR3D &startPos, const VECTOR3D &endPos, float radiusNoSqrt)
{
    this->mvCenterLineStartPos = startPos;
    this->mvCenterLineEndPos = endPos;
    this->SetBaseRadiusNoSqrt(radiusNoSqrt);

    // 値の変更があったことを記憶しておく
    this->SetBasePosUpdateFlag(true);
    this->SetBaseRadiusUpdateFlag(true);
}