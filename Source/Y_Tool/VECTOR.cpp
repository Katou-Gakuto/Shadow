#include "VECTOR.h"

#include <math.h>
#include <float.h>

#ifdef USE_MY_MATRIX
#include "MyMatrix.h"
#endif // USE_MY_MATRIX

#define VECTOR_EQUAL_RANGE (0.000001f)

// このcpp内にしか存在しない関数の列挙
bool EqualInRangeF_VECTOR_cpp(float target, float base, float range)
{
    return ((base - range <= target) && (target <= base + range));
}
float GetFloatLerp_VECTOR_cpp(float start, float end, float rate)
{
    // 補間率を調整する
    if (rate < 0.0f)
    {
        rate = 0.0f;
    }
    if (1.0f < rate)
    {
        rate = 1.0f;
    }

    // 返り値
    float temp = 0.0f;

    // startからtargetまでの差分を取得する
    float distanceF = end - start;

    // 差分に倍率(補間率)をかけ、差分から戻すためにstartを足す
    temp = (distanceF * rate) + start;

    // 値を返す
    return temp;
}
float GetClamp01_VECTOR_cpp(float target)
{
    if (target <0.0f)
    {
        target = 0.0f;
    }
    else if (1.0f < target)
    {
        target = 1.0f;
    }
    return target;
}

#ifdef USE_MY_VECTOR2D
VECTOR2D::VECTOR2D() :
    mfParamX(0.0f),
    mfParamY(0.0f)
{

}

VECTOR2D::VECTOR2D(float xParam, float yParam) :
    mfParamX(xParam),
    mfParamY(yParam)
{

}

VECTOR2D::~VECTOR2D()
{

}

VECTOR2D VECTOR2D::Add(const VECTOR2D &vec) const
{
    return VECTOR2D(this->mfParamX + vec.mfParamX, this->mfParamY + vec.mfParamY);
}

VECTOR2D VECTOR2D::Sub(const VECTOR2D &vec) const
{
    return VECTOR2D(this->mfParamX - vec.mfParamX, this->mfParamY - vec.mfParamY);
}

VECTOR2D VECTOR2D::Scale(const float size) const
{
    return VECTOR2D(this->mfParamX * size, this->mfParamY * size);
}

// ベクトルの大きさを取得する(√あり)
float VECTOR2D::Magnitude() const
{
    // 
    float param = (this->mfParamX * this->mfParamX) + (this->mfParamY * this->mfParamY);
    float temp = 0.0f;
    if (EqualInRangeF_VECTOR_cpp(param, 0.0f, 0.00001f) == false)
    {
        // で√をつくってます。
        temp = sqrtf(param);
    }
    else
    {
        temp = 0.0f;
    }
    return temp;
}

// ベクトルの大きさを取得する(√なし)
float VECTOR2D::MagnitudeNoSqrt() const
{
    return (this->mfParamX * this->mfParamX) + (this->mfParamY * this->mfParamY);
}

// ベクトルの正規化(√あり)
VECTOR2D VECTOR2D::Normalize() const
{
    VECTOR2D temp = *this;
    float magnitude = this->Magnitude();
    if (EqualInRangeF_VECTOR_cpp(magnitude, 0.0f, 0.00001f) == false)
    {
        temp.mfParamX = (EqualInRangeF_VECTOR_cpp(this->mfParamX, 0.0f, 0.00001f) == false ? (this->mfParamX / magnitude) : (0.0f));
        temp.mfParamY = (EqualInRangeF_VECTOR_cpp(this->mfParamY, 0.0f, 0.00001f) == false ? (this->mfParamY / magnitude) : (0.0f));
    }
    else
    {
        temp.mfParamX = 0.0f;
        temp.mfParamY = 0.0f;
    }
    return temp;
}

// ベクトルの正規化(√なし)
// ※本来のサイズよりかなり小さいベクトルになります。ベクトル同士の比較などにのみ使用してください
VECTOR2D VECTOR2D::NormalizeNoSqrt() const
{
    VECTOR2D temp = *this;
    float magnitude = this->MagnitudeNoSqrt();
    if (EqualInRangeF_VECTOR_cpp(magnitude, 0.0f, 0.00001f) == false)
    {
        temp.mfParamX = (EqualInRangeF_VECTOR_cpp(this->mfParamX, 0.0f, 0.00001f) == false ? (this->mfParamX / magnitude) : (0.0f));
        temp.mfParamY = (EqualInRangeF_VECTOR_cpp(this->mfParamY, 0.0f, 0.00001f) == false ? (this->mfParamY / magnitude) : (0.0f));
    }
    else
    {
        temp.mfParamX = 0.0f;
        temp.mfParamY = 0.0f;
    }
    return temp;
}

// ベクトルの正規化→大きさの掛け算(√あり)
VECTOR2D VECTOR2D::SetMagnitude(const float size)  const
{
    return this->Normalize() * size;
}

// ベクトルの正規化→大きさの掛け算(√なし)
// ※使用非推奨の関数です
VECTOR2D VECTOR2D::SetMagnitudeNoSqrt(const float size) const
{
    return NormalizeNoSqrt() * size;
}

VECTOR2D VECTOR2D::GetZero()
{
    return VECTOR2D(0.0f, 0.0f);
}

VECTOR2D VECTOR2D::GetLeft()
{
    return VECTOR2D(-1.0f, 0.0f);
}

VECTOR2D VECTOR2D::GetRight()
{
    return VECTOR2D(1.0f, 0.0f);
}

VECTOR2D VECTOR2D::GetUp()
{
    return VECTOR2D(0.0f, -1.0f);
}

VECTOR2D VECTOR2D::GetDown()
{
    return VECTOR2D(0.0f, 1.0f);
}

// ベクトルの内積を取得する関数
float VECTOR2D::Dot(const VECTOR2D &vecL, const VECTOR2D &vecR)
{
    // これらしい。原理はわからん。
    return ((vecL.mfParamX * vecR.mfParamX) + (vecL.mfParamY * vecR.mfParamY));
}

// 二つのベクトルの直行する成分(Zを取得するやつ)
float VECTOR2D::Cross(const VECTOR2D &vec1, const VECTOR2D &vec2)
{
    return  vec1.mfParamX * vec2.mfParamY - vec2.mfParamX * vec1.mfParamY;
}

VECTOR2D VECTOR2D::Reflect(const VECTOR2D &vec, const VECTOR2D &mirrorNorm)
{
    // ① 光線ベクトルと法線ベクトルの内積
    float dot = VECTOR2D::Dot(vec, mirrorNorm);

    // ② その値を2倍
    float value = 2.0f * dot;

    // ③ 法線ベクトルに掛ける
    VECTOR2D offset = mirrorNorm * value;

    // ④ 入射ベクトルから引く
    VECTOR2D result = vec - offset;

    // ⑤ 反射ベクトル
    return result;
}

// 指定の範囲内でベクトルが等しいか
bool VECTOR2D::EqualInRange(const VECTOR2D &vec1, const VECTOR2D &vec2, const float range)
{
    return (EqualInRangeF_VECTOR_cpp(vec1.mfParamX, vec2.mfParamX, range) && EqualInRangeF_VECTOR_cpp(vec1.mfParamY, vec2.mfParamY, range));
}

// 点pと直線ABの線上最近点を求め、世界座標を返す
VECTOR2D VECTOR2D::GetNewrPosOnLine(const VECTOR2D &pointPos, const VECTOR2D &startPos, const VECTOR2D &endPos)
{
    // 二つのベクトルを作る
    VECTOR2D startToEnd = endPos - startPos;
    VECTOR2D StartToPoint = pointPos - startPos;

    // ABベクトルの単位ベクトルを求める
    VECTOR2D startToEndNorm = startToEnd.Normalize();

    // Aから線上最近点までの距離
    float distance = Dot(startToEndNorm, StartToPoint);

    // 線上最近点
    VECTOR2D newrPoint(
        startPos.mfParamX + (startToEndNorm.mfParamX * distance),
        startPos.mfParamY + (startToEndNorm.mfParamY * distance));

    // 線上最近点を返す
    return newrPoint;
}

// vecAの方向成分とvecBの方向成分のどちらがよりtargetVecの方向成分に近いかを確認する関数。
// AとBのより方向成分が近い方のベクトルを返す
VECTOR2D VECTOR2D::GetTargetDirectionNewrVec(const VECTOR2D &targetVec, const VECTOR2D &vecA, const VECTOR2D &vecB)
{
    VECTOR2D returnValue = targetVec;
    float target_vecA_Dot = Dot(targetVec, vecA);
    float target_vecB_Dot = Dot(targetVec, vecB);

    // より+1に近い方を返す
    if (target_vecB_Dot <= target_vecA_Dot)
    {
        returnValue = vecA;
    }
    else
    {
        returnValue = vecB;
    }
    return returnValue;
}

