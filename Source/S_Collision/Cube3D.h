#pragma once

#include "BaseCollision3D.h"

// キューブの各面の番号
enum CubeFaceNumber : unsigned char
{
    CubeFace_Left = 0,
    CubeFace_Right,
    CubeFace_Up,
    CubeFace_Down,
    CubeFace_Front,
    CubeFace_Back,
    CubeFace_Max,
};

// キューブの頂点情報入力時のCubeの各面の頂点番号
// このキューブは回転が全くない状態で『FrontFace_LeftUp = VECTOR3D(-1.0f, 1.0f, -1.0f)』の考えの元作られています
// 入力する際もこの考えを基準に入力してください
typedef enum InputCubeVertexNumber : unsigned char
{
    // 左面
    ICVN_LeftFace_LeftUp = 4,
    ICVN_LeftFace_RightUp = 0,
    ICVN_LeftFace_LeftBottom = 6,
    ICVN_LeftFace_RightBottom = 2,
    // 右面
    ICVN_RightFace_LeftUp = 1,
    ICVN_RightFace_RightUp = 5,
    ICVN_RightFace_LeftBottom = 3,
    ICVN_RightFace_RightBottom = 7,
    // 天井
    ICVN_UpFace_LeftUp = 4,
    ICVN_UpFace_RightUp = 5,
    ICVN_UpFace_LeftBottom = 0,
    ICVN_UpFace_RightBottom = 1,
    // 底面
    ICVN_DownFace_LeftUp = 2,
    ICVN_DownFace_RightUp = 3,
    ICVN_DownFace_LeftBottom = 6,
    ICVN_DownFace_RightBottom = 7,
    // 正面
    ICVN_FrontFace_LeftUp = 0,
    ICVN_FrontFace_RightUp = 1,
    ICVN_FrontFace_LeftBottom = 2,
    ICVN_FrontFace_RightBottom = 3,
    // 裏面
    ICVN_BackFace_LeftUp = 5,
    ICVN_BackFace_RightUp = 4,
    ICVN_BackFace_LeftBottom = 7,
    ICVN_BackFace_RightBottom = 6,

    // Cube3Dの頂点数
    CubeVertex_Max = 8,
};

class Quadrangle3D;

// 最大六面の3D空間の四角形をまとめて計算できるようにしたクラス
class Cube3D : public BaseCollision3D
{
private:
    VECTOR3D mvVertexPos[InputCubeVertexNumber::CubeVertex_Max];
    BitFlag mbProcessingQuadFlag;

    // 頂点情報出力時のCubeの各面の頂点番号(OutputCubeVertexNumberの略)
    enum OCVN : unsigned char;

    int GetFaceQuad(Quadrangle3D *dest, CubeFaceNumber face, CollisionNorm normNum, bool unconditionalGetFlag) const;

public:
    Cube3D();
    Cube3D(
        const VECTOR3D *vertexPosBox,
        GameObject *object, CollisionTag tag, CollisionNorm normNum,
        bool penetrate, bool hitMove, float moveLate);
    Cube3D(
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
        bool penetrate, bool hitMove, float moveLate);
    ~Cube3D() override;

    int Draw(const Material2D &color) override;
    int SetPosToMoveVec() override;
    int SlideMove(const CollisionCheckResult3D &result, float moveLate) override;
    int SetBaseParamMySelf() override;
    int GetAABB(AABB3D &out) override;

    void SetShapeParameter(const VECTOR3D *vertexBox);
    void SetShapeParameter(
        const VECTOR3D &forntFaceLeftUp,
        const VECTOR3D &forntFaceLeftBottom,
        const VECTOR3D &forntFaceRightUp,
        const VECTOR3D &forntFaceRightBottom,
        const VECTOR3D &backFaceLeftUp,
        const VECTOR3D &backFaceLeftBottom,
        const VECTOR3D &backFaceRightUp,
        const VECTOR3D &backFaceRightBottom);

    int AddProcessingFace(CubeFaceNumber face);
    int DeleteProcessingFace(CubeFaceNumber face);
    bool CheckInPoint(const VECTOR3D &pointNowPos, const VECTOR3D &pointMoveVec, VECTOR3D &pointUseNorm);

    int GetFaceQuad(Quadrangle3D *dest, CubeFaceNumber face, CollisionNorm normNum) const;
    VECTOR3D GetVertexPos(InputCubeVertexNumber targetVertex) const;
    VECTOR3D GetVertexPos(unsigned char index) const;

    static CubeFaceNumber Int2CFN(int num);
};