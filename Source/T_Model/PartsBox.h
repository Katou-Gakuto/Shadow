#pragma once

#include "../Y_Tool/VECTOR.h"
#include "../Y_Tool/MyMatrix.h"
#include "../Z_Except/Resource.h"

struct VERTEX3D
{
    VECTOR3D mvPos;
    VECTOR3D mvNorm;
    float mfU;
    float mfV;
    float mfSU;
    float mfSV;
};

enum BoxFace : unsigned char
{
    BoxFace_Front,
    BoxFace_Up,
    BoxFace_Left,
    BoxFace_Back,
    BoxFace_Down,
    BoxFace_Right,
    BoxFace_Max,
};

class PartsBox
{
private:
    VERTEX3D vertex[24];
    unsigned short index[36];
    TextureHandle mnGraphHandle;

    VECTOR3D mvScalingParam;        // 拡縮行列の各軸の値
    VECTOR3D mvOffSetParam;         // 回転前の原点調整の行列
    VECTOR3D mvRotationParam;       // 回転行列の各軸の値
    VECTOR3D mvTranslationParam;    // 移動行列の各軸の値(Positionの代替メンバ)

    MyMatrix mmWorld;               // ワールド変換行列
    PartsBox *mpParent;             // 自身の親オブジェクト(親がいない場合はnullptr)

public:
    PartsBox();
    ~PartsBox();

    int Transform();

    int Initialize();
    int Finalize();
    int Update();
    int Draw();

    void SetBoxSize(float witdh, float height, float depth);

    void SetScaling(float x, float y, float z);
    void SetScaling(const VECTOR3D &scaling);
    void SetOffSet(float x, float y, float z);
    void SetOffSet(const VECTOR3D &offSet);
    void SetRotation(float x, float y, float z);
    void SetRotation(const VECTOR3D &rotation);
    void SetTranslation(float x, float y, float z);
    void SetTranslation(const VECTOR3D &translation);

    void SetUV(BoxFace face, float uStart, float vStart, float uEnd, float vEnd);

    void SetGraphHandle(TextureHandle handle);
    void SetParent(PartsBox *parent);
    VECTOR3D GetScaling();
    VECTOR3D GetOffSet();
    VECTOR3D GetRotation();
    VECTOR3D GetTranslation();
};