// 線形補間
VECTOR2D VECTOR2D::GetLerpPos(const VECTOR2D &startPos, const VECTOR2D &targetPos, float rate)
{
    // 補間率を調整する
    if (rate < 0.0f)
    {
        rate = 0.0f;
    }
    if (1.0f < rate)
    {
        rate = 1.0f;
    }

    // 返り値
    VECTOR2D temp = GetZero();

    // startからtargetまでの矢印ベクトルを取得する
    VECTOR2D distanceVec = targetPos - startPos;

    // 矢印ベクトルに倍率(補間率)をかける
    VECTOR2D lerpVec = distanceVec * rate;

    // 倍率のかかった矢印ベクトルを世界座標に直す
    temp = lerpVec + startPos;

    // 値を返す
    return temp;
}

// 円形補間
VECTOR2D VECTOR2D::GetSlerpPos(const VECTOR2D &centerPos, const VECTOR2D &startPos, const VECTOR2D &targetPos, float rate)
{
    // 補間率を調整する
    if (rate < 0.0f)
    {
        rate = 0.0f;
    }
    if (1.0f < rate)
    {
        rate = 1.0f;
    }

    // 返り値
    VECTOR2D temp = GetZero();

    // startからtargetまでを線形補間し、補間後の方向を意味するVECTORを取得する
    VECTOR2D lerpVec = GetLerpPos(
        startPos- centerPos,
        targetPos- centerPos,
        rate);

    // startのマグニチュードからtargetのマグニチュードまでを線形補間し、補間後のマグニチュードを取得する
    float lerpMagnitude = GetFloatLerp_VECTOR_cpp(
        (startPos - centerPos).Magnitude(),
        (targetPos - centerPos).Magnitude(),
        rate);

    // 補間後の方向と補間後のマグニチュードを合わせ、補間後の矢印ベクトルを取得する
    VECTOR2D slerpVec = lerpVec.SetMagnitude(lerpMagnitude);

    // 補間後の矢印ベクトルを世界座標に直す
    temp = slerpVec + centerPos;

    // 値を返す
    return temp;
}

// ベクトルAがベクトルBを軸に+方向か-方向か、それともまったく別の方向に向かっているのかを判断する関数
// プラスなら1、マイナスなら-1、別ベクトルなら0を返す
int VECTOR2D::CheckAxisVecPlusOrMinus(const VECTOR2D &targetVec, const VECTOR2D &axisVec)
{
    int temp = 0;

    // 二つのベクトルの単位ベクトルを取得
    VECTOR2D targetNorm = targetVec.Normalize();
    VECTOR2D axisNorm = axisVec.Normalize();

    // プラス方向かマイナス方向かを確認する
    if (EqualInRange(targetNorm, axisNorm, 0.00001f) == true)
    {
        // プラス方向だったので
        temp = 1;
    }
    else if (EqualInRange(targetNorm, axisNorm, 0.00001f) == true)
    {
        // マイナス方向だったので
        temp = -1;
    }
    else
    {
        // そもそも別方向のものだったので
        temp = 0;
    }

    return temp;
}

// 点pが直線ABの中にあるかを判断する関数
// 直線ABの中にあるなら1、AからBの延長線上にはあるが直線内にないなら0、そもそもその延長線上にすらないなら-1を返す
int VECTOR2D::CheckPointInVector(const VECTOR2D &targetPointPos, const VECTOR2D &vecStartPos, VECTOR2D &vecEndPos)
{
    // 返り値
    int temp = -1;

    // 原点。点Oみたいなもんです
    VECTOR2D startPos = vecStartPos - vecStartPos;

    // 原点からベクトルがどれだけ進んでいるか
    VECTOR2D endVec = vecEndPos - vecStartPos;

    // 原点からどれだけ離れた位置にターゲットの点Pがあるか
    VECTOR2D targetVec = targetPointPos - vecStartPos;

    // break文で抜け出したいがためのdo文なので意味はないです
    do
    {
        // ターゲットがスタートと同じ場所にあった場合は1を返す
        if (targetVec.mfParamX == startPos.mfParamX &&
            targetVec.mfParamY == startPos.mfParamY)
        {
            temp = 1;
            break;
        }

        // スタートとエンドが同じ場所にある場合
        if (endVec.mfParamX == startPos.mfParamX &&
            endVec.mfParamY == startPos.mfParamY)
        {
            // この時点でターゲットがスタートにいないことが確定しているため、即終了
            break;
        }

        // ターゲットがそれぞれの軸に、どのような状態にあるのかを記憶しておくためのbool
        bool targetInX = false;
        bool targetInY = false;
        bool targetOnX = false;
        bool targetOnY = false;

        // endVec.xが+方向なのか、-方向なのかを判断している
        if (startPos.mfParamX < endVec.mfParamX)
        {
            // targetもEndと同じように+方向に進んでいるか
            if (startPos.mfParamX <= targetVec.mfParamX)
            {
                targetOnX = true;
                // targetはstartPosから伸びるendVecの中に納まっているか
                if (targetVec.mfParamX <= endVec.mfParamX)
                {
                    targetInX = true;
                }
            }
            else
            {
                // targetは-方向に進んでいたため、延長線上にすらないと判断する
                break;
            }
        }
        else if (endVec.mfParamX < startPos.mfParamX)
        {
            // endVecと同じ方向を向いている場合はtrue
            if (targetVec.mfParamX <= startPos.mfParamX)
            {
                targetOnX = true;
                // ターゲットからスタートから伸びるエンドまでの中に納まっている場合はtrue
                if (endVec.mfParamX <= targetVec.mfParamX)
                {
                    targetInX = true;
                }
            }
            else
            {
                // そもそもターゲットがプラス方向に伸びているので即終了
                break;
            }
        }
        else/* (vecEndPos.x == startPos.x) */
        {
            // ようするに0地点でなくちゃいけない
            if (targetVec.mfParamX == startPos.mfParamX)
            {
                targetOnX = true;
                targetInX = true;
            }
            else/* (targetVec.x != startPos.x) */
            {
                break;
            }
        }

        // endVec.yが+方向なのか、-方向なのかを判断している
        if (startPos.mfParamY < endVec.mfParamY)
        {
            // targetもEndと同じように+方向に進んでいるか
            if (startPos.mfParamY <= targetVec.mfParamY)
            {
                targetOnY = true;
                // targetはstartPosから伸びるendVecの中に納まっているか
                if (targetVec.mfParamY <= endVec.mfParamY)
                {
                    targetInY = true;
                }
            }
            else
            {
                // targetは-方向に進んでいたため、延長線上にすらないと判断する
                break;
            }
        }
        else if (endVec.mfParamY < startPos.mfParamY)
        {
            // endVecと同じ方向を向いている場合はtrue
            if (targetVec.mfParamY <= startPos.mfParamY)
            {
                targetOnY = true;
                // ターゲットからスタートから伸びるエンドまでの中に納まっている場合はtrue
                if (endVec.mfParamY <= targetVec.mfParamY)
                {
                    targetInY = true;
                }
            }
            else
            {
                // そもそもターゲットがプラス方向に伸びているので即終了
                break;
            }
        }
        else/* (vecEndPos.y == startPos.y) */
        {
            // ようするに0地点でなくちゃいけない
            if (targetVec.mfParamY == startPos.mfParamY)
            {
                targetOnY = true;
                targetInY = true;
            }
            else/* (targetVec.y != startPos.y) */
            {
                break;
            }
        }

        // 全ての軸でtargetVecがstartPosから伸びるendVecの中に納まっていたらtrue
        if (targetInX == true &&
            targetInY == true)
        {
            temp = 1;
            break;
        }

        // 全ての軸でtargetVecはendVecと同じような方向に向かっているか
        if (targetOnX == true &&
            targetOnY == true)
        {
            if (CheckAxisVecPlusOrMinus(targetVec, endVec) == 1)
            {
                temp = 0;
                break;
            }
            else
            {
                temp = -1;
                break;
            }
        }
    } while (false);

    return temp;
}

// moveVecに加える"スライド移動の分"の移動ベクトルを取得する関数。normVecには√ありで正規化したVECTOR2Dを渡してください
VECTOR2D VECTOR2D::GetSlide(const VECTOR2D &moveVec, const VECTOR2D &normVec)
{
    // 壁ずり移動の進行ベクトル保持する変数
    VECTOR2D slideMoveVec(0.0f, 0.0f);

    // 進行ベクトルと、壁の法線ベクトルの内積の逆を係数aとしておく
    float a = -Dot(moveVec, normVec);

    // 現在の移動の進行ベクトルから壁の法線成分を抜いた新しい進行ベクトルを取得する
    slideMoveVec = normVec * a;

    // 
    return slideMoveVec;
}

