#include "MyQuaternion.h"

#ifdef USE_MY_ROTATION

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef USE_MY_VECTOR3D
#include "VECTOR.h"
#endif // USE_MY_VECTOR3D

#ifdef USE_MY_MATRIX
#include "MyMatrix.h"
#endif // USE_MY_MATRIX

#ifdef USE_MY_EULERANGLE
#include "MyEulerAngle.h"
#endif // USE_MY_EULERANGLE

#include "MyFunctions.h"

#ifdef USE_MY_VECTOR3D
#ifdef USE_MY_QUATERNION
// コンストラクタ
MyQuaternion::MyQuaternion() :
    mfParamX(0.0f),
    mfParamY(0.0f),
    mfParamZ(0.0f),
    mfParamW(1.0f)
{

}

MyQuaternion::MyQuaternion(float x, float y, float z, float w) :
    mfParamX(x),
    mfParamY(y),
    mfParamZ(z),
    mfParamW(w)
{

}

// デストラクタ
MyQuaternion::~MyQuaternion()
{

}

// 自身の逆クォータニオンを取得する関数
MyQuaternion MyQuaternion::GetInverse() const
{
    return MyQuaternion(-this->mfParamX, -this->mfParamY, -this->mfParamZ, this->mfParamW);
}

// 自身に正規化を行ったものを新たに取得する関数
MyQuaternion MyQuaternion::Normalize() const
{
    float num = sqrtf(MyQuaternion::GetDot(*this, *this));
    if (num < 0.000000001f)
    {
        return MyQuaternion::GetIdentity();
    }
    return MyQuaternion(
        this->mfParamX / num,
        this->mfParamY / num,
        this->mfParamZ / num,
        this->mfParamW / num);
}

// 回転を座標に対する角度の値(AngleAxis)に変換する
// 返り値であるMyQuaternionの中身はx,y,z,までが座標、wは角度の値です
MyQuaternion MyQuaternion::GetAngleAndAxis() const
{
    // クォータニオンの定義, cos(rad/2) = w より
    float radian = acosf(this->mfParamW) * 2.0f;

    // x = axis.x * sin(rad/2), 
    // y = axis.y * sin(rad/2), 
    // z = axis.z * sin(rad/2) より
    VECTOR3D axis(
        this->mfParamX / sinf(radian / 2),
        this->mfParamY / sinf(radian / 2),
        this->mfParamZ / sinf(radian / 2));

    return MyQuaternion(axis.GetX(), axis.GetY(), axis.GetZ(), radian);
}

// 自身の値をオイラー角に直して返す関数
VECTOR3D MyQuaternion::GetEulerRadian() const
{
    // x/y/z軸の回転量
    float angle_x = 0;
    float angle_y = 0;
    float angle_z = 0;

    // 角度が90度のときの誤差を補正する
    float theta_x = (2 * (mfParamY * mfParamZ + mfParamX * mfParamW));
    if (0.9999999f <= theta_x)
    {
        theta_x = 1.0f;
    }

    angle_x = asinf(theta_x);
    float cosAngleX = cosf(angle_x);
    if (cosAngleX != 0)
    {
        float y_top = 2 * (mfParamX * mfParamZ - mfParamY * mfParamW);
        float y_bottom = 2 * (mfParamW * mfParamW + mfParamZ * mfParamZ) - 1;
        angle_y = atan2f(-y_top, y_bottom);

        float z_top = 2 * (mfParamX * mfParamY - mfParamZ * mfParamW);
        float z_bottom = 2 * (mfParamW * mfParamW + mfParamY * mfParamY) - 1;
        angle_z = atan2f(-z_top, z_bottom);
    }
    else
    {
        angle_y = 0;

        float z_top = 2 * (mfParamX * mfParamY + mfParamZ * mfParamW);
        float z_bottom = 2 * (mfParamW * mfParamW + mfParamX * mfParamX) - 1;
        angle_z = atan2f(z_top, z_bottom);
    }

    return VECTOR3D(angle_x, angle_y, angle_z);
}

// オイラー角から自身の値を設定し直す関数。
// ※x軸デグリー90°に加えて他の軸にさらに値が入ってしまうとジンバルロックが発生します。
void MyQuaternion::SetEulerRadian(const VECTOR3D &eulerRadian)
{
    *this = MyQuaternion::GetQtnEuler(eulerRadian);
}

// fromからtoへの回転を作成して代入する関数
void MyQuaternion::SetFromToRotation(const VECTOR3D &from, const VECTOR3D &to)
{
    *this = GetQtnFromToRotation(from, to);
}

