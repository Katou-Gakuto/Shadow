#include "Cube3D.h"

#include "Triangle3D.h"
#include "Quadrangle3D.h"
#include "../Z_Except/Master.h"
#include "../V_Display/System.h"

typedef InputCubeVertexNumber ICVN;

// 頂点情報出力時のCubeの各面の頂点番号(OutputCubeVertexNumberの略)
enum Cube3D::OCVN : unsigned char
{
    // 内向きCube
    // 左面
    OCVN_In_LeftFace_LeftUp = 0,
    OCVN_In_LeftFace_RightUp = 4,
    OCVN_In_LeftFace_LeftBottom = 2,
    OCVN_In_LeftFace_RightBottom = 6,
    // 右面
    OCVN_In_RightFace_LeftUp = 5,
    OCVN_In_RightFace_RightUp = 1,
    OCVN_In_RightFace_LeftBottom = 7,
    OCVN_In_RightFace_RightBottom = 3,
    // 天井
    OCVN_In_UpFace_LeftUp = 5,
    OCVN_In_UpFace_RightUp = 4,
    OCVN_In_UpFace_LeftBottom = 1,
    OCVN_In_UpFace_RightBottom = 0,
    // 底面
    OCVN_In_DownFace_LeftUp = 3,
    OCVN_In_DownFace_RightUp = 2,
    OCVN_In_DownFace_LeftBottom = 7,
    OCVN_In_DownFace_RightBottom = 6,
    // 正面
    OCVN_In_FrontFace_LeftUp = 1,
    OCVN_In_FrontFace_RightUp = 0,
    OCVN_In_FrontFace_LeftBottom = 3,
    OCVN_In_FrontFace_RightBottom = 2,
    // 裏面
    OCVN_In_BackFace_LeftUp = 4,
    OCVN_In_BackFace_RightUp = 5,
    OCVN_In_BackFace_LeftBottom = 6,
    OCVN_In_BackFace_RightBottom = 7,

    // 外向きCube
    // 左面
    OCVN_Out_LeftFace_LeftUp = 4,
    OCVN_Out_LeftFace_RightUp = 0,
    OCVN_Out_LeftFace_LeftBottom = 6,
    OCVN_Out_LeftFace_RightBottom = 2,
    // 右面
    OCVN_Out_RightFace_LeftUp = 1,
    OCVN_Out_RightFace_RightUp = 5,
    OCVN_Out_RightFace_LeftBottom = 3,
    OCVN_Out_RightFace_RightBottom = 7,
    // 天井
    OCVN_Out_UpFace_LeftUp = 4,
    OCVN_Out_UpFace_RightUp = 5,
    OCVN_Out_UpFace_LeftBottom = 0,
    OCVN_Out_UpFace_RightBottom = 1,
    // 底面
    OCVN_Out_DownFace_LeftUp = 2,
    OCVN_Out_DownFace_RightUp = 3,
    OCVN_Out_DownFace_LeftBottom = 6,
    OCVN_Out_DownFace_RightBottom = 7,
    // 正面
    OCVN_Out_FrontFace_LeftUp = 0,
    OCVN_Out_FrontFace_RightUp = 1,
    OCVN_Out_FrontFace_LeftBottom = 2,
    OCVN_Out_FrontFace_RightBottom = 3,
    // 裏面
    OCVN_Out_BackFace_LeftUp = 5,
    OCVN_Out_BackFace_RightUp = 4,
    OCVN_Out_BackFace_LeftBottom = 7,
    OCVN_Out_BackFace_RightBottom = 6,
};