// 特定の平面上にある単位円上の点の座標を、ラジアン角から取得する関数
VECTOR2D VECTOR2D::GetOnUnitCirclePos(const char planeX, const char planeY, float radian)
{
    VECTOR2D temp;

    // ここのスイッチ文の中で行っているtempへの代入について詳しく知りたい場合は
    // 『授業用/学びまとめ/三角関数について.txt』を参照してください
    switch (planeX)
    {
    case 'x':
    case 'X':
        switch (planeY)
        {
        case 'y':
        case 'Y':
            // radian == 0
            // x = 0
            // y = 1
            temp.mfParamX = -sinf(radian);
            temp.mfParamY = cosf(radian);
            break;
        case 'z':
        case 'Z':
            // radian == 0
            // x = 0
            // y = -1
            temp.mfParamX = -sinf(radian);
            temp.mfParamY = -cosf(radian);
            break;
        default:
            // 何もしない
            break;
        }
        break;
    case 'y':
    case 'Y':
        switch (planeY)
        {
        case 'x':
        case 'X':
            // radian == 0
            // x = 0
            // y = 1
            temp.mfParamX = -sinf(radian);
            temp.mfParamY = cosf(radian);
            break;
        case 'z':
        case 'Z':
            // radian == 0
            // x = 0
            // y = 1
            temp.mfParamX = -sinf(radian);
            temp.mfParamY = cosf(radian);
            break;
        default:
            // 何もしない
            break;
        }
        break;
    case 'z':
    case 'Z':
        switch (planeY)
        {
        case 'x':
        case 'X':
            // radian == 0
            // x = 0
            // y = -1
            temp.mfParamX = -sinf(radian);
            temp.mfParamY = -cosf(radian);
            break;
        case 'y':
        case 'Y':
            // radian == 0
            // x = 0
            // y = -1
            temp.mfParamX = -sinf(radian);
            temp.mfParamY = -cosf(radian);
            break;
        default:
            // 何もしない
            break;
        }
        break;
    default:
        break;
    }

    // 計算結果を返す
    return temp;
}

void VECTOR2D::SetX(float x)
{
    this->mfParamX = x;
}

void VECTOR2D::SetY(float y)
{
    this->mfParamY = y;
}

void VECTOR2D::SetXY(float x, float y)
{
    this->mfParamX = x;
    this->mfParamY = y;
}

float VECTOR2D::GetX() const
{
    return this->mfParamX;
}

float VECTOR2D::GetY() const
{
    return this->mfParamY;
}

VECTOR2D VECTOR2D::operator +() const
{
    return *this;
}

VECTOR2D VECTOR2D::operator -() const
{
    return VECTOR2D(-(this->mfParamX), -(this->mfParamY));
}

void VECTOR2D::operator =(const VECTOR2D &mat)
{
    this->mfParamX = mat.mfParamX;
    this->mfParamY = mat.mfParamY;
}

void VECTOR2D::operator +=(const VECTOR2D &mat)
{
    this->mfParamX += mat.mfParamX;
    this->mfParamY += mat.mfParamY;
}

void VECTOR2D::operator -=(const VECTOR2D &mat)
{
    this->mfParamX -= mat.mfParamX;
    this->mfParamY -= mat.mfParamY;
}

void VECTOR2D::operator *=(const float &scaler)
{
    this->mfParamX *= scaler;
    this->mfParamY *= scaler;
}

void VECTOR2D::operator /=(const float &scaler)
{
    if (EqualInRangeF_VECTOR_cpp(scaler, 0.0f, 1e-6f))
    {
        this->mfParamX = 0;
        this->mfParamY = 0;
    }
    else
    {
        this->mfParamX /= scaler;
        this->mfParamY /= scaler;
    }
}

#ifdef USE_MY_VECTOR3D
// VECTOR2DからVECOTR3Dに変換する関数
VECTOR3D VECTOR2D::To3D(float zParam) const
{
    return this->To3D('x', 'y', zParam);
}

// VECTOR2DからVECOTR3Dに変換する関数
VECTOR3D VECTOR2D::To3D(float newAxisParam, char y3D, char z3D) const
{
    VECTOR3D temp = VECTOR3D::GetZero();
    temp.SetX(newAxisParam);
    switch (y3D)
    {
    case 'x':
    case 'X':
        temp.SetY(this->mfParamX);
        break;
    case 'y':
    case 'Y':
        temp.SetY(this->mfParamY);
        break;
    default:
        break;
    }

    switch (z3D)
    {
    case 'x':
    case 'X':
        temp.SetZ(this->mfParamX);
        break;
    case 'y':
    case 'Y':
        temp.SetZ(this->mfParamY);
        break;
    default:
        break;
    }

    return temp;
}

// VECTOR2DからVECOTR3Dに変換する関数
VECTOR3D VECTOR2D::To3D(char x3D, float newAxisParam, char z3D) const
{
    VECTOR3D temp = VECTOR3D::GetZero();
    temp.SetY(newAxisParam);
    switch (x3D)
    {
    case 'x':
    case 'X':
        temp.SetX(this->mfParamX);
        break;
    case 'y':
    case 'Y':
        temp.SetX(this->mfParamY);
        break;
    default:
        break;
    }

    switch (z3D)
    {
    case 'x':
    case 'X':
        temp.SetZ(this->mfParamX);
        break;
    case 'y':
    case 'Y':
        temp.SetZ(this->mfParamY);
        break;
    default:
        break;
    }

    return temp;
}

// VECTOR2DからVECOTR3Dに変換する関数
VECTOR3D VECTOR2D::To3D(char x3D, char y3D, float newAxisParam) const
{
    VECTOR3D temp = VECTOR3D::GetZero();
    temp.SetZ(newAxisParam);
    switch (x3D)
    {
    case 'x':
    case 'X':
        temp.SetX(this->mfParamX);
        break;
    case 'y':
    case 'Y':
        temp.SetX(this->mfParamY);
        break;
    default:
        break;
    }

    switch (y3D)
    {
    case 'x':
    case 'X':
        temp.SetY(this->mfParamX);
        break;
    case 'y':
    case 'Y':
        temp.SetY(this->mfParamY);
        break;
    default:
        break;
    }

    return temp;

}
#endif // USE_MY_VECTOR3D

#ifdef USE_MY_MATRIX
// VECTOR2Dを計算できる行列に変換する関数
MyMatrix VECTOR2D::ToMat() const
{
    MyMatrix temp(1, 3);
    temp.SetParam(0, 0, this->mfParamX);
    temp.SetParam(0, 1, this->mfParamY);
    temp.SetParam(0, 2, 1);
    return temp;
}

// 行列を用いてVECTOR2Dの拡大・縮小(Scaling)、回転(Rotation)、平行移動(Transportion)を行う関数
VECTOR2D VECTOR2D::MatrixOperation(const VECTOR2D &s, float r, const VECTOR2D &t) const
{
    return ((((*this) * MyMatrix::GetScaling2D(s)) * MyMatrix::GetRotation2D(r)) * MyMatrix::GetTransportion2D(t)).To2VD();
}
#endif // USE_MY_MATRIX

VECTOR2D operator +(const VECTOR2D &vecL, const VECTOR2D &vecR)
{
    return VECTOR2D(vecL.GetX() + vecR.GetX(), vecL.GetY() + vecR.GetY());
}

VECTOR2D operator -(const VECTOR2D &vecL, const VECTOR2D &vecR)
{
    return VECTOR2D(vecL.GetX() - vecR.GetX(), vecL.GetY() - vecR.GetY());
}

VECTOR2D operator *(const VECTOR2D &vecL, float scaler)
{
    return VECTOR2D(vecL.GetX() + scaler, vecL.GetY() * scaler);
}

VECTOR2D operator /(const VECTOR2D &vecL, float scaler)
{
    VECTOR2D temp;
    if (EqualInRangeF_VECTOR_cpp(scaler, 0.0f, 1e-6f))
    {
        temp.SetX(0);
        temp.SetY(0);
    }
    else
    {
        temp.SetX(vecL.GetX() / scaler);
        temp.SetY(vecL.GetY() / scaler);
    }
    return temp;
}

bool operator ==(const VECTOR2D &vecL, const VECTOR2D &vecR)
{
    return vecL.EqualInRange(vecL, vecR, VECTOR_EQUAL_RANGE);
}

bool operator !=(const VECTOR2D &vecL, const VECTOR2D &vecR)
{
    return !(vecL.EqualInRange(vecL, vecR, VECTOR_EQUAL_RANGE));
}

#ifdef USE_MY_MATRIX
MyMatrix operator *(const VECTOR2D &vec, const MyMatrix &mat)
{
    MyMatrix vecMat = vec.ToMat();
    return vecMat * mat;
}
#endif // USE_MY_MATRIX
#endif // USE_MY_VECTOR2D

