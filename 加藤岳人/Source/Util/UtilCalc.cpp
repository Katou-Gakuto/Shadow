#include <cmath>

#include "DxLib.h"

#include "UtilCalc.h"


// float同士がの誤差範囲内なら「true」を返す
bool UtilCalc::FloatEqual(float src, float dst, float error)
{
    return std::fabs(src - dst) < error;
}

// 方向を3.14から-3.14になるように調整する
float UtilCalc::NotExceedAngle(float angle)
{
    if (angle > Pi) {
        angle -= PiTwo;
    }
    else if (angle < -Pi) {
        angle += PiTwo;
    }

    return angle;
}

// 方向を3.14から-3.14になるように調整する(3方向)
VECTOR UtilCalc::VNotExceedAngle(VECTOR angle, bool radFlag)
{
    // 半径
    float radius = (radFlag ? RadPi : Pi);
    // 直径
    float diameter = (radFlag ? RadPiTwo : PiTwo);

    if (angle.x > radius) {
        angle.x -= diameter;
        while (angle.x > radius)
        {
            angle.x -= diameter;
        }
    }
    else if (angle.x < -radius) {
        angle.x += diameter;
        while (angle.x < -radius)
        {
            angle.x += diameter;
        }
    }

    if (angle.y > radius) {
        angle.y -= diameter;
        while (angle.y > radius)
        {
            angle.y -= diameter;
        }
    }
    else if (angle.y < -radius) {
        angle.y += diameter;
        while (angle.y < -radius)
        {
            angle.y += diameter;
        }
    }

    if (angle.z > radius) {
        angle.z -= diameter;
        while (angle.z > radius)
        {
            angle.z -= diameter;
        }
    }
    else if (angle.z < -radius) {
        angle.z += diameter;
        while (angle.z < -radius)
        {
            angle.z += diameter;
        }
    }

    return angle;
}

// 球面移動した場所
VECTOR UtilCalc::VSphericalMovePos(float size, VECTOR angle)
{
    VECTOR pos = VAngleToVec(angle);
    pos.z = -pos.z;
    // pos.x = size * cosf(NotExceedAngle(angle.x)) * sinf(NotExceedAngle(angle.y));
    // pos.y = size * sinf(NotExceedAngle(angle.x));
    // pos.z = -(size * cosf(NotExceedAngle(angle.x)) * cosf(NotExceedAngle(angle.y)));
    return VScale(pos, size);
}

//　ベクトルをアングルに変換する
VECTOR UtilCalc::VVecToAngle(VECTOR vec)
{
    VECTOR angle;
    angle.x = 0.0f;
    angle.y = atan2f(vec.x, vec.z);
    angle.z = 0.0f;
    return angle;
}

// アングルをベクトルに変換する
VECTOR UtilCalc::VAngleToVec(VECTOR angle)
{
    VECTOR vec;
    vec.x = cosf(NotExceedAngle(angle.x)) * sinf(NotExceedAngle(angle.y));
    vec.y = sinf(NotExceedAngle(angle.x));
    vec.z = cosf(NotExceedAngle(angle.x)) * cosf(NotExceedAngle(angle.y));
    return vec;
}

// 移動量をアングルに反映した値を返す
VECTOR UtilCalc::VMoveVecToAngle(VECTOR moveVec, VECTOR angle, float speed)
{
    float targetAngle = VVecToAngle(moveVec).y;
    float diffAngle = targetAngle - angle.y;
    diffAngle = NotExceedAngle(diffAngle);

    // 角度の差を0に近づける
    if (diffAngle > 0.0f)
    {
        diffAngle -= speed;
        if (diffAngle < 0.0f)
        {
            diffAngle = 0.0f;
        }
    }
    else
    {
        diffAngle += speed;
        if (diffAngle > 0.0f)
        {
            diffAngle = 0.0f;
        }
    }

    return VGet(angle.x, targetAngle - diffAngle, angle.z);
}

// VECTORの平均を出す(正の数)
float UtilCalc::PutVectorAverage(VECTOR vector)
{
    return (-vector.x + vector.y + vector.z) * 0.3333333f;
}

// VECTORの掛け算
VECTOR UtilCalc::VMultiply(VECTOR src, VECTOR dst)
{
    return VGet(src.x * dst.x, src.y * dst.y, src.z * dst.z);
}

// ベクトル同士の差を出す
float UtilCalc::VDiff(VECTOR src, VECTOR dst)
{
    VECTOR result = VSub(src, dst);

    return std::abs(result.x) + std::abs(result.y) + std::abs(result.z);
}

// アングル同士の差を返す
float UtilCalc::AngleDiff(float srcAngle, float dstAngle)
{
    float angleDiff = std::abs(srcAngle - dstAngle);
    if (angleDiff > Pi)
    {
        return PiTwo - angleDiff;
    }

    return angleDiff;
}

/*--------------*/
/*【当たり判定】*/
/*--------------*/

// 球同士の当たり判定
bool UtilCalc::SphereCollision(VECTOR srcPos, float srcRadius, VECTOR dstPos, float dstRadius)
{
    //円の当たり判定を行う
    VECTOR diff = VSub(dstPos, srcPos);
    float distance = VSize(diff);

    if (distance < (srcRadius + dstRadius))
    {
        return true;
    }
    return false;
}

//// 四角同士の当たり判定
//bool UtilCalc::BoxCollision(VECTOR srcTopRight, VECTOR srcBottomLeft, VECTOR dstTopRight, VECTOR dstBottomLeft)
//{
//    if ()
//    {
//
//    }
//    ele
//    {
//
//    }
//}