int Cube3D::GetFaceQuad(Quadrangle3D *dest, CubeFaceNumber face, CollisionNorm normNum, bool unconditionalGetFlag) const
{
    if (dest == nullptr)
    {
        return -1;
    }
    if (face == CubeFaceNumber::CubeFace_Max)
    {
        return -1;
    }
    if ((normNum == CollisionNorm::CollisionNorm_In || normNum == CollisionNorm::CollisionNorm_Out) == false)
    {
        return -1;
    }
    if (unconditionalGetFlag == false)
    {
        if (this->GetCollisionNorm() != normNum)
        {
            return -1;
        }
        if (this->mbProcessingQuadFlag.CheckActive() == false ||
            this->mbProcessingQuadFlag.CheckFlag((unsigned long)((unsigned char)(face))) == false)
        {
            return -1;
        }
    }
    if (normNum == CollisionNorm::CollisionNorm_In)
    {
        switch (face)
        {
        case CubeFaceNumber::CubeFace_Left:
            // 左
            dest->SetShapeParameter(
                this->mvVertexPos[OCVN::OCVN_In_LeftFace_LeftUp],
                this->mvVertexPos[OCVN::OCVN_In_LeftFace_RightUp],
                this->mvVertexPos[OCVN::OCVN_In_LeftFace_LeftBottom],
                this->mvVertexPos[OCVN::OCVN_In_LeftFace_RightBottom]);
            break;
        case CubeFaceNumber::CubeFace_Right:
            // 右
            dest->SetShapeParameter(
                this->mvVertexPos[OCVN::OCVN_In_RightFace_LeftUp],
                this->mvVertexPos[OCVN::OCVN_In_RightFace_RightUp],
                this->mvVertexPos[OCVN::OCVN_In_RightFace_LeftBottom],
                this->mvVertexPos[OCVN::OCVN_In_RightFace_RightBottom]);
            break;
        case CubeFaceNumber::CubeFace_Up:
            // 天井
            dest->SetShapeParameter(
                this->mvVertexPos[OCVN::OCVN_In_UpFace_LeftUp],
                this->mvVertexPos[OCVN::OCVN_In_UpFace_RightUp],
                this->mvVertexPos[OCVN::OCVN_In_UpFace_LeftBottom],
                this->mvVertexPos[OCVN::OCVN_In_UpFace_RightBottom]);
            break;
        case CubeFaceNumber::CubeFace_Down:
            // 底
            dest->SetShapeParameter(
                this->mvVertexPos[OCVN::OCVN_In_DownFace_LeftUp],
                this->mvVertexPos[OCVN::OCVN_In_DownFace_RightUp],
                this->mvVertexPos[OCVN::OCVN_In_DownFace_LeftBottom],
                this->mvVertexPos[OCVN::OCVN_In_DownFace_RightBottom]);
            break;
        case CubeFaceNumber::CubeFace_Front:
            // 正面
            dest->SetShapeParameter(
                this->mvVertexPos[OCVN::OCVN_In_FrontFace_LeftUp],
                this->mvVertexPos[OCVN::OCVN_In_FrontFace_RightUp],
                this->mvVertexPos[OCVN::OCVN_In_FrontFace_LeftBottom],
                this->mvVertexPos[OCVN::OCVN_In_FrontFace_RightBottom]);
            break;
        case CubeFaceNumber::CubeFace_Back:
            // 裏
            dest->SetShapeParameter(
                this->mvVertexPos[OCVN::OCVN_In_BackFace_LeftUp],
                this->mvVertexPos[OCVN::OCVN_In_BackFace_RightUp],
                this->mvVertexPos[OCVN::OCVN_In_BackFace_LeftBottom],
                this->mvVertexPos[OCVN::OCVN_In_BackFace_RightBottom]);
            break;
        default:
            break;
        }
    }
    else
    {
        switch (face)
        {
        case CubeFaceNumber::CubeFace_Left:
            // 左
            dest->SetShapeParameter(
                this->mvVertexPos[OCVN::OCVN_Out_LeftFace_LeftUp],
                this->mvVertexPos[OCVN::OCVN_Out_LeftFace_RightUp],
                this->mvVertexPos[OCVN::OCVN_Out_LeftFace_LeftBottom],
                this->mvVertexPos[OCVN::OCVN_Out_LeftFace_RightBottom]);
            break;
        case CubeFaceNumber::CubeFace_Right:
            // 右
            dest->SetShapeParameter(
                this->mvVertexPos[OCVN::OCVN_Out_RightFace_LeftUp],
                this->mvVertexPos[OCVN::OCVN_Out_RightFace_RightUp],
                this->mvVertexPos[OCVN::OCVN_Out_RightFace_LeftBottom],
                this->mvVertexPos[OCVN::OCVN_Out_RightFace_RightBottom]);
            break;
        case CubeFaceNumber::CubeFace_Up:
            // 天井
            dest->SetShapeParameter(
                this->mvVertexPos[OCVN::OCVN_Out_UpFace_LeftUp],
                this->mvVertexPos[OCVN::OCVN_Out_UpFace_RightUp],
                this->mvVertexPos[OCVN::OCVN_Out_UpFace_LeftBottom],
                this->mvVertexPos[OCVN::OCVN_Out_UpFace_RightBottom]);
            break;
        case CubeFaceNumber::CubeFace_Down:
            // 底
            dest->SetShapeParameter(
                this->mvVertexPos[OCVN::OCVN_Out_DownFace_LeftUp],
                this->mvVertexPos[OCVN::OCVN_Out_DownFace_RightUp],
                this->mvVertexPos[OCVN::OCVN_Out_DownFace_LeftBottom],
                this->mvVertexPos[OCVN::OCVN_Out_DownFace_RightBottom]);
            break;
        case CubeFaceNumber::CubeFace_Front:
            // 正面
            dest->SetShapeParameter(
                this->mvVertexPos[OCVN::OCVN_Out_FrontFace_LeftUp],
                this->mvVertexPos[OCVN::OCVN_Out_FrontFace_RightUp],
                this->mvVertexPos[OCVN::OCVN_Out_FrontFace_LeftBottom],
                this->mvVertexPos[OCVN::OCVN_Out_FrontFace_RightBottom]);
            break;
        case CubeFaceNumber::CubeFace_Back:
            // 裏
            dest->SetShapeParameter(
                this->mvVertexPos[OCVN::OCVN_Out_BackFace_LeftUp],
                this->mvVertexPos[OCVN::OCVN_Out_BackFace_RightUp],
                this->mvVertexPos[OCVN::OCVN_Out_BackFace_LeftBottom],
                this->mvVertexPos[OCVN::OCVN_Out_BackFace_RightBottom]);
            break;
        default:
            break;
        }
    }
 
    dest->SetMoveVec(this->GetMoveVec());
    return 0;
}