#ifdef USE_MY_VECTOR3D
// コンストラクタ
VECTOR3D::VECTOR3D():
    mfParamX(0.0f),
    mfParamY(0.0f),
    mfParamZ(0.0f)
{

}

// コンストラクタ
VECTOR3D::VECTOR3D(float xParam, float yParam, float zParam) :
    mfParamX(xParam),
    mfParamY(yParam),
    mfParamZ(zParam)
{    
}

// デストラクタ
VECTOR3D::~VECTOR3D()
{

}

VECTOR3D VECTOR3D::Add(const VECTOR3D &vec) const
{
    return VECTOR3D(this->mfParamX + vec.mfParamX, this->mfParamY + vec.mfParamY, this->mfParamZ + vec.mfParamZ);
}

VECTOR3D VECTOR3D::Sub(const VECTOR3D &vec) const
{
    return VECTOR3D(this->mfParamX - vec.mfParamX, this->mfParamY - vec.mfParamY, this->mfParamZ - vec.mfParamZ);
}

VECTOR3D VECTOR3D::Scale(const float size) const
{
    return VECTOR3D(this->mfParamX - size, this->mfParamY - size, this->mfParamZ - size);
}

float VECTOR3D::Magnitude() const
{
    // 
    float param = (this->mfParamX * this->mfParamX) + (this->mfParamY * this->mfParamY) + (this->mfParamZ * this->mfParamZ);
    float temp = 0.0f;
    if (EqualInRangeF_VECTOR_cpp(param, 0.0f, 0.0001f) == false)
    {
        // で√をつくってます。
        temp = sqrtf(param);
    }
    else
    {
        temp = 0.0f;
    }
    return temp;
}

float VECTOR3D::MagnitudeNoSqrt() const
{
    return ((this->mfParamX * this->mfParamX) + (this->mfParamY * this->mfParamY) + (this->mfParamZ * this->mfParamZ));
}

// ベクトルの正規化(√あり)
VECTOR3D VECTOR3D::Normalize() const
{
    VECTOR3D temp = *this;
    float magnitude = this->Magnitude();
    if (EqualInRangeF_VECTOR_cpp(magnitude, 0.0f, 0.00001f) == false)
    {
        temp.mfParamX = (EqualInRangeF_VECTOR_cpp(this->mfParamX, 0.0f, 0.00001f) == false ? (this->mfParamX / magnitude) : (0.0f));
        temp.mfParamY = (EqualInRangeF_VECTOR_cpp(this->mfParamY, 0.0f, 0.00001f) == false ? (this->mfParamY / magnitude) : (0.0f));
        temp.mfParamZ = (EqualInRangeF_VECTOR_cpp(this->mfParamZ, 0.0f, 0.00001f) == false ? (this->mfParamZ / magnitude) : (0.0f));
    }
    else
    {
        temp.mfParamX = 0.0f;
        temp.mfParamY = 0.0f;
        temp.mfParamZ = 0.0f;
    }
    return temp;
}

// ベクトルの正規化(√なし)
// ※本来のサイズよりかなり小さいベクトルになります。ベクトル同士の比較などにのみ使用してください
VECTOR3D VECTOR3D::NormalizeNoSqrt() const
{
    VECTOR3D temp = *this;
    float magnitude = this->MagnitudeNoSqrt();
    if (EqualInRangeF_VECTOR_cpp(magnitude, 0.0f, 0.00001f) == false)
    {
        temp.mfParamX = (EqualInRangeF_VECTOR_cpp(this->mfParamX, 0.0f, 0.00001f) == false ? (this->mfParamX / magnitude) : (0.0f));
        temp.mfParamY = (EqualInRangeF_VECTOR_cpp(this->mfParamY, 0.0f, 0.00001f) == false ? (this->mfParamY / magnitude) : (0.0f));
        temp.mfParamZ = (EqualInRangeF_VECTOR_cpp(this->mfParamZ, 0.0f, 0.00001f) == false ? (this->mfParamZ / magnitude) : (0.0f));
    }
    else
    {
        temp.mfParamX = 0.0f;
        temp.mfParamY = 0.0f;
        temp.mfParamZ = 0.0f;
    }
    return temp;
}

// ベクトルの正規化→大きさの掛け算(√あり)
VECTOR3D VECTOR3D::SetMagnitude(const float size) const
{
    return this->Normalize() * size;
}

// ベクトルの正規化→大きさの掛け算(√なし)
// ※使用非推奨の関数です
VECTOR3D VECTOR3D::SetMagnitudeNoSqrt(const float size) const
{
    return this->NormalizeNoSqrt() * size;
}

VECTOR3D VECTOR3D::GetZero()
{
    return VECTOR3D(0.0f, 0.0f, 0.0f);
}

VECTOR3D VECTOR3D::GetLeft()
{
    return VECTOR3D(-1.0f, 0.0f, 0.0f);
}

VECTOR3D VECTOR3D::GetRight()
{
    return VECTOR3D(1.0f, 0.0f, 0.0f);
}

VECTOR3D VECTOR3D::GetUp()
{
    return VECTOR3D(0.0f, 1.0f, 0.0f);
}

VECTOR3D VECTOR3D::GetDown()
{
    return VECTOR3D(0.0f, -1.0f, 0.0f);
}

VECTOR3D VECTOR3D::GetFront()
{
    return VECTOR3D(0.0f, 0.0f, 1.0f);
}

VECTOR3D VECTOR3D::GetBack()
{
    return VECTOR3D(0.0f, 0.0f, -1.0f);
}

float VECTOR3D::Dot(const VECTOR3D &vecL, const VECTOR3D &vecR)
{
    // これらしい。原理はわからん。
    return ((vecL.mfParamX * vecR.mfParamX) + (vecL.mfParamY * vecR.mfParamY) + (vecL.mfParamZ * vecR.mfParamZ));
}

VECTOR3D VECTOR3D::Cross(const VECTOR3D &vec1, const VECTOR3D &vec2)
{
    return VECTOR3D(
        vec1.mfParamY * vec2.mfParamZ - vec2.mfParamY * vec1.mfParamZ,
        vec1.mfParamZ * vec2.mfParamX - vec2.mfParamZ * vec1.mfParamX,
        vec1.mfParamX * vec2.mfParamY - vec2.mfParamX * vec1.mfParamY);

    /*
    外積ベクトルの求め方、簡易版
    VGet((vecA.y * vecB.z - vecA.z * vecB.y),
         (vecA.z * vecB.x - vecA.x * vecB.z)
         (vecA.x * vecB.y - vecA.y * vecB.x));

    外積ベクトルの求め方、ちょっと細かい版

    簡単に言うと、外積のxを求めたいならyとzで計算して、yならzとx、ｚならxとyで計算するっていう感じ。
    */
}

// 指定の範囲内でベクトルが等しいか。
bool VECTOR3D::EqualInRange(const VECTOR3D &vec1, const VECTOR3D &vec2, const float range)
{
    return (EqualInRangeF_VECTOR_cpp(vec1.mfParamX, vec2.mfParamX, range) && 
            EqualInRangeF_VECTOR_cpp(vec1.mfParamY, vec2.mfParamY, range) &&
            EqualInRangeF_VECTOR_cpp(vec1.mfParamZ, vec2.mfParamZ, range));
}

// 点pと直線ABから線上最近点を求め、世界座標を返す
VECTOR3D VECTOR3D::GetNewrPosOnLine(const VECTOR3D &pointPos, const VECTOR3D &startPos, const VECTOR3D &endPos)
{
    // 二つのベクトルを作る
    VECTOR3D startToEnd = endPos - startPos;
    VECTOR3D StartToPoint = pointPos - startPos;

    // ABベクトルの単位ベクトルを求める
    VECTOR3D startToEndNorm = startToEnd.Normalize();

    // Aから線上最近点までの距離
    float distance = Dot(startToEndNorm, StartToPoint);

    // 線上最近点
    VECTOR3D newrPoint(
        startPos.mfParamX + (startToEndNorm.mfParamX * distance),
        startPos.mfParamY + (startToEndNorm.mfParamY * distance),
        startPos.mfParamZ + (startToEndNorm.mfParamZ * distance));

    // 線上最近点を返す
    return newrPoint;
}

// vecAの方向成分とvecBの方向成分のどちらがよりtargetVecの方向成分に近いかを確認する関数。
// AとBのより方向成分が近い方のベクトルを返す
VECTOR3D VECTOR3D::GetTargetDirectionNewrVec(const VECTOR3D &targetVec, const VECTOR3D &vecA, const VECTOR3D &vecB)
{
    VECTOR3D returnValue = targetVec;
    float target_vecA_Dot = Dot(targetVec, vecA);
    float target_vecB_Dot = Dot(targetVec, vecB);

    // より+1に近い方を返す
    if (target_vecB_Dot <= target_vecA_Dot)
    {
        returnValue = vecA;
    }
    else
    {
        returnValue = vecB;
    }
    return returnValue;
}