// 指定した forward と upwards 方向に回転する関数
void MyQuaternion::SetLookRotation(VECTOR3D view)
{
    *this = GetQtnLookRotation(view);
}

float MyQuaternion::GetX() const
{
    return this->mfParamX;
}

float MyQuaternion::GetY() const
{
    return this->mfParamY;
}

float MyQuaternion::GetZ() const
{
    return this->mfParamZ;
}

float MyQuaternion::GetW() const
{
    return this->mfParamW;
}

// 二つのクォータニオンの内積を取得する
float MyQuaternion::GetDot(const MyQuaternion &qtnL, const MyQuaternion &qtnR)
{
    return qtnL.GetX() * qtnR.GetX() + qtnL.GetY() * qtnR.GetY() + qtnL.GetZ() * qtnR.GetZ() + qtnL.GetW() * qtnR.GetW();
}

//  2つの回転a、b間の角度を返す
float MyQuaternion::GetAngle(const MyQuaternion &qtnA, const MyQuaternion &qtnB)
{
    // a・b = cos(θ/2) となることを利用する
    float a = fabsf(MyQuaternion::GetDot(qtnA, qtnB));
    float dot = (a < 1.0f) ? a : 1.0f;
    return (0.999999f < dot) ? 0.0f : (acosf(dot) * 2.0f);
}

// aとbの間をtで補間して正規化する関数。
// rateは 0～1 の範囲のクランプされる
MyQuaternion MyQuaternion::Lerp(const MyQuaternion &a, const MyQuaternion &b, float rate)
{
    // rateの範囲の制限
    rate = MyFunctions::GetClamp01(rate);

    // 線形補間の本体を行う
    return LerpUnclamped(a, b, rate);
}

// aとbの間をrateで補間して正規化する関数。
// rateは 0～1 の範囲にクランプされない
MyQuaternion MyQuaternion::LerpUnclamped(const MyQuaternion &a, const MyQuaternion &b, float rate)
{
    // 2つの回転の内積を求める
    float dot = MyQuaternion::GetDot(a, b);
    MyQuaternion temp = b;

    // 内積が負の時、最短距離での補間を得るために片方を負にする
    if (dot < 0)
    {
        temp = MyQuaternion(-b.mfParamX, -b.mfParamY, -b.mfParamZ, -b.mfParamW);
        dot = -dot;
    }

    // 直線経路で補完する a*(1-t) + b*t
    MyQuaternion lerp(
        a.mfParamX + (-a.mfParamX + b.mfParamX) * rate,
        a.mfParamY + (-a.mfParamY + b.mfParamY) * rate,
        a.mfParamZ + (-a.mfParamZ + b.mfParamZ) * rate,
        a.mfParamW + (-a.mfParamW + b.mfParamW) * rate
    );

    return lerp.Normalize();
}

// 球面線形補完を得る関数。rateは 0～1 の範囲にクランプされる
MyQuaternion MyQuaternion::Slerp(const MyQuaternion &a, const MyQuaternion &b, float rate)
{
    // tの範囲の制限
    rate = MyFunctions::GetClamp01(rate);
    return SlerpUnclamped(a, b, rate);
}

// 球面線形補完を得る関数。rateは 0～1 の範囲にクランプされない
MyQuaternion MyQuaternion::SlerpUnclamped(const MyQuaternion &a, const MyQuaternion &b, float rate)
{
    // 2つの回転の内積を求める
    float dot = MyQuaternion::GetDot(a, b);
    MyQuaternion temp = b;

    // 内積が負の時、最短距離での補間を得るために
    // 片方の回転を負にして、内積を正の値にする
    if (dot < 0)
    {
        temp = MyQuaternion(-b.mfParamX, -b.mfParamY, -b.mfParamZ, -b.mfParamW);
        dot = -dot;
    }

    // 2つの回転の角度を求める
    float rad = (0.999999f < dot) ? 0.0f : acosf(dot);

    // 球面線形補完
    float bottom = sinf(rad);
    float a_rate = sinf((1 - rate) * rad) / bottom;
    float b_rate = sinf(rate * rad) / bottom;

    MyQuaternion slerp(
        a.mfParamX * a_rate + b.mfParamX * b_rate,
        a.mfParamY * a_rate + b.mfParamY * b_rate,
        a.mfParamZ * a_rate + b.mfParamZ * b_rate,
        a.mfParamW * a_rate + b.mfParamW * b_rate
    );

    return slerp.Normalize();
}

// 単位クォータニオンを取得する関数
MyQuaternion MyQuaternion::GetIdentity()
{
    return MyQuaternion(0, 0, 0, 1);
}