Cube3D::Cube3D() :
    mvVertexPos(),
    mbProcessingQuadFlag(CubeFaceNumber::CubeFace_Max),
    BaseCollision3D(nullptr, CollisionType::CollisionType_Cube3D, CollisionTag_Checker, CollisionNorm_Out, true, false, 0.0f)
{

}

Cube3D::Cube3D(
    const VECTOR3D *vertexPosBox,
    GameObject *object, CollisionTag tag, CollisionNorm normNum,
    bool penetrate, bool hitMove, float moveLate) :
    mvVertexPos(),
    mbProcessingQuadFlag(CubeFaceNumber::CubeFace_Max),
    BaseCollision3D(object, CollisionType::CollisionType_Cube3D, tag, normNum, penetrate, hitMove, moveLate)
{
    this->SetShapeParameter(vertexPosBox);
}

Cube3D::Cube3D(
    const VECTOR3D &forntFaceLeftUp,
    const VECTOR3D &forntFaceLeftBottom,
    const VECTOR3D &forntFaceRightUp,
    const VECTOR3D &forntFaceRightBottom,
    const VECTOR3D &backFaceLeftUp,
    const VECTOR3D &backFaceLeftBottom,
    const VECTOR3D &backFaceRightUp,
    const VECTOR3D &backFaceRightBottom,
    CollisionNorm normNum,
    GameObject *object, CollisionTag tag,
    bool penetrate, bool hitMove, float moveLate) :
    mvVertexPos(),
    mbProcessingQuadFlag(CubeFaceNumber::CubeFace_Max),
    BaseCollision3D(object, CollisionType::CollisionType_Cube3D, tag, normNum, penetrate, hitMove, moveLate)
{
    this->SetShapeParameter(
        forntFaceLeftUp,
        forntFaceLeftBottom,
        forntFaceRightUp,
        forntFaceRightBottom,
        backFaceLeftUp,
        backFaceLeftBottom,
        backFaceRightUp,
        backFaceRightBottom);
}

Cube3D::~Cube3D()
{

}