// 線形補間
VECTOR3D VECTOR3D::GetLerpPos(const VECTOR3D &startPos, const VECTOR3D &targetPos, float rate)
{
    // 補間率を調整する
    if (rate < 0.0f)
    {
        rate = 0.0f;
    }
    if (1.0f < rate)
    {
        rate = 1.0f;
    }

    // 返り値
    VECTOR3D temp = GetZero();

    // startからtargetまでの矢印ベクトルを取得する
    VECTOR3D distanceVec = targetPos - startPos;

    // 矢印ベクトルに倍率(補間率)をかける
    VECTOR3D lerpVec = distanceVec * rate;

    // 倍率のかかった矢印ベクトルを世界座標に直す
    temp = lerpVec + startPos;

    // 値を返す
    return temp;
}

// 円形補間
VECTOR3D VECTOR3D::GetSlerpPos(const VECTOR3D &centerPos, const VECTOR3D &startPos, const VECTOR3D &targetPos, float rate)
{
    // 補間率を調整する
    if (rate < 0.0f)
    {
        rate = 0.0f;
    }
    if (1.0f < rate)
    {
        rate = 1.0f;
    }

    // 返り値
    VECTOR3D temp = GetZero();

    // startからtargetまでを線形補間し、補間後の方向を意味するVECTORを取得する
    VECTOR3D lerpVec = GetLerpPos(
        startPos - centerPos,
        targetPos - centerPos,
        rate);

    // startのマグニチュードからtargetのマグニチュードまでを線形補間し、補間後のマグニチュードを取得する
    float lerpMagnitude = GetFloatLerp_VECTOR_cpp(
        (startPos - centerPos).Magnitude(),
        (targetPos - centerPos).Magnitude(),
        rate);

    // 補間後の方向と補間後のマグニチュードを合わせ、補間後の矢印ベクトルを取得する
    VECTOR3D slerpVec = lerpVec.SetMagnitude(lerpMagnitude);

    // 補間後の矢印ベクトルを世界座標に直す
    temp = slerpVec + centerPos;

    // 値を返す
    return temp;
}

// ベクトルAがベクトルBを軸に+方向か-方向か、それともまったく別の方向に向かっているのかを判断する関数
// プラスなら1、マイナスなら-1、別ベクトルなら0を返す
int VECTOR3D::CheckAxisVecPlusOrMinus(const VECTOR3D &targetVec, const VECTOR3D &axisVec)
{
    int temp = 0;

    // 二つのベクトルの単位ベクトルを取得
    VECTOR3D targetNorm = targetVec.Normalize();
    VECTOR3D axisNorm = axisVec.Normalize();

    // プラス方向かマイナス方向かを確認する
    if (EqualInRange(targetNorm, axisNorm, 0.00001f) == true)
    {
        // プラス方向だったので
        temp = 1;
    }
    else if (EqualInRange(-targetNorm, axisNorm, 0.00001f) == true)
    {
        // マイナス方向だったので
        temp = -1;
    }
    else
    {
        // そもそも別方向のものだったので
        temp = 0;
    }

    return temp;
}

// 点pが直線ABの中にあるかを判断する関数
// 直線ABの中にあるなら1、AからBの延長線上にはあるが直線内にないなら0、そもそもその延長線上にすらないなら-1を返す
int VECTOR3D::CheckPointInVector(const VECTOR3D &targetPointPos, const VECTOR3D &vecStartPos, const VECTOR3D &vecEndPos)
{
    // 行数が多いだけで、やっていることは結構簡単です


    // 返り値
    int temp = -1;

    // 原点。点Oみたいなもんです
    VECTOR3D startPos = vecStartPos - vecStartPos;

    // 原点からベクトルがどれだけ進んでいるか
    VECTOR3D endVec = vecEndPos - vecStartPos;

    // 原点からどれだけ離れた位置にターゲットの点Pがあるか
    VECTOR3D targetVec = targetPointPos - vecStartPos;

    // break文で抜け出したいがためのdo文なので意味はないです
    do
    {
        // ターゲットがスタートと同じ場所にあった場合は1を返す
        if (targetVec == startPos)
        {
            temp = 1;
            break;
        }

        // スタートとエンドが同じ場所にある場合
        if (endVec == VECTOR3D::GetZero())
        {
            // この時点でターゲットがスタートにいないことが確定しているため、即終了
            break;
        }

        // ターゲットがそれぞれの軸に、どのような状態にあるのかを記憶しておくためのbool
        bool targetInX = false;
        bool targetInY = false;
        bool targetInZ = false;
        bool targetOnX = false;
        bool targetOnY = false;
        bool targetOnZ = false;

        // endVec.xが+方向なのか、-方向なのかを判断している
        if (startPos.mfParamX < endVec.mfParamX)
        {
            // targetもEndと同じように+方向に進んでいるか
            if (startPos.mfParamX <= targetVec.mfParamX)
            {
                targetOnX = true;
                // targetはstartPosから伸びるendVecの中に納まっているか
                if (targetVec.mfParamX <= endVec.mfParamX)
                {
                    targetInX = true;
                }
            }
            else
            {
                // targetは-方向に進んでいたため、延長線上にすらないと判断する
                break;
            }
        }
        else if (endVec.mfParamX < startPos.mfParamX)
        {
            // endVecと同じ方向を向いている場合はtrue
            if (targetVec.mfParamX <= startPos.mfParamX)
            {
                targetOnX = true;
                // ターゲットからスタートから伸びるエンドまでの中に納まっている場合はtrue
                if (endVec.mfParamX <= targetVec.mfParamX)
                {
                    targetInX = true;
                }
            }
            else
            {
                // そもそもターゲットがプラス方向に伸びているので即終了
                break;
            }
        }
        else/* (vecEndPos.x == startPos.x) */
        {
            // ようするに0地点でなくちゃいけない
            if (EqualInRangeF_VECTOR_cpp(targetVec.mfParamX, startPos.mfParamX, VECTOR_EQUAL_RANGE))
            {
                targetOnX = true;
                targetInX = true;
            }
            else/* (targetVec.x != startPos.x) */
            {
                break;
            }
        }

        // endVec.yが+方向なのか、-方向なのかを判断している
        if (startPos.mfParamY < endVec.mfParamY)
        {
            // targetもEndと同じように+方向に進んでいるか
            if (startPos.mfParamY <= targetVec.mfParamY)
            {
                targetOnY = true;
                // targetはstartPosから伸びるendVecの中に納まっているか
                if (targetVec.mfParamY <= endVec.mfParamY)
                {
                    targetInY = true;
                }
            }
            else
            {
                // targetは-方向に進んでいたため、延長線上にすらないと判断する
                break;
            }
        }
        else if (endVec.mfParamY < startPos.mfParamY)
        {
            // endVecと同じ方向を向いている場合はtrue
            if (targetVec.mfParamY <= startPos.mfParamY)
            {
                targetOnY = true;
                // ターゲットからスタートから伸びるエンドまでの中に納まっている場合はtrue
                if (endVec.mfParamY <= targetVec.mfParamY)
                {
                    targetInY = true;
                }
            }
            else
            {
                // そもそもターゲットがプラス方向に伸びているので即終了
                break;
            }
        }
        else/* (vecEndPos.y == startPos.y) */
        {
            // ようするに0地点でなくちゃいけない
            if (EqualInRangeF_VECTOR_cpp(targetVec.mfParamY, startPos.mfParamY, VECTOR_EQUAL_RANGE))
            {
                targetOnY = true;
                targetInY = true;
            }
            else/* (targetVec.y != startPos.y) */
            {
                break;
            }
        }

        // endVec.zが+方向なのか、-方向なのかを判断している
        if (startPos.mfParamZ < endVec.mfParamZ)
        {
            // targetもEndと同じように+方向に進んでいるか
            if (startPos.mfParamZ <= targetVec.mfParamZ)
            {
                targetOnZ = true;
                // targetはstartPosから伸びるendVecの中に納まっているか
                if (targetVec.mfParamZ <= endVec.mfParamZ)
                {
                    targetInZ = true;
                }
            }
            else
            {
                // targetは-方向に進んでいたため、延長線上にすらないと判断する
                break;
            }
        }
        else if (endVec.mfParamZ < startPos.mfParamZ)
        {
            // endVecと同じ方向を向いている場合はtrue
            if (targetVec.mfParamZ <= startPos.mfParamZ)
            {
                targetOnZ = true;
                // ターゲットからスタートから伸びるエンドまでの中に納まっている場合はtrue
                if (endVec.mfParamZ <= targetVec.mfParamZ)
                {
                    targetInZ = true;
                }
            }
            else
            {
                // そもそもターゲットがプラス方向に伸びているので即終了
                break;
            }
        }
        else/* (vecEndPos.z == startPos.z) */
        {
            // ようするに0地点でなくちゃいけない
            if (EqualInRangeF_VECTOR_cpp(targetVec.mfParamZ, startPos.mfParamZ, VECTOR_EQUAL_RANGE))
            {
                targetOnZ = true;
                targetInZ = true;
            }
            else/* (targetVec.z != startPos.z) */
            {
                break;
            }
        }

        // 全ての軸でtargetVecがstartPosから伸びるendVecの中に納まっていたらtrue
        if (targetInX == true &&
            targetInY == true &&
            targetInZ == true)
        {
            temp = 1;
            break;
        }

        // 全ての軸でtargetVecはendVecと同じような方向に向かっているか
        if (targetOnX == true &&
            targetOnY == true &&
            targetOnZ == true)
        {
            if (CheckAxisVecPlusOrMinus(targetVec, endVec) == 1)
            {
                temp = 0;
                break;
            }
            else
            {
                temp = -1;
                break;
            }
        }
    } while (false);

    return temp;
}