// 軸となるベクトルと回転量から新しいクォータニオンを取得する関数
MyQuaternion MyQuaternion::GetQtnAixs(const VECTOR3D &axis, float radian)
{
    VECTOR3D normAxis = axis.Normalize();
    MyQuaternion temp(
        normAxis.GetX() * sinf(radian / 2),
        normAxis.GetY() * sinf(radian / 2),
        normAxis.GetZ() * sinf(radian / 2),
        cosf(radian / 2));
    return temp;
}

// x軸, y軸, z軸の回転量から新しいクォータニオンを取得する関数
MyQuaternion MyQuaternion::GetQtnEuler(const VECTOR3D &eulerAxisRadian)
{
    MyQuaternion xQtn = MyQuaternion::GetQtnAixs(VECTOR3D(1.0f, 0.0f, 0.0f), eulerAxisRadian.GetX());
    MyQuaternion yQtn = MyQuaternion::GetQtnAixs(VECTOR3D(0.0f, 1.0f, 0.0f), eulerAxisRadian.GetY());
    MyQuaternion zQtn = MyQuaternion::GetQtnAixs(VECTOR3D(0.0f, 0.0f, 1.0f), eulerAxisRadian.GetZ());
    return zQtn * xQtn * yQtn;
}

// fromDirection から toDirection への回転を作成して返す関数
MyQuaternion MyQuaternion::GetQtnFromToRotation(const VECTOR3D &fromDirection, const VECTOR3D &toDirection)
{
    // 外積を用いて、軸ベクトルを求める
    VECTOR3D axis = VECTOR3D::Cross(fromDirection, toDirection);

    // 外積が(0,0,0)の時は、無回転のクォータニオン(0,0,0,1)にする
    if (VECTOR3D::EqualInRange(axis, VECTOR3D::GetZero(), 0.00001f))
    {
        return MyQuaternion::GetIdentity();
    }

    // 内積の定義から回転量を求める a・b = |a||b|cosθ なので
    float rad = acosf(VECTOR3D::Dot(fromDirection, toDirection) / (fromDirection.Magnitude() * toDirection.Magnitude()));

    // 求めた軸と回転量でクォータニオンの生成
    return GetQtnAixs(axis, rad);
}

// 引数のupwardsを省略した場合に、Vector3.upがデフォルト引数となるようにオーバーロード
MyQuaternion MyQuaternion::GetQtnLookRotation(const VECTOR3D &forward)
{
    return GetQtnLookRotation(forward, VECTOR3D(0.0f, 1.0f, 0.0f));
}

// オブジェクトの正面(forward)を引数のforwardの向きに回転させる回転を生成する
MyQuaternion MyQuaternion::GetQtnLookRotation(const VECTOR3D &forward, const VECTOR3D &upwards)
{
    // オブジェクトの正面からforwardに向ける回転を取得
    MyQuaternion lookRotation = GetQtnFromToRotation(VECTOR3D(0.0f, 0.0f, 1.0f), forward);

    // 外積を用いてupwardsとforwardに垂直なベクトル(赤軸)を得る
    VECTOR3D xAxisHorizontal = VECTOR3D::Cross(upwards, forward);

    // 回転後のy軸(緑軸)を求める
    VECTOR3D yAxisAfterRotate = VECTOR3D::Cross(forward, xAxisHorizontal);

    // Look後のy軸(緑) から 回転後のy軸(緑) へ修正する回転を求める
    VECTOR3D yAxisBeforeModify = lookRotation * VECTOR3D(0.0f, 1.0f, 0.0f);
    MyQuaternion modifyRotation = MyQuaternion::GetQtnFromToRotation(yAxisBeforeModify, yAxisAfterRotate);

    // 回転を合成して返す
    return modifyRotation * lookRotation;
}