int Cube3D::Draw(const Material2D &color)
{
    // 作業用変数
    VECTOR3D startPos;
    VECTOR3D endPos;

    // 描画が可能になったら処理を行う
    if (Master::mpSystem->ChangeShaderMode(ShaderMode::ShaderMode_Normal_3DLine) == 0)
    {
        // 前面
        startPos = this->mvVertexPos[OCVN::OCVN_Out_FrontFace_LeftUp];
        endPos = this->mvVertexPos[OCVN::OCVN_Out_FrontFace_RightUp];
        Master::mpSystem->DrawLineLight3D(startPos, endPos, color, 1.0f, 20.0f);
        startPos = this->mvVertexPos[OCVN::OCVN_Out_FrontFace_RightUp];
        endPos = this->mvVertexPos[OCVN::OCVN_Out_FrontFace_RightBottom];
        Master::mpSystem->DrawLineLight3D(startPos, endPos, color, 1.0f, 20.0f);
        startPos = this->mvVertexPos[OCVN::OCVN_Out_FrontFace_RightBottom];
        endPos = this->mvVertexPos[OCVN::OCVN_Out_FrontFace_LeftBottom];
        Master::mpSystem->DrawLineLight3D(startPos, endPos, color, 1.0f, 20.0f);
        startPos = this->mvVertexPos[OCVN::OCVN_Out_FrontFace_LeftBottom];
        endPos = this->mvVertexPos[OCVN::OCVN_Out_FrontFace_LeftUp];
        Master::mpSystem->DrawLineLight3D(startPos, endPos, color, 1.0f, 20.0f);

        // 背面
        startPos = this->mvVertexPos[OCVN::OCVN_Out_BackFace_LeftUp];
        endPos = this->mvVertexPos[OCVN::OCVN_Out_BackFace_RightUp];
        Master::mpSystem->DrawLineLight3D(startPos, endPos, color, 1.0f, 20.0f);
        startPos = this->mvVertexPos[OCVN::OCVN_Out_BackFace_RightUp];
        endPos = this->mvVertexPos[OCVN::OCVN_Out_BackFace_RightBottom];
        Master::mpSystem->DrawLineLight3D(startPos, endPos, color, 1.0f, 20.0f);
        startPos = this->mvVertexPos[OCVN::OCVN_Out_BackFace_RightBottom];
        endPos = this->mvVertexPos[OCVN::OCVN_Out_BackFace_LeftBottom];
        Master::mpSystem->DrawLineLight3D(startPos, endPos, color, 1.0f, 20.0f);
        startPos = this->mvVertexPos[OCVN::OCVN_Out_BackFace_LeftBottom];
        endPos = this->mvVertexPos[OCVN::OCVN_Out_BackFace_LeftUp];
        Master::mpSystem->DrawLineLight3D(startPos, endPos, color, 1.0f, 20.0f);

        // 前面と背面を繋げる部分
        startPos = this->mvVertexPos[OCVN::OCVN_Out_FrontFace_LeftUp];
        endPos = this->mvVertexPos[OCVN::OCVN_Out_BackFace_RightUp];
        Master::mpSystem->DrawLineLight3D(startPos, endPos, color, 1.0f, 20.0f);
        startPos = this->mvVertexPos[OCVN::OCVN_Out_FrontFace_RightUp];
        endPos = this->mvVertexPos[OCVN::OCVN_Out_BackFace_LeftUp];
        Master::mpSystem->DrawLineLight3D(startPos, endPos, color, 1.0f, 20.0f);
        startPos = this->mvVertexPos[OCVN::OCVN_Out_FrontFace_LeftBottom];
        endPos = this->mvVertexPos[OCVN::OCVN_Out_BackFace_RightBottom];
        Master::mpSystem->DrawLineLight3D(startPos, endPos, color, 1.0f, 20.0f);
        startPos = this->mvVertexPos[OCVN::OCVN_Out_FrontFace_RightBottom];
        endPos = this->mvVertexPos[OCVN::OCVN_Out_BackFace_LeftBottom];
        Master::mpSystem->DrawLineLight3D(startPos, endPos, color, 1.0f, 20.0f);
    }
    return 0;
}

int Cube3D::SetPosToMoveVec()
{
    VECTOR3D moveVec = this->GetMoveVec();
    if (moveVec != VECTOR3D::GetZero())
    {
        for (int i = 0; i < 8; i++)
        {
            this->mvVertexPos[i] += moveVec;
        }

        this->SetBasePosUpdateFlag(true);

        this->SetMoveVec(VECTOR3D::GetZero());
    }

    return 0;
}

int Cube3D::SlideMove(const CollisionCheckResult3D &result, float moveLate)
{
    return 0;
}

