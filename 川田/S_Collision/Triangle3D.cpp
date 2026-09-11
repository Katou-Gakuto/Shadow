#include "Triangle3D.h"

#include "../V_Display/System.h"
#include "../Z_Except/Master.h"

Triangle3D::Triangle3D() :
    mvVertexPos(),
    BaseCollision3D(nullptr, CollisionType::CollisionType_Triangle3D, CollisionTag_Checker, CollisionNorm_Out, true, false, 0.0f)
{
    for (int i = 0; i < 3; i++)
    {
        this->mvVertexPos[i] = VECTOR3D::GetZero();
    }
}

Triangle3D::Triangle3D(
    const VECTOR3D &pos0, const VECTOR3D &pos1, const VECTOR3D &pos2,
    GameObject *myObject, CollisionTag tag, CollisionNorm normNum,
    bool penetrate, bool hitMove, float moveLate) :
    mvVertexPos(),
    BaseCollision3D(myObject, CollisionType::CollisionType_Triangle3D, tag, normNum, penetrate, hitMove, moveLate)
{
    // 頂点の座標を設定する
    this->mvVertexPos[0] = pos0;
    this->mvVertexPos[1] = pos1;
    this->mvVertexPos[2] = pos2;

    // BaseParamを設定する
    this->SetBaseParamMySelf();
}

Triangle3D::~Triangle3D()
{

}

int Triangle3D::Draw(const Material2D &color)
{
    // 作業用変数
    VECTOR3D startPos;
    VECTOR3D endPos;

    // 描画が可能になったら処理を行う
    if (Master::mpSystem->ChangeShaderMode(ShaderMode::ShaderMode_Normal_3DLine) == 0)
    {
        // 前面
        startPos = this->mvVertexPos[0];
        endPos = this->mvVertexPos[1];
        Master::mpSystem->DrawLine(startPos, endPos, color);
        startPos = this->mvVertexPos[1];
        endPos = this->mvVertexPos[2];
        Master::mpSystem->DrawLine(startPos, endPos, color);
        startPos = this->mvVertexPos[2];
        endPos = this->mvVertexPos[0];
        Master::mpSystem->DrawLine(startPos, endPos, color);
    }

    return 0;
}

int Triangle3D::SetPosToMoveVec()
{
    VECTOR3D moveVec = this->GetMoveVec();
    if (moveVec != VECTOR3D::GetZero())
    {
        this->mvVertexPos[0] = (this->mvVertexPos[0] + moveVec);
        this->mvVertexPos[1] = (this->mvVertexPos[1] + moveVec);
        this->mvVertexPos[2] = (this->mvVertexPos[2] + moveVec);
        this->SetBasePosUpdateFlag(true);

        this->SetMoveVec(VECTOR3D::GetZero());
    }

    return 0;
}

int Triangle3D::SlideMove(const CollisionCheckResult3D &result, float moveLate)
{
    return 0;
}

int Triangle3D::SetBaseParamMySelf()
{
    // 値が変わっている可能性がある場合のみ処理を行う
    if (this->GetBasePosUpdateFlag() || this->GetBaseRadiusUpdateFlag())
    {
        VECTOR3D basePos = VECTOR3D::GetZero();
        VECTOR3D maxDistance = this->mvVertexPos[1] - this->mvVertexPos[0];
        float maxMagNoSqrt = maxDistance.MagnitudeNoSqrt();

        VECTOR3D currentDistance = VECTOR3D::GetZero();
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
                    basePos = VECTOR3D::GetLerpPos((this->mvVertexPos[i]), (this->mvVertexPos[j]), 0.5f);
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

int Triangle3D::GetAABB(AABB3D &out)
{
    float minX = this->mvVertexPos[0].GetX();
    float minY = this->mvVertexPos[0].GetY();
    float minZ = this->mvVertexPos[0].GetZ();
    float maxX = this->mvVertexPos[0].GetX();
    float maxY = this->mvVertexPos[0].GetY();
    float maxZ = this->mvVertexPos[0].GetZ();
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


void Triangle3D::SetShapeParameter(const VECTOR3D &pos0, const VECTOR3D &pos1, const VECTOR3D &pos2)
{
    // 頂点の座標を設定する
    this->mvVertexPos[0] = pos0;
    this->mvVertexPos[1] = pos1;
    this->mvVertexPos[2] = pos2;

    // 値の変更があったことを記憶しておく
    this->SetBasePosUpdateFlag(true);
    this->SetBaseRadiusUpdateFlag(true);
}

VECTOR3D Triangle3D::GetVertexPos(int index)
{
    if (0 <= index && index < 3)
    {
        return this->mvVertexPos[index];
    }
    else
    {
        return VECTOR3D::GetZero();
    }
}

// この三角形の法線ベクトルを返す関数
VECTOR3D Triangle3D::GetNormVec()
{
    return VECTOR3D::Cross((this->mvVertexPos[1] - this->mvVertexPos[0]), (this->mvVertexPos[2] - this->mvVertexPos[0])).Normalize();
}