#ifdef USE_MY_MATRIX
MyMatrix MyQuaternion::ToMat() const
{
    MyMatrix temp = MyMatrix::GetIdentity(4);
    float xx2 = this->mfParamX * this->mfParamX * 2;
    float xy2 = this->mfParamX * this->mfParamY * 2;
    float xz2 = this->mfParamX * this->mfParamZ * 2;
    float xw2 = this->mfParamX * this->mfParamW * 2;
    float yy2 = this->mfParamY * this->mfParamY * 2;
    float yz2 = this->mfParamY * this->mfParamZ * 2;
    float yw2 = this->mfParamY * this->mfParamW * 2;
    float zz2 = this->mfParamZ * this->mfParamZ * 2;
    float zw2 = this->mfParamZ * this->mfParamW * 2;
    float ww2 = this->mfParamW * this->mfParamW * 2;
    /*
    temp.SetParam(0, 0, ww2 + 2 * this->mfParamX * this->mfParamX - 1);
    temp.SetParam(0, 1, xy2 + zw2);
    temp.SetParam(0, 2, xz2 - yw2);
    temp.SetParam(1, 0, xy2 - zw2);
    temp.SetParam(1, 1, ww2 + 2 * this->mfParamY * this->mfParamY - 1);
    temp.SetParam(1, 2, yz2 + xw2);
    temp.SetParam(2, 0, xz2 + yw2);
    temp.SetParam(2, 1, yz2 - xw2);
    temp.SetParam(2, 2, ww2 + 2 * this->mfParamZ * this->mfParamZ - 1);
    */

    temp.SetParam(0, 0, 1 - yy2 - zz2);
    temp.SetParam(0, 1, xy2 + zw2);
    temp.SetParam(0, 2, xz2 - yw2);
    temp.SetParam(1, 0, xy2 - zw2);
    temp.SetParam(1, 1, 1 - xx2 - zz2);
    temp.SetParam(1, 2, yz2 + xw2);
    temp.SetParam(2, 0, xz2 + yw2);
    temp.SetParam(2, 1, yz2 - xw2);
    temp.SetParam(2, 2, 1 - xx2 - yy2);

    return temp;
}
#endif // USE_MY_MATRIX

#ifdef USE_MY_EULERANGLE
MyEulerAngle MyQuaternion::ToEulerAngle() const
{
    return this->ToEulerAngle(globalOrder);
}

MyEulerAngle MyQuaternion::ToEulerAngle(EulerOrder order) const
{
    MyEulerAngle temp(order, 0.0f, 0.0f, 0.0f);
    if (order == EulerOrder::XYZ)
    {
        float sy = 2 * this->mfParamX * this->mfParamZ + 2 * this->mfParamY * this->mfParamW;
        bool unlocked = (fabsf(sy) < 0.99999f);
        temp = MyEulerAngle(
            order,
            unlocked ? atan2f(-(2 * this->mfParamY * this->mfParamZ - 2 * this->mfParamX * this->mfParamX), 2 * this->mfParamW * this->mfParamW + 2 * this->mfParamZ * this->mfParamZ - 1) 
            : atan2f(2 * this->mfParamY * this->mfParamZ + 2 * this->mfParamX * this->mfParamW, 2 * this->mfParamW * this->mfParamW + 2 * this->mfParamY * this->mfParamY - 1),
            asinf(sy),
            unlocked ? atan2f(-(2 * this->mfParamX * this->mfParamY - 2 * this->mfParamZ * this->mfParamW), 2 * this->mfParamW * this->mfParamW + 2 * this->mfParamX * this->mfParamX - 1) : 0);
    }
    else if (order == EulerOrder::XZY)
    {
        float sz = -(2 * this->mfParamX * this->mfParamY - 2 * this->mfParamZ * this->mfParamW);
        bool unlocked = (fabsf(sz) < 0.99999f);
        temp = MyEulerAngle(
            order,
            unlocked ? atan2f(2 * this->mfParamY * this->mfParamZ + 2 * this->mfParamX * this->mfParamW, 2 * this->mfParamW * this->mfParamW + 2 * this->mfParamY * this->mfParamY - 1)
            : atan2f(-(2 * this->mfParamY * this->mfParamZ - 2 * this->mfParamX * this->mfParamW), 2 * this->mfParamW * this->mfParamW + 2 * this->mfParamZ * this->mfParamZ - 1),
            unlocked ? atan2f(2 * mfParamX * mfParamZ + 2 * mfParamY * mfParamW, 2 * mfParamW * mfParamW + 2 * mfParamX * mfParamX - 1) : 0,
            asinf(sz));
    }
    else if (order == EulerOrder::YXZ)
    {
        float sx = -(2 * mfParamY * mfParamZ - 2 * mfParamX * mfParamW);
        bool unlocked = (fabsf(sx) < 0.99999f);
        temp = MyEulerAngle(
            order,
            asinf(sx),
            unlocked ? atan2f(2 * mfParamX * mfParamZ + 2 * mfParamY * mfParamW, 2 * mfParamW * mfParamW + 2 * mfParamZ * mfParamZ - 1)
            : atan2f(-(2 * mfParamX * mfParamZ - 2 * mfParamY * mfParamW), 2 * mfParamW * mfParamW + 2 * mfParamX * mfParamX - 1),
            unlocked ? atan2f(2 * mfParamX * mfParamY + 2 * mfParamZ * mfParamW, 2 * mfParamW * mfParamW + 2 * mfParamY * mfParamY - 1) : 0);
    }
    else if (order == EulerOrder::YZX)
    {
        float sz = 2 * mfParamX * mfParamY + 2 * mfParamZ * mfParamW;
        bool unlocked = (fabsf(sz) < 0.99999f);
        temp = MyEulerAngle(
            order,
            unlocked ? atan2f(-(2 * mfParamY * mfParamZ - 2 * mfParamX * mfParamW), 2 * mfParamW * mfParamW + 2 * mfParamY * mfParamY - 1) : 0,
            unlocked ? atan2f(-(2 * mfParamX * mfParamZ - 2 * mfParamY * mfParamW), 2 * mfParamW * mfParamW + 2 * mfParamX * mfParamX - 1)
            : atan2f(2 * mfParamX * mfParamZ + 2 * mfParamY * mfParamW, 2 * mfParamW * mfParamW + 2 * mfParamZ * mfParamZ - 1),
            asinf(sz));
    }
    else if (order == EulerOrder::ZXY)
    {
        float sx = 2 * mfParamY * mfParamZ + 2 * mfParamX * mfParamW;
        bool unlocked = (fabsf(sx) < 0.99999f);
        temp = MyEulerAngle(
            order,
            asinf(sx),
            unlocked ? atan2f(-(2 * mfParamX * mfParamZ - 2 * mfParamY * mfParamW), 2 * mfParamW * mfParamW + 2 * mfParamZ * mfParamZ - 1) : 0,
            unlocked ? atan2f(-(2 * mfParamX * mfParamY - 2 * mfParamZ * mfParamW), 2 * mfParamW * mfParamW + 2 * mfParamY * mfParamY - 1)
            : atan2f(2 * mfParamX * mfParamY + 2 * mfParamZ * mfParamW, 2 * mfParamW * mfParamW + 2 * mfParamX * mfParamX - 1));
    }
    else if (order == EulerOrder::ZYX)
    {
        float sy = -(2 * mfParamX * mfParamZ - 2 * mfParamY * mfParamW);
        bool unlocked = (fabsf(sy) < 0.99999f);
        temp = MyEulerAngle(
            order,
            unlocked ? atan2f(2 * mfParamY * mfParamZ + 2 * mfParamX * mfParamW, 2 * mfParamW * mfParamW + 2 * mfParamZ * mfParamZ - 1) : 0,
            asinf(sy),
            unlocked ? atan2f(2 * mfParamX * mfParamY + 2 * mfParamZ * mfParamW, 2 * mfParamW * mfParamW + 2 * mfParamX * mfParamX - 1)
            : atan2f(-(2 * mfParamX * mfParamY - 2 * mfParamZ * mfParamW), 2 * mfParamW * mfParamW + 2 * mfParamY * mfParamY - 1));
    }
    return temp;
}
#endif // USE_MY_EULERANGLE