int Cube3D::SetBaseParamMySelf()
{
    if (this->GetBasePosUpdateFlag() || this->GetBaseRadiusUpdateFlag())
    {
        // 真上から見て手前側のやつを0123、奥側のやつを4567としてます。
        VECTOR3D maxStart;
        VECTOR3D maxEnd;
        float maxMagNoSqrt;
        VECTOR3D nowStart;
        VECTOR3D nowEnd;
        float nowMagNoSqrt;

        for (unsigned char i = 0; i < (unsigned char)(InputCubeVertexNumber::CubeVertex_Max); i++)
        {
            for (unsigned char j = i + 1; j < (unsigned char)(InputCubeVertexNumber::CubeVertex_Max); j++)
            {
                nowStart = this->mvVertexPos[i];
                nowEnd = this->mvVertexPos[j];
                nowMagNoSqrt = (nowEnd - nowStart).MagnitudeNoSqrt();

                if (i == 0 && j == 1)
                {
                    maxStart = nowStart;
                    maxEnd = nowEnd;
                    maxMagNoSqrt = nowMagNoSqrt;
                }
                else
                {
                    if (maxMagNoSqrt < nowMagNoSqrt)
                    {
                        maxStart = nowStart;
                        maxEnd = nowEnd;
                        maxMagNoSqrt = nowMagNoSqrt;
                    }
                }
            }
        }

        VECTOR3D basePos = VECTOR3D::GetLerpPos(maxStart, maxEnd, 0.5f);
        float baseRadiusNoSqrt = maxMagNoSqrt * 0.5f;
        this->SetBasePos(basePos);
        this->SetBasePosUpdateFlag(false);
        this->SetBaseRadiusNoSqrt(baseRadiusNoSqrt);
        this->SetBaseRadiusUpdateFlag(false);
    }

    return 0;
}

