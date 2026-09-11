#include "PartsBox.h"

PartsBox::PartsBox() :
    mmWorld(4, 4)
{

}

PartsBox::~PartsBox()
{

}

int PartsBox::Transform()
{
    // 自身のワールド変換行列を更新する
    this->mmWorld = MyMatrix::GetIdentity(4);
    MyMatrix scale = MyMatrix::GetScaling3D(this->mvScalingParam);
    MyMatrix offSet = MyMatrix::GetTransportion3D(this->mvOffSetParam);
    MyMatrix rotateY = MyMatrix::GetRotation3DY(this->mvRotationParam.GetY());
    MyMatrix rotateX = MyMatrix::GetRotation3DX(this->mvRotationParam.GetX());
    MyMatrix rotateZ = MyMatrix::GetRotation3DZ(this->mvRotationParam.GetZ());
    MyMatrix translation = MyMatrix::GetTransportion3D(this->mvTranslationParam);

    this->mmWorld = (((((scale * offSet) * rotateY) * rotateX) * rotateZ) * translation);

    // 行列の親子関係を設定する
    if (this->mpParent != nullptr)
    {
        this->mmWorld *= this->mpParent->mmWorld;
    }

    return 0;
}

int PartsBox::Initialize()
{
    return 0;
}

int PartsBox::Finalize()
{
    return 0;
}

int PartsBox::Update()
{
    return 0;
}

int PartsBox::Draw()
{
    return 0;
}