// moveVecに加える"スライド移動の分"の移動ベクトルを取得する関数。normVecには√ありで正規化したVECTOR2Dを渡してください
VECTOR3D VECTOR3D::GetSlide(const VECTOR3D &moveVec, const VECTOR3D &normVec)
{
    // 壁ずり移動の進行ベクトル保持する変数
    VECTOR3D slideMoveVec(0.0f, 0.0f, 0.0f);

    // 進行ベクトルと、壁の法線ベクトルの内積の逆を係数aとしておく
    float a = -Dot(moveVec, normVec);

    // 現在の移動の進行ベクトルから壁の法線成分を抜いた新しい進行ベクトルを取得する
    slideMoveVec = normVec * a;

    // 
    return slideMoveVec;
}

// ある点から無限平面までの距離を求める関数
float VECTOR3D::GetPointToPlaneDistance(const VECTOR3D &pointPos, const VECTOR3D &onPlanePos, const VECTOR3D &planeNorm)
{
    // (http://marupeke296.com/COL_Basic_No2_ShortTec.html#%E5%86%85%E5%88%86%E7%82%B9)このサイトを参考にします。
    // ベクトルの計算式によくある『|vn|』みたいな表記は、大きさの話っぽい。
    VECTOR3D v1 = onPlanePos - pointPos;
    return fabsf(VECTOR3D::Dot(planeNorm, v1) / planeNorm.Magnitude());
}

// 二つの直線の最短距離を調べる関数
float VECTOR3D::GetVECTOR3DToVECTOR3DNewrMagnitude()
{
    // (http://marupeke296.com/COL_Basic_No2_ShortTec.html#%E5%86%85%E5%88%86%E7%82%B9)このサイトを参考にします。
    VECTOR3D v1;
    VECTOR3D v2;
    VECTOR3D norm = VECTOR3D::Cross(v1, v2);
    return (VECTOR3D::Dot(norm, v1) / norm.Magnitude());    // 点から平面までの距離を求めるのとほぼ同じ中身。
}

// 線(線分ではないので長さ無限)と平面の衝突判定を求める関数
bool VECTOR3D::GetHitEndlessLineToEndlessPlane(const VECTOR3D &lineVec, const VECTOR3D &praneNorm)
{
    return VECTOR3D::Dot(lineVec, praneNorm) != 0.0f;
}

// 線分と無限平面の衝突判定を求める関数
bool VECTOR3D::GetHitLineToEndlessPlane(const VECTOR3D &lineStart, const VECTOR3D &lineEnd, const VECTOR3D &planePos, const VECTOR3D &praneNorm)
{

    VECTOR3D dir = lineEnd - lineStart;
    double denom = VECTOR3D::Dot(praneNorm, dir);

    // denom == 0 → 線分が平面と平行（交差しない or 平面上）
    if (fabsf(denom) < 1e-6f)
    {
        return false;
    }

    float t = VECTOR3D::Dot(praneNorm, planePos - lineStart) / denom;

    // 交点が線分上にあるかチェック
    if (t < 0.0 || t > 1.0)
    {
        return false;
    }

    /*
    if (out != nullptr) {
        *out = L1 + dir * t;
    }
    */
    return true;
}

// 線分と板ポリゴンの衝突判定を求める関数
bool VECTOR3D::GetHitLineToPolygon()
{
    VECTOR3D porigonVertex;
    VECTOR3D porigonNorm;
    VECTOR3D lineStart;
    VECTOR3D lineEnd;
    VECTOR3D lineVec = (lineEnd - lineStart);
    VECTOR3D vertex2start = lineStart - porigonVertex;
    VECTOR3D vertex2end = lineEnd - porigonVertex;

    if (VECTOR3D::GetHitLineToEndlessPlane(lineStart, lineEnd, porigonVertex, porigonNorm) == true)
    {
        float start2porigonDistance = VECTOR3D::GetPointToPlaneDistance(lineStart, porigonVertex, porigonNorm);
        float end2porigonDistance = VECTOR3D::GetPointToPlaneDistance(lineEnd, porigonVertex, porigonNorm);
        float a = start2porigonDistance / (start2porigonDistance + end2porigonDistance);
        VECTOR3D hitPos = porigonVertex + ((vertex2start * (1.0f - a)) + (vertex2end * a));

        // ↓ここでポリゴンのn角形と点の当たり判定を行い、その当たり判定の結果がこの関数の結果になる。
        return true;
    }
    return false;
}

// ある点からある線への垂線を取得する関数
VECTOR3D VECTOR3D::GetPerpendicular(const VECTOR3D &point, const VECTOR3D &lineStart, const VECTOR3D &lineEnd)
{
    VECTOR3D newrPos = VECTOR3D::GetNewrPosOnLine(point, lineStart, lineEnd);
    return newrPos - point;
}

// 点と線分の関係性を返す関数
// 返り値が0の場合は点は線分の中、1の場合は点は線分のend方向の延長線上、-1の場合は点は線分のstart方向の延長線上、-2の場合は線分とは関係がないという意味。
int VECTOR3D::CheckPointAndLine(const VECTOR3D &pointPos, const VECTOR3D &lineStartPos, const VECTOR3D &lineEndPos)
{
    int returnValue = -2;

    VECTOR3D start2point = pointPos - lineStartPos;
    VECTOR3D cross0 = VECTOR3D::Cross(pointPos, lineStartPos);
    if (cross0 == VECTOR3D::GetZero())
    {
        // ここの時点でstart→endベクトルとstart→pointベクトルが平行であることを確認してある
        // つまり、ベクトルの中 or プラス延長線上のどこか or マイナ延長線上のどこかにあることが確定している

        // 今から書いてある内容はVECTOR3D::CheckPointInVector()とやってることほとんど同じです。

        bool pointInX = false;
        bool pointInY = false;
        bool pointInZ = false;
        bool pointOnX = false;
        bool pointOnY = false;
        bool pointOnZ = false;
        bool pointMinX = false;
        bool pointMinY = false;
        bool pointMinZ = false;

        VECTOR3D lineVec = lineEndPos - lineStartPos;

        // 線分のx成分の増加方向を確認して処理を変える
        if (lineVec.GetX() < 0.0f)
        {
            if (pointPos.GetX() <= lineStartPos.GetX())
            {
                if (lineEndPos.GetX() <= pointPos.GetX())
                {
                    pointInX = true;
                }
                else
                {
                    pointOnX = true;
                }
            }
            else
            {
                pointMinX = true;
            }
        }
        else if (0.0f < lineVec.GetX())
        {
            if (lineStartPos.GetX() <= pointPos.GetX())
            {
                if (pointPos.GetX() <= lineEndPos.GetX())
                {
                    pointInX = true;
                }
                else
                {
                    pointOnX = true;
                }
            }
            else
            {
                pointMinX = true;
            }
        }
        else
        {
            if (EqualInRangeF_VECTOR_cpp(lineStartPos.GetX(), pointPos.GetX(), VECTOR_EQUAL_RANGE))
            {
                pointInX = true;
                pointOnX = true;
                pointMinX = true;
            }
        }

        // 線分のy成分の増加方向を確認して処理を変える
        if (lineVec.GetY() < 0.0f)
        {
            if (pointPos.GetY() <= lineStartPos.GetY())
            {
                if (lineEndPos.GetY() <= pointPos.GetY())
                {
                    pointInY = true;
                }
                else
                {
                    pointOnY = true;
                }
            }
            else
            {
                pointMinY = true;
            }
        }
        else if (0.0f < lineVec.GetY())
        {
            if (lineStartPos.GetY() <= pointPos.GetY())
            {
                if (pointPos.GetY() <= lineEndPos.GetY())
                {
                    pointInY = true;
                }
                else
                {
                    pointOnY = true;
                }
            }
            else
            {
                pointMinY = true;
            }
        }
        else
        {
            if (EqualInRangeF_VECTOR_cpp(lineStartPos.GetY(), pointPos.GetY(), VECTOR_EQUAL_RANGE))
            {
                pointInY = true;
                pointOnY = true;
                pointMinY = true;
            }
        }

        // 線分のz成分の増加方向を確認して処理を変える
        if (lineVec.GetZ() < 0.0f)
        {
            if (pointPos.GetZ() <= lineStartPos.GetZ())
            {
                if (lineEndPos.GetZ() <= pointPos.GetZ())
                {
                    pointInZ = true;
                }
                else
                {
                    pointOnZ = true;
                }
            }
            else
            {
                pointMinZ = true;
            }
        }
        else if (0.0f < lineVec.GetZ())
        {
            if (lineStartPos.GetZ() <= pointPos.GetZ())
            {
                if (pointPos.GetZ() <= lineEndPos.GetZ())
                {
                    pointInZ = true;
                }
                else
                {
                    pointOnZ = true;
                }
            }
            else
            {
                pointMinZ = true;
            }
        }
        else
        {
            if (EqualInRangeF_VECTOR_cpp(lineStartPos.GetZ(), pointPos.GetZ(), VECTOR_EQUAL_RANGE))
            {
                pointInZ = true;
                pointOnZ = true;
                pointMinZ = true;
            }
        }

        // 
        if (pointInX == true &&
            pointInY == true &&
            pointInZ == true)
        {
            returnValue = 0;
        }
        else if (
            pointOnX == true &&
            pointOnY == true &&
            pointOnZ == true)
        {
            returnValue = 1;
        }
        else if (
            pointMinX == true &&
            pointMinY == true &&
            pointMinZ == true)
        {
            returnValue = -1;
        }
    }

    return returnValue;
}