int Cube3D::GetAABB(AABB3D &out)
{
    float minX = this->mvVertexPos[0].GetX();
    float minY = this->mvVertexPos[0].GetY();
    float minZ = this->mvVertexPos[0].GetZ();
    float maxX = this->mvVertexPos[0].GetX();
    float maxY = this->mvVertexPos[0].GetY();
    float maxZ = this->mvVertexPos[0].GetZ();
    for (unsigned char i = 1; i < (unsigned char)(InputCubeVertexNumber::CubeVertex_Max); i++)
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


void Cube3D::SetShapeParameter(const VECTOR3D *vertexBox)
{
    this->mvVertexPos[ICVN::ICVN_FrontFace_LeftUp] = vertexBox[ICVN::ICVN_FrontFace_LeftUp];
    this->mvVertexPos[ICVN::ICVN_FrontFace_RightUp] = vertexBox[ICVN::ICVN_FrontFace_RightUp];
    this->mvVertexPos[ICVN::ICVN_FrontFace_LeftBottom] = vertexBox[ICVN::ICVN_FrontFace_LeftBottom];
    this->mvVertexPos[ICVN::ICVN_FrontFace_RightBottom] = vertexBox[ICVN::ICVN_FrontFace_RightBottom];
    this->mvVertexPos[ICVN::ICVN_BackFace_LeftUp] = vertexBox[ICVN::ICVN_BackFace_LeftUp];
    this->mvVertexPos[ICVN::ICVN_BackFace_RightUp] = vertexBox[ICVN::ICVN_BackFace_RightUp];
    this->mvVertexPos[ICVN::ICVN_BackFace_LeftBottom] = vertexBox[ICVN::ICVN_BackFace_LeftBottom];
    this->mvVertexPos[ICVN::ICVN_BackFace_RightBottom] = vertexBox[ICVN::ICVN_BackFace_RightBottom];

    this->SetBasePosUpdateFlag(true);
    this->SetBaseRadiusUpdateFlag(true);
}

void Cube3D::SetShapeParameter(
    const VECTOR3D &forntFaceLeftUp,
    const VECTOR3D &forntFaceLeftBottom,
    const VECTOR3D &forntFaceRightUp,
    const VECTOR3D &forntFaceRightBottom,
    const VECTOR3D &backFaceLeftUp,
    const VECTOR3D &backFaceLeftBottom,
    const VECTOR3D &backFaceRightUp,
    const VECTOR3D &backFaceRightBottom)
{
    this->mvVertexPos[ICVN::ICVN_FrontFace_LeftUp]      = forntFaceRightUp;
    this->mvVertexPos[ICVN::ICVN_FrontFace_RightUp]     = forntFaceLeftUp;
    this->mvVertexPos[ICVN::ICVN_FrontFace_LeftBottom]  = forntFaceRightBottom;
    this->mvVertexPos[ICVN::ICVN_FrontFace_RightBottom] = forntFaceLeftBottom;
    this->mvVertexPos[ICVN::ICVN_BackFace_LeftUp]       = backFaceRightUp;
    this->mvVertexPos[ICVN::ICVN_BackFace_RightUp]      = backFaceLeftUp;
    this->mvVertexPos[ICVN::ICVN_BackFace_LeftBottom]   = backFaceRightBottom;
    this->mvVertexPos[ICVN::ICVN_BackFace_RightBottom]  = backFaceLeftBottom;
    this->SetBasePosUpdateFlag(true);
    this->SetBaseRadiusUpdateFlag(true);
}

int Cube3D::AddProcessingFace(CubeFaceNumber face)
{
    return this->mbProcessingQuadFlag.OnFlag((unsigned long)((unsigned char)(face)));
}

int Cube3D::DeleteProcessingFace(CubeFaceNumber face)
{
    return this->mbProcessingQuadFlag.OffFlag((unsigned long)((unsigned char)(face)));
}

bool Cube3D::CheckInPoint(const VECTOR3D &pointNowPos, const VECTOR3D &pointMoveVec, VECTOR3D &pointUseNorm)
{
    this->SetBaseParamMySelf();
    VECTOR3D pointNextPos = (pointNowPos + pointMoveVec);
    VECTOR3D cubeBase2point = pointNextPos - this->GetMovedPos(this->GetBasePos());
    if (this->GetBaseRadiusNoSqrt() < cubeBase2point.MagnitudeNoSqrt())
    {
        return false;
    }

    Quadrangle3D thisQuad;
    CubeFaceNumber currentFace = CubeFace_Max;
    int result = -1;
    bool returnValue = true;
    VECTOR3D normBox[6];
    VECTOR3D onPlanePos;
    VECTOR3D quadBase2point;
    int quadBase2pointMaxNumber;
    float distance = 0.0f;
    float distanceMin = 0.0f;
    for (int i = 0; i < CubeFace_Max; i++)
    {
        currentFace = Cube3D::Int2CFN(i);
        result = this->GetFaceQuad(&thisQuad, currentFace, CollisionNorm::CollisionNorm_In, true);
        if (result != 0)
        {
            continue;
        }
        normBox[i] = thisQuad.GetNormVec();
        thisQuad.SetBaseParamMySelf();
        quadBase2point = pointNextPos - thisQuad.GetMovedPos(thisQuad.GetBasePos());

        // 『壁から目標地点までのベクトルが、壁(強制的に内向き)の法線ベクトルの逆ベクトルに近い』
        // という場合は目標地点は壁の法線方向、つまりキューブの外にあるということ
        if (-normBox[i] == VECTOR3D::GetTargetDirectionNewrVec(quadBase2point, normBox[i], -normBox[i]))
        {
            returnValue = false;
            break;
        }
        if (this->mbProcessingQuadFlag.CheckFlag(currentFace))
        {
            onPlanePos = thisQuad.GetMovedPos(thisQuad.GetVertexPos(0));

            // 四角形の面から現在のpointまでの最短距離を取得する
            distance = VECTOR3D::GetPointToPlaneDistance(
                pointNowPos,
                onPlanePos,
                normBox[i]);
            if (i != 0)
            {
                if (distanceMin <= distance)
                {
                    continue;
                }
            }
            quadBase2pointMaxNumber = i;
            distanceMin = distance;
        }
    }
    if (returnValue)
    {
        pointUseNorm = normBox[quadBase2pointMaxNumber];
    }

    return returnValue;
}

int Cube3D::GetFaceQuad(Quadrangle3D *dest, CubeFaceNumber face, CollisionNorm normNum) const
{
    return this->GetFaceQuad(dest, face, normNum, false);
}

VECTOR3D Cube3D::GetVertexPos(InputCubeVertexNumber targetVertex) const
{
    VECTOR3D temp;
    if (targetVertex != InputCubeVertexNumber::CubeVertex_Max)
    {
        temp = this->mvVertexPos[(unsigned char)(targetVertex)];
    }
    return temp;
}

VECTOR3D Cube3D::GetVertexPos(unsigned char index) const
{
    VECTOR3D temp;
    if (0 <= index && index < InputCubeVertexNumber::CubeVertex_Max)
    {
        temp = this->mvVertexPos[index];
    }
    return temp;
}

CubeFaceNumber Cube3D::Int2CFN(int num)
{
    CubeFaceNumber temp = CubeFaceNumber::CubeFace_Max;
    switch (num)
    {
    case 0:
        temp = CubeFace_Left;
        break;
    case 1:
        temp = CubeFace_Right;
        break;
    case 2:
        temp = CubeFace_Up;
        break;
    case 3:
        temp = CubeFace_Down;
        break;
    case 4:
        temp = CubeFace_Front;
        break;
    case 5:
        temp = CubeFace_Back;
        break;
    default:
        break;
    }
    return temp;
}