void PartsBox::SetBoxSize(float witdh, float height, float depth)
{
    witdh /= 2;
    height /= 2;
    depth /= 2;

    // 正面
    // 左上
    vertex[0].mvPos = VECTOR3D(-witdh, height, -depth);
    vertex[0].mvNorm = VECTOR3D(0.0f, 0.0f, -1.0f);
    vertex[0].mfU = 0.0f;
    vertex[0].mfV = 0.0f;
    vertex[0].mfSU = 0.0f;
    vertex[0].mfSV = 0.0f;
    // 右上
    vertex[1].mvPos = VECTOR3D(witdh, height, -depth);
    vertex[1].mvNorm = VECTOR3D(0.0f, 0.0f, -1.0f);
    vertex[1].mfU = 1.0f;
    vertex[1].mfV = 0.0f;
    vertex[1].mfSU = 0.0f;
    vertex[1].mfSV = 0.0f;
    // 左下
    vertex[2].mvPos = VECTOR3D(-witdh, -height, -depth);
    vertex[2].mvNorm = VECTOR3D(0.0f, 0.0f, -1.0f);
    vertex[2].mfU = 0.0f;
    vertex[2].mfV = 1.0f;
    vertex[2].mfSU = 0.0f;
    vertex[2].mfSV = 0.0f;
    // 右下
    vertex[3].mvPos = VECTOR3D(witdh, -height, -depth);
    vertex[3].mvNorm = VECTOR3D(0.0f, 0.0f, -1.0f);
    vertex[3].mfU = 1.0f;
    vertex[3].mfV = 1.0f;
    vertex[3].mfSU = 0.0f;
    vertex[3].mfSV = 0.0f;

    // 天井
    // 左上
    vertex[4].mvPos = VECTOR3D(-witdh, height, depth);
    vertex[4].mvNorm = VECTOR3D(0.0f, 1.0f, 0.0f);
    vertex[4].mfU = 0.0f;
    vertex[4].mfV = 0.0f;
    vertex[4].mfSU = 0.0f;
    vertex[4].mfSV = 0.0f;
    // 右上
    vertex[5].mvPos = VECTOR3D(witdh, height, depth);
    vertex[5].mvNorm = VECTOR3D(0.0f, 1.0f, 0.0f);
    vertex[5].mfU = 1.0f;
    vertex[5].mfV = 0.0f;
    vertex[5].mfSU = 0.0f;
    vertex[5].mfSV = 0.0f;
    // 左下
    vertex[6].mvPos = VECTOR3D(-witdh, height, -depth);
    vertex[6].mvNorm = VECTOR3D(0.0f, 1.0f, 0.0f);
    vertex[6].mfU = 0.0f;
    vertex[6].mfV = 1.0f;
    vertex[6].mfSU = 0.0f;
    vertex[6].mfSV = 0.0f;
    // 右下
    vertex[7].mvPos = VECTOR3D(witdh, height, -depth);
    vertex[7].mvNorm = VECTOR3D(0.0f, 1.0f, 0.0f);
    vertex[7].mfU = 1.0f;
    vertex[7].mfV = 1.0f;
    vertex[7].mfSU = 0.0f;
    vertex[7].mfSV = 0.0f;

    // 左面
    // 左上
    vertex[8].mvPos = VECTOR3D(-witdh, height, depth);
    vertex[8].mvNorm = VECTOR3D(-1.0f, 0.0f, 0.0f);
    vertex[8].mfU = 0.0f;
    vertex[8].mfV = 0.0f;
    vertex[8].mfSU = 0.0f;
    vertex[8].mfSV = 0.0f;
    // 右上
    vertex[9].mvPos = VECTOR3D(-witdh, height, -depth);
    vertex[9].mvNorm = VECTOR3D(-1.0f, 0.0f, 0.0f);
    vertex[9].mfU = 0.0f;
    vertex[9].mfV = 1.0f;
    vertex[9].mfSU = 0.0f;
    vertex[9].mfSV = 0.0f;
    // 左下
    vertex[10].mvPos = VECTOR3D(-witdh, -height, depth);
    vertex[10].mvNorm = VECTOR3D(-1.0f, 0.0f, 0.0f);
    vertex[10].mfU = 1.0f;
    vertex[10].mfV = 0.0f;
    vertex[10].mfSU = 0.0f;
    vertex[10].mfSV = 0.0f;
    // 右下
    vertex[11].mvPos = VECTOR3D(-witdh, -height, -depth);
    vertex[11].mvNorm = VECTOR3D(-1.0f, 0.0f, 0.0f);
    vertex[11].mfU = 1.0f;
    vertex[11].mfV = 1.0f;
    vertex[11].mfSU = 0.0f;
    vertex[11].mfSV = 0.0f;

    // 裏面
    // 左上
    vertex[12].mvPos = VECTOR3D(witdh, height, depth);
    vertex[12].mvNorm = VECTOR3D(0.0f, 0.0f, 1.0f);
    vertex[12].mfU = 0.0f;
    vertex[12].mfV = 0.0f;
    vertex[12].mfSU = 0.0f;
    vertex[12].mfSV = 0.0f;
    // 右上
    vertex[13].mvPos = VECTOR3D(-witdh, height, depth);
    vertex[13].mvNorm = VECTOR3D(0.0f, 0.0f, 1.0f);
    vertex[13].mfU = 1.0f;
    vertex[13].mfV = 0.0f;
    vertex[13].mfSU = 0.0f;
    vertex[13].mfSV = 0.0f;
    // 左下
    vertex[14].mvPos = VECTOR3D(witdh, -height, depth);
    vertex[14].mvNorm = VECTOR3D(0.0f, 0.0f, 1.0f);
    vertex[14].mfU = 0.0f;
    vertex[14].mfV = 1.0f;
    vertex[14].mfSU = 0.0f;
    vertex[14].mfSV = 0.0f;
    // 右下
    vertex[15].mvPos = VECTOR3D(-witdh, -height, depth);
    vertex[15].mvNorm = VECTOR3D(0.0f, 0.0f, 1.0f);
    vertex[15].mfU = 1.0f;
    vertex[15].mfV = 1.0f;
    vertex[15].mfSU = 0.0f;
    vertex[15].mfSV = 0.0f;

    // 底面
    // 左上
    vertex[16].mvPos = VECTOR3D(-witdh, -height, -depth);
    vertex[16].mvNorm = VECTOR3D(0.0f, -1.0f, 0.0f);
    vertex[16].mfU = 0.0f;
    vertex[16].mfV = 0.0f;
    vertex[16].mfSU = 0.0f;
    vertex[16].mfSV = 0.0f;
    // 右上
    vertex[17].mvPos = VECTOR3D(witdh, -height, -depth);
    vertex[17].mvNorm = VECTOR3D(0.0f, -1.0f, 0.0f);
    vertex[17].mfU = 1.0f;
    vertex[17].mfV = 0.0f;
    vertex[17].mfSU = 0.0f;
    vertex[17].mfSV = 0.0f;
    // 左下
    vertex[18].mvPos = VECTOR3D(-witdh, -height, depth);
    vertex[18].mvNorm = VECTOR3D(0.0f, -1.0f, 0.0f);
    vertex[18].mfU = 0.0f;
    vertex[18].mfV = 1.0f;
    vertex[18].mfSU = 0.0f;
    vertex[18].mfSV = 0.0f;
    // 右下
    vertex[19].mvPos = VECTOR3D(witdh, -height, depth);
    vertex[19].mvNorm = VECTOR3D(0.0f, -1.0f, 0.0f);
    vertex[19].mfU = 1.0f;
    vertex[19].mfV = 1.0f;
    vertex[19].mfSU = 0.0f;
    vertex[19].mfSV = 0.0f;

    // 右面
    // 左上
    vertex[20].mvPos = VECTOR3D(witdh, height, -depth);
    vertex[20].mvNorm = VECTOR3D(1.0f, 0.0f, 0.0f);
    vertex[20].mfU = 0.0f;
    vertex[20].mfV = 0.0f;
    vertex[20].mfSU = 0.0f;
    vertex[20].mfSV = 0.0f;
    // 右上
    vertex[21].mvPos = VECTOR3D(witdh, height, depth);
    vertex[21].mvNorm = VECTOR3D(1.0f, 0.0f, 0.0f);
    vertex[21].mfU = 1.0f;
    vertex[21].mfV = 0.0f;
    vertex[21].mfSU = 0.0f;
    vertex[21].mfSV = 0.0f;
    // 左下
    vertex[22].mvPos = VECTOR3D(witdh, -height, -depth);
    vertex[22].mvNorm = VECTOR3D(1.0f, 0.0f, 0.0f);
    vertex[22].mfU = 0.0f;
    vertex[22].mfV = 1.0f;
    vertex[22].mfSU = 0.0f;
    vertex[22].mfSV = 0.0f;
    // 右下
    vertex[23].mvPos = VECTOR3D(witdh, -height, depth);
    vertex[23].mvNorm = VECTOR3D(1.0f, 0.0f, 0.0f);
    vertex[23].mfU = 1.0f;
    vertex[23].mfV = 1.0f;
    vertex[23].mfSU = 0.0f;
    vertex[23].mfSV = 0.0f;

    // 六面なので
    for (int i = 0; i < 6; i++)
    {
        index[(i * 6) + 0] = 0 + (i * 4);
        index[(i * 6) + 1] = 1 + (i * 4);
        index[(i * 6) + 2] = 2 + (i * 4);
        index[(i * 6) + 1] = 1 + (i * 4);
        index[(i * 6) + 3] = 3 + (i * 4);
        index[(i * 6) + 2] = 2 + (i * 4);
    }
}