// 二つの線分の最短距離を取得する関数(2本の線分の最近点c1, c2は出力先)。
float VECTOR3D::GetSegmentToSegmentNewrPos(
    const VECTOR3D &aStart, const VECTOR3D &aEnd,
    const VECTOR3D &bStart, const VECTOR3D &bEnd,
    VECTOR3D &outA, VECTOR3D &outB)
{
    const float EPS = 1e-6f;
    VECTOR3D aVec = aEnd - aStart;
    VECTOR3D dVec = bEnd - bStart;
    VECTOR3D r = aStart - bStart;
    float a = VECTOR3D::Dot(aVec, aVec);
    float e = VECTOR3D::Dot(dVec, dVec);
    float f = VECTOR3D::Dot(dVec, r);

    float s, t;

    if (a <= EPS && e <= EPS)
    {
        // 両方点
        outA = aStart;
        outB = bStart;
        return (outA - outB).Magnitude();
    }

    if (a <= EPS)
    {
        // 線分1が点
        s = 0.0f;
        t = GetClamp01_VECTOR_cpp(f / e);
    }
    else
    {
        float c = VECTOR3D::Dot(aVec, r);
        if (e <= EPS)
        {
            // 線分2が点
            t = 0.0f;
            s = GetClamp01_VECTOR_cpp(-c / a);
        }
        else
        {
            float b = VECTOR3D::Dot(aVec, dVec);
            float denom = a * e - b * b;
            if (denom != 0.0f)
            {
                s = GetClamp01_VECTOR_cpp((b * f - c * e) / denom);
            }
            else
            {
                s = 0.0f;
            }
            t = (b * s + f) / e;
            if (t < 0.0f)
            {
                t = 0.0f;
                s = GetClamp01_VECTOR_cpp(-c / a);
            }
            else if (t > 1.0f)
            {
                t = 1.0f;
                s = GetClamp01_VECTOR_cpp((b - c) / a);
            }
        }
    }

    outA = aStart + aVec * s;
    outB = bStart + dVec * t;
    return (outA - outB).Magnitude();
}

// 線分と三角形の最短距離と最近点を取得する関数
float VECTOR3D::GetSegmentToTriangleNewrPos(
    const VECTOR3D &segStart, const VECTOR3D &segEnd,
    const VECTOR3D &vertex0, const VECTOR3D &vertex1, const VECTOR3D &vertex2,
    VECTOR3D &outOnSeg, VECTOR3D &outOnTri)
{
    VECTOR3D tempSeg, tempTri;
    float minDistance = FLT_MAX;

    // 三角形の各辺に対して線分-線分距離を計算
    float distance = GetSegmentToSegmentNewrPos(segStart, segEnd, vertex0, vertex1, tempSeg, tempTri);
    if (distance < minDistance)
    {
        minDistance = distance;
        outOnSeg = tempSeg;
        outOnTri = tempTri;
    }

    distance = GetSegmentToSegmentNewrPos(segStart, segEnd, vertex1, vertex2, tempSeg, tempTri);
    if (distance < minDistance)
    {
        minDistance = distance;
        outOnSeg = tempSeg;
        outOnTri = tempTri;
    }

    distance = GetSegmentToSegmentNewrPos(segStart, segEnd, vertex2, vertex0, tempSeg, tempTri);
    if (distance < minDistance)
    {
        minDistance = distance;
        outOnSeg = tempSeg;
        outOnTri = tempTri;
    }

    // 三角形平面上に線分が貫通している場合
    VECTOR3D norm = (VECTOR3D::Cross(vertex1 - vertex0, vertex2 - vertex0)).Normalize();
    VECTOR3D dir = segEnd - segStart;
    float denom = VECTOR3D::Dot(norm, dir);

    if (fabs(denom) > 1e-6f)
    {
        float t = VECTOR3D::Dot(vertex0 - segStart, norm) / denom;
        if (t >= 0.0f && t <= 1.0f)
        {
            VECTOR3D p = segStart + dir * t;
            VECTOR3D c0 = VECTOR3D::Cross(vertex1 - vertex0, p - vertex0);
            VECTOR3D c1 = VECTOR3D::Cross(vertex2 - vertex1, p - vertex1);
            VECTOR3D c2 = VECTOR3D::Cross(vertex0 - vertex2, p - vertex2);
            if (VECTOR3D::Dot(norm, c0) >= 0 && VECTOR3D::Dot(norm, c1) >= 0 && VECTOR3D::Dot(norm, c2) >= 0)
            {
                outOnSeg = p;
                outOnTri = p;
                minDistance = 0.0f;
            }
        }
    }

    return minDistance;
}

// AABBの中に点が入っているかを確認する関数
bool VECTOR3D::CheckABB3DToPoint(const VECTOR3D &aabbMin, const VECTOR3D &aabbMax, const VECTOR3D point)
{
    return (
        aabbMin.mfParamX <= point.mfParamX && point.mfParamX <= aabbMax.mfParamX &&
        aabbMin.mfParamY <= point.mfParamY && point.mfParamY <= aabbMax.mfParamY &&
        aabbMin.mfParamZ <= point.mfParamZ && point.mfParamZ <= aabbMax.mfParamZ);
}

void VECTOR3D::SetX(float x)
{
    this->mfParamX = x;
}

void VECTOR3D::SetY(float y)
{
    this->mfParamY = y;
}

void VECTOR3D::SetZ(float z)
{
    this->mfParamZ = z;
}

void VECTOR3D::SetXYZ(float x, float y, float z)
{
    this->mfParamX = x;
    this->mfParamY = y;
    this->mfParamZ = z;
}

float VECTOR3D::GetX() const
{
    return this->mfParamX;
}

float VECTOR3D::GetY() const
{
    return this->mfParamY;
}

float VECTOR3D::GetZ() const
{
    return this->mfParamZ;
}

VECTOR3D VECTOR3D::operator +() const
{
    return *this;
}

VECTOR3D VECTOR3D::operator -() const
{
    return VECTOR3D(-this->mfParamX, -this->mfParamY, -this->mfParamZ);
}

void VECTOR3D::operator =(const VECTOR3D &mat)
{
    this->mfParamX = mat.mfParamX;
    this->mfParamY = mat.mfParamY;
    this->mfParamZ = mat.mfParamZ;
}

void VECTOR3D::operator +=(const VECTOR3D &mat)
{
    this->mfParamX += mat.mfParamX;
    this->mfParamY += mat.mfParamY;
    this->mfParamZ += mat.mfParamZ;
}

void VECTOR3D::operator -=(const VECTOR3D &mat)
{
    this->mfParamX -= mat.mfParamX;
    this->mfParamY -= mat.mfParamY;
    this->mfParamZ -= mat.mfParamZ;
}

void VECTOR3D::operator *=(const float &scaler)
{
    this->mfParamX *= scaler;
    this->mfParamY *= scaler;
    this->mfParamZ *= scaler;
}