MyQuaternion operator *(const MyQuaternion &qtnL, const MyQuaternion &qtnR)
{
    MyQuaternion temp(
        qtnL.GetW() * qtnR.GetX() - qtnL.GetZ() * qtnR.GetY() + qtnL.GetY() * qtnR.GetZ() + qtnL.GetX() * qtnR.GetW(),
        qtnL.GetZ() * qtnR.GetX() + qtnL.GetW() * qtnR.GetY() - qtnL.GetX() * qtnR.GetZ() + qtnL.GetY() * qtnR.GetW(),
        qtnL.GetY() * qtnR.GetX() + qtnL.GetX() * qtnR.GetY() + qtnL.GetW() * qtnR.GetZ() + qtnL.GetZ() * qtnR.GetW(),
        qtnL.GetX() * qtnR.GetX() - qtnL.GetY() * qtnR.GetY() - qtnL.GetZ() * qtnR.GetZ() + qtnL.GetW() * qtnR.GetW()
    );
    return temp;
}

VECTOR3D operator * (const MyQuaternion &qtn, const VECTOR3D &vec)
{
    // 回転後のベクトルv'について
    // v' = q * v * q_inverse となる
    // 3次元座標をクォータニオンで表す
    MyQuaternion vecQuaternion(vec.GetX(), vec.GetY(), vec.GetZ(), 0);

    // ベクトルに回転を施す
    MyQuaternion vecAfterRotate = qtn * vecQuaternion * qtn.GetInverse();

    // クォータニオンを3次元座標に変換する
    return VECTOR3D(vecAfterRotate.GetX(), vecAfterRotate.GetY(), vecAfterRotate.GetZ());
}

#endif // USE_MY_QUATERNION
#endif // USE_MY_VECTOR3D
#endif // USE_MY_ROTATION