void PartsBox::SetScaling(float x, float y, float z)
{
    this->mvScalingParam = VECTOR3D(x, y, z);
}

void PartsBox::SetScaling(const VECTOR3D &scaling)
{
    this->mvScalingParam = scaling;
}

void PartsBox::SetOffSet(float x, float y, float z)
{
    this->mvOffSetParam = VECTOR3D(x, y, z);
}

void PartsBox::SetOffSet(const VECTOR3D &offSet)
{
    this->mvOffSetParam = offSet;
}

void PartsBox::SetRotation(float x, float y, float z)
{
    this->mvRotationParam = VECTOR3D(x, y, z);
}

void PartsBox::SetRotation(const VECTOR3D &rotation)
{
    this->mvRotationParam = rotation;
}

void PartsBox::SetTranslation(float x, float y, float z)
{
    this->mvTranslationParam = VECTOR3D(x, y, z);
}

void PartsBox::SetTranslation(const VECTOR3D &translation)
{
    this->mvTranslationParam = translation;
}

void PartsBox::SetUV(BoxFace face, float uStart, float vStart, float uEnd, float vEnd)
{
    vertex[((unsigned char)(face) * 4) + 0].mfU = uStart;
    vertex[((unsigned char)(face) * 4) + 0].mfV = vStart;
    vertex[((unsigned char)(face) * 4) + 1].mfU = uEnd;
    vertex[((unsigned char)(face) * 4) + 1].mfV = vStart;
    vertex[((unsigned char)(face) * 4) + 2].mfU = uStart;
    vertex[((unsigned char)(face) * 4) + 2].mfV = vEnd;
    vertex[((unsigned char)(face) * 4) + 3].mfU = uEnd;
    vertex[((unsigned char)(face) * 4) + 3].mfV = vEnd;
}

void PartsBox::SetGraphHandle(TextureHandle handle)
{
    this->mnGraphHandle = handle;
}

void PartsBox::SetParent(PartsBox *parent)
{
    this->mpParent = parent;
}

VECTOR3D PartsBox::GetScaling()
{
    return mvScalingParam;
}

VECTOR3D PartsBox::GetOffSet()
{
    return mvOffSetParam;
}

VECTOR3D PartsBox::GetRotation()
{
    return mvRotationParam;
}

VECTOR3D PartsBox::GetTranslation()
{
    return mvTranslationParam;
}