void VECTOR3D::operator /=(const float &scaler)
{
    if (EqualInRangeF_VECTOR_cpp(scaler, 0.0f, 1e-6f))
    {
        this->mfParamX = 0;
        this->mfParamY = 0;
        this->mfParamZ = 0;

    }
    else
    {
        this->mfParamX /= scaler;
        this->mfParamY /= scaler;
        this->mfParamZ /= scaler;
    }
}

#ifdef USE_MY_VECTOR2D
// VECTOR3DからVECOTR2Dに変換する関数
VECTOR2D VECTOR3D::To2D(char toX, char toY) const
{
    VECTOR2D temp(0.0f, 0.0f);
    float x = 0.0f;
    float y = 0.0f;
    switch (toX)
    {
    case 'x':
        x = this->mfParamX;
        break;
    case 'y':
        x = this->mfParamY;
        break;
    case 'z':
        x = this->mfParamZ;
        break;
    case 'X':
        x = this->mfParamX;
        break;
    case 'Y':
        x = this->mfParamY;
        break;
    case 'Z':
        x = this->mfParamZ;
        break;
    default:
        x = this->mfParamX;
        break;
    }
    switch (toY)
    {
    case 'x':
        y = this->mfParamX;
        break;
    case 'y':
        y = this->mfParamY;
        break;
    case 'z':
        y = this->mfParamZ;
        break;
    case 'X':
        y = this->mfParamX;
        break;
    case 'Y':
        y = this->mfParamY;
        break;
    case 'Z':
        y = this->mfParamZ;
        break;
    default:
        y = this->mfParamY;
        break;
    }

    temp.SetXY(x, y);
    return temp;
}
#endif // USE_MY_VECTOR3D

#ifdef USE_MY_MATRIX
// VECTOR3Dを計算できる行列に変換する関数
MyMatrix VECTOR3D::ToMat() const
{
    MyMatrix temp(1, 4);
    temp.SetParam(0, 0, this->mfParamX);
    temp.SetParam(0, 1, this->mfParamY);
    temp.SetParam(0, 2, this->mfParamZ);
    temp.SetParam(0, 3, 1);
    return temp;
}

// 行列を用いてベクトルの拡大・縮小(Scaling)、回転(Rotation)、平行移動(Transportion)を行う関数
VECTOR3D VECTOR3D::MatrixOperation(const VECTOR3D &s, const VECTOR3D &r, const VECTOR3D &t) const
{
    return (*this * MyMatrix::GetScaling3D(s)
        * MyMatrix::GetRotation3DZ(r.GetZ())
        * MyMatrix::GetRotation3DX(r.GetX())
        * MyMatrix::GetRotation3DY(r.GetY())
        * MyMatrix::GetTransportion3D(t)).To3VD();
}

// BaseLine(一つの軸の成分のみ1の単位ベクトル)をtargetと同じベクトルにする変換行列を取得する関数
// 四つ目の引数はこの関数内で計算してほしい場合0.0f以下の数字を入力してください
MyMatrix VECTOR3D::GetBaseVecToTarget(const VECTOR3D &baseVec, const VECTOR3D &targetStart, const VECTOR3D &targetEnd, float distance)
{
    // BaseVecがおかしい場合は処理を終了する
    if (baseVec != VECTOR3D::GetLeft() &&
        baseVec != VECTOR3D::GetRight() &&
        baseVec != VECTOR3D::GetUp() &&
        baseVec != VECTOR3D::GetDown() &&
        baseVec != VECTOR3D::GetFront() &&
        baseVec != VECTOR3D::GetBack())
    {
        return MyMatrix::GetDeactivate();
    }

    // 4行4列の単位行列を取得
    MyMatrix mat = MyMatrix::GetIdentity(4);

    // 作業用変数
    VECTOR3D start2end = (targetEnd - targetStart);

    // 基準線(0.0f, 0.0f, 1.0fのzベクトル)をABベクトルと同じベクトルにするコード
    if (distance <= 0.0f)
    {
        // 大きさを改めて取得する
        distance = start2end.Magnitude();
    }

    // 拡縮行列の成分を入れる
    char oneAxis = '\0';
    float xScale = 1.0f;
    float yScale = 1.0f;
    float zScale = 1.0f;
    if (EqualInRangeF_VECTOR_cpp(baseVec.GetY(), 0.0f, VECTOR_EQUAL_RANGE) == false)
    {
        xScale = distance;
        oneAxis = 'x';
    }
    else if (EqualInRangeF_VECTOR_cpp(baseVec.GetY(), 0.0f, VECTOR_EQUAL_RANGE) == false)
    {
        yScale = distance;
        oneAxis = 'y';
    }
    else
    {
        zScale = distance;
        oneAxis = 'z';
    }

    mat *= MyMatrix::GetScaling3D(xScale, yScale, zScale);

    // 発生源に近い座標から現在に近い座標への距離ベクトルを取得
    VECTOR3D start2endNorm = start2end / distance;

    // 二つのベクトルの角度を確認する(互いに単位ベクトルだからクランプの必要はないと思うけど念のためクランプもしておく)
    float dot = VECTOR3D::Dot(baseVec, start2endNorm);
    if (dot < -1.0f)
    {
        dot = -1.0f;
    }
    else if (1.0f < dot)
    {
        dot = 1.0f;
    }

    // 回転行列が必要ない場合はtrueになる
    bool rotateFlag = true;

    // 回転軸を取得する
    VECTOR3D crossVec = VECTOR3D::Cross(baseVec, start2endNorm);
    if (crossVec == VECTOR3D::GetZero())
    {
        if (0.9999f < dot)
        {
            // 二つのベクトルがほとんど同じ方向なら回転行列が単位行列になる
            // つまり何もする必要がないのでフラグもfalseに変更する
            rotateFlag = false;
        }
        else if (fabsf(dot + 1.0f) < 1e-6f)
        {
            // 二つのベクトルがほとんど逆方向なら任意の軸を逆方向にしないといけないので
            switch (oneAxis)
            {
            case 'x':
                crossVec = VECTOR3D(0.0f, 1.0f, 0.0f);
                break;
            case 'y':
                crossVec = VECTOR3D(0.0f, 0.0f, 1.0f);
                break;
            case 'z':
                crossVec = VECTOR3D(1.0f, 0.0f, 0.0f);
                break;
            default:
                break;
            }
        }
    }

    // 回転行列をかける場合は処理を行う
    if (rotateFlag)
    {
        // 比から角度を取得するacosf()を使う
        float angle = acosf(dot);

        // ワールド座標に回転行列を入れる
        mat *= MyMatrix::GetRotationQuaternion3D(crossVec, angle);
    }

    // 移動行列をかける
    mat *= MyMatrix::GetTransportion3D(targetStart);

    // 成功したことを返す
    return mat;
}

#endif // USE_MY_MATRIX

VECTOR3D operator +(const VECTOR3D &vecL, const VECTOR3D &vecR)
{
    return VECTOR3D(vecL.GetX() + vecR.GetX(), vecL.GetY() + vecR.GetY(), vecL.GetZ() + vecR.GetZ());
}

VECTOR3D operator -(const VECTOR3D &vecL, const VECTOR3D &vecR)
{
    return VECTOR3D(vecL.GetX() - vecR.GetX(), vecL.GetY() - vecR.GetY(), vecL.GetZ() - vecR.GetZ());
}

VECTOR3D operator*(const VECTOR3D &vecL, float scaler)
{
    return VECTOR3D(vecL.GetX() * scaler, vecL.GetY() * scaler, vecL.GetZ() * scaler);
}

VECTOR3D operator /(const VECTOR3D &vecL, float scaler)
{
    VECTOR3D temp;
    if (EqualInRangeF_VECTOR_cpp(scaler, 0.0f, 1e-6f))
    {
        temp.SetX(0);
        temp.SetY(0);
        temp.SetZ(0);
    }
    else
    {
        temp.SetX(vecL.GetX() / scaler);
        temp.SetY(vecL.GetY() / scaler);
        temp.SetZ(vecL.GetZ() / scaler);
    }
    return temp;
}

bool operator ==(const VECTOR3D &vecL, const VECTOR3D &vecR)
{
    return vecL.EqualInRange(vecL, vecR, VECTOR_EQUAL_RANGE);
}

bool operator !=(const VECTOR3D &vecL, const VECTOR3D &vecR)
{
    return !(vecL.EqualInRange(vecL, vecR, VECTOR_EQUAL_RANGE));
}

#ifdef USE_MY_MATRIX
MyMatrix operator *(const VECTOR3D &vec, const MyMatrix &mat)
{
    MyMatrix vecMat = vec.ToMat();
    return vecMat * mat;
}

#endif // USE_MY_MATRIX
#endif // USE_MY_VECTOR3D
