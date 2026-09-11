#include "MyEulerAngle.h"

#include <math.h>

#ifdef USE_MY_MATRIX
#include "MyMatrix.h"
#endif

#ifdef USE_MY_QUATERNION
#include "MyQuaternion.h"
#endif

#ifdef USE_MY_ROTATION
#ifdef USE_MY_EULERANGLE
MyEulerAngle::MyEulerAngle() :
    mnOrder(EulerOrder::ZXY),
    mfParamX(0.0f),
    mfParamY(0.0f),
    mfParamZ(0.0f)
{

}

MyEulerAngle::MyEulerAngle(float x, float y, float z) :
    mnOrder(EulerOrder::ZXY),
    mfParamX(x),
    mfParamY(y),
    mfParamZ(z)
{

}

MyEulerAngle::MyEulerAngle(EulerOrder order, float x, float y, float z) :
    mnOrder(order),
    mfParamX(x),
    mfParamY(y),
    mfParamZ(z)
{

}

MyEulerAngle::~MyEulerAngle()
{

}

void MyEulerAngle::SetParam(float x, float y, float z)
{
    mfParamX = x;
    mfParamY = y;
    mfParamZ = z;   
}

void MyEulerAngle::SetParam(EulerOrder order, float x, float y, float z)
{
    mnOrder = order;
    mfParamX = x;
    mfParamY = y;
    mfParamZ = z;
}

EulerOrder MyEulerAngle::GetOrder() const
{
    return this->mnOrder;
}

float MyEulerAngle::GetX() const
{
    return this->mfParamX;
}

float MyEulerAngle::GetY() const
{
    return this->mfParamY;
}

float MyEulerAngle::GetZ() const
{
    return this->mfParamZ;
}

#ifdef USE_MY_MATRIX
MyMatrix MyEulerAngle::ToMat() const
{
    // ‚¿‚È‚Ý‚É‰EŽèŒn‚©¶ŽèŒn‚©A‚Ç‚Á‚¿‚©‚í‚©‚ñ‚È‚¢‚Å‚·B‚±‚êB
    auto cx = cosf(this->mfParamX);
    auto sx = sinf(this->mfParamX);
    auto cy = cosf(this->mfParamY);
    auto sy = sinf(this->mfParamY);
    auto cz = cosf(this->mfParamZ);
    auto sz = sinf(this->mfParamZ);
    MyMatrix temp = MyMatrix::GetIdentity(4);
    switch (this->mnOrder)
    {
    case EulerOrder::XYZ:
        temp.SetParam(0, 0, (cy * cz));
        temp.SetParam(0, 1, (sx * sy * cz + cx * sz));
        temp.SetParam(0, 2, (-cx * sy * cz + sx * sz));
        temp.SetParam(1, 0, (-cy * sz));
        temp.SetParam(1, 1, (-sx * sy * sz + cx * cz));
        temp.SetParam(1, 2, (cx * sy * sz + sx * cz));
        temp.SetParam(2, 0, (sy));
        temp.SetParam(2, 1, (-sx * cy));
        temp.SetParam(2, 2, (cx * cy));
        break;
    case EulerOrder::XZY:
        temp.SetParam(0, 0, (cy * cz));
        temp.SetParam(0, 1, (cx * cy * sz + sx * sy));
        temp.SetParam(0, 2, (sx * cy * sz - cx * sy));
        temp.SetParam(1, 0, (-sz));
        temp.SetParam(1, 1, (cx * cz));
        temp.SetParam(1, 2, (sx * cz));
        temp.SetParam(2, 0, (sy * cz));
        temp.SetParam(2, 1, (cx * sy * sz - sx * cy));
        temp.SetParam(2, 2, (sx * sy * sz + cx * cy));
        break;
    case EulerOrder::YXZ:
        temp.SetParam(0, 0, (sx * sy * sz + cy * cz));
        temp.SetParam(0, 1, (cx * sz));
        temp.SetParam(0, 2, (sx * cy * sz - sy * cz));
        temp.SetParam(1, 0, (sx * sy * cz - cy * sz));
        temp.SetParam(1, 1, (cx * cz));
        temp.SetParam(1, 2, (sx * cz));
        temp.SetParam(2, 0, (cx * sy));
        temp.SetParam(2, 1, (-sx));
        temp.SetParam(2, 2, (cx * cy));
        break;
    case EulerOrder::YZX:
        temp.SetParam(0, 0, (cy * cz));
        temp.SetParam(0, 1, (sz));
        temp.SetParam(0, 2, (-sy * cz));
        temp.SetParam(1, 0, (-cx * cy * sz + sx * sy));
        temp.SetParam(1, 1, (cx * cz));
        temp.SetParam(1, 2, (cx * sy * sz + sx * cy));
        temp.SetParam(2, 0, (sx * cy * sz + cx * sy));
        temp.SetParam(2, 1, (-sx * cz));
        temp.SetParam(2, 2, (-sx * sy * sz + cx * cy));
        break;
    case EulerOrder::ZXY:
        temp.SetParam(0, 0, (-sx * sy * sz + cy * cz));
        temp.SetParam(0, 1, (sx * sy * cz + cy * sz));
        temp.SetParam(0, 2, (-cx * sy));
        temp.SetParam(1, 0, (-cx * sz));
        temp.SetParam(1, 1, (cx * cz));
        temp.SetParam(1, 2, (sx));
        temp.SetParam(2, 0, (sx * cy * sz + sy * cz));
        temp.SetParam(2, 1, (-sx * cy * cz + sy * sz));
        temp.SetParam(2, 2, (cx * cy));
        break;
    case EulerOrder::ZYX:
        temp.SetParam(0, 0, (cy * cz));
        temp.SetParam(0, 1, (cy * sz));
        temp.SetParam(0, 2, (-sy));
        temp.SetParam(1, 0, (sx * sy * cz - cx * sz));
        temp.SetParam(1, 1, (sx * sy * sz + cx * cz));
        temp.SetParam(1, 2, (sx * cy));
        temp.SetParam(2, 0, (cx * sy * cz + sx * sz));
        temp.SetParam(2, 1, (cx * sy * sz - sx * cz));
        temp.SetParam(2, 2, (cx * cy));
        break;
    default:
        temp = MyMatrix::GetDeactivate();
        break;
    }
    return temp;
}
#endif // USE_MY_MATRIX

#ifdef USE_MY_QUATERNION
MyQuaternion MyEulerAngle::ToQtn() const
{
    MyQuaternion temp(0.0f, 0.0f, 0.0f, 0.0f);
    auto cx = cosf(0.5f * this->mfParamX);
    auto sx = sinf(0.5f * this->mfParamX);
    auto cy = cosf(0.5f * this->mfParamY);
    auto sy = sinf(0.5f * this->mfParamY);
    auto cz = cosf(0.5f * this->mfParamZ);
    auto sz = sinf(0.5f * this->mfParamZ);
    switch (this->mnOrder)
    {
    case EulerOrder::XYZ:
        temp = MyQuaternion(
            cx * sy * sz + sx * cy * cz,
            -sx * cy * sz + cx * sy * cz,
            cx * cy * sz + sx * sy * cz,
            -sx * sy * sz + cx * cy * cz);
        break;
    case EulerOrder::XZY:
        temp = MyQuaternion(
            -cx * sy * sz + sx * cy * cz,
            cx * sy * cz - sx * cy * sz,
            sx * sy * cz + cx * cy * sz,
            sx * sy * sz + cx * cy * cz);
        break;
    case EulerOrder::YXZ:
        temp = MyQuaternion(
            cx * sy * sz + sx * cy * cz,
            -sx * cy * sz + cx * sy * cz,
            cx * cy * sz - sx * sy * cz,
            sx * sy * sz + cx * cy * cz);
        break;
    case EulerOrder::YZX:
        temp = MyQuaternion(
            sx * cy * cz + cx * sy * sz,
            sx * cy * sz + cx * sy * cz,
            -sx * sy * cz + cx * cy * sz,
            -sx * sy * sz + cx * cy * cz);
        break;
    case EulerOrder::ZXY:
        temp = MyQuaternion(
            -cx * sy * sz + sx * cy * cz,
            cx * sy * cz + sx * cy * sz,
            sx * sy * cz + cx * cy * sz,
            -sx * sy * sz + cx * cy * cz);
        break;
    case EulerOrder::ZYX:
        temp = MyQuaternion(
            sx * cy * cz - cx * sy * sz,
            sx * cy * sz + cx * sy * cz,
            -sx * sy * cz + cx * cy * sz,
            sx * sy * sz + cx * cy * cz);
        break;
    default:
        break;
    }
    return temp;
}
#endif // USE_MY_QUATERNION
#endif // USE_MY_EULERANGLE

#ifdef USE_MY_EULERANGLE
MyEulerAngle_D::MyEulerAngle_D() :
    mnOrder(EulerOrder::ZXY),
    mfParamX(0.0),
    mfParamY(0.0),
    mfParamZ(0.0)
{

}

MyEulerAngle_D::MyEulerAngle_D(double x, double y, double z) :
    mnOrder(EulerOrder::ZXY),
    mfParamX(x),
    mfParamY(y),
    mfParamZ(z)
{

}

MyEulerAngle_D::MyEulerAngle_D(EulerOrder order, double x, double y, double z) :
    mnOrder(order),
    mfParamX(x),
    mfParamY(y),
    mfParamZ(z)
{

}

MyEulerAngle_D::~MyEulerAngle_D()
{

}

void MyEulerAngle_D::SetParam(double x, double y, double z)
{
    mfParamX = x;
    mfParamY = y;
    mfParamZ = z;
}

void MyEulerAngle_D::SetParam(EulerOrder order, double x, double y, double z)
{
    mnOrder = order;
    mfParamX = x;
    mfParamY = y;
    mfParamZ = z;
}

EulerOrder MyEulerAngle_D::GetOrder() const
{
    return this->mnOrder;
}

double MyEulerAngle_D::GetX() const
{
    return this->mfParamX;
}

double MyEulerAngle_D::GetY() const
{
    return this->mfParamY;
}

double MyEulerAngle_D::GetZ() const
{
    return this->mfParamZ;
}

#ifdef USE_MY_MATRIX
MyMatrix MyEulerAngle_D::ToMat() const
{
    // ‚¿‚È‚Ý‚É‰EŽèŒn‚©¶ŽèŒn‚©A‚Ç‚Á‚¿‚©‚í‚©‚ñ‚È‚¢‚Å‚·B‚±‚êB
    auto cx = cos(this->mfParamX);
    auto sx = sin(this->mfParamX);
    auto cy = cos(this->mfParamY);
    auto sy = sin(this->mfParamY);
    auto cz = cos(this->mfParamZ);
    auto sz = sin(this->mfParamZ);
    MyMatrix temp = MyMatrix::GetIdentity(4);
    switch (this->mnOrder)
    {
    case EulerOrder::XYZ:
        temp.SetParam(0, 0, (cy * cz));
        temp.SetParam(0, 1, (sx * sy * cz + cx * sz));
        temp.SetParam(0, 2, (-cx * sy * cz + sx * sz));
        temp.SetParam(1, 0, (-cy * sz));
        temp.SetParam(1, 1, (-sx * sy * sz + cx * cz));
        temp.SetParam(1, 2, (cx * sy * sz + sx * cz));
        temp.SetParam(2, 0, (sy));
        temp.SetParam(2, 1, (-sx * cy));
        temp.SetParam(2, 2, (cx * cy));
        break;
    case EulerOrder::XZY:
        temp.SetParam(0, 0, (cy * cz));
        temp.SetParam(0, 1, (cx * cy * sz + sx * sy));
        temp.SetParam(0, 2, (sx * cy * sz - cx * sy));
        temp.SetParam(1, 0, (-sz));
        temp.SetParam(1, 1, (cx * cz));
        temp.SetParam(1, 2, (sx * cz));
        temp.SetParam(2, 0, (sy * cz));
        temp.SetParam(2, 1, (cx * sy * sz - sx * cy));
        temp.SetParam(2, 2, (sx * sy * sz + cx * cy));
        break;
    case EulerOrder::YXZ:
        temp.SetParam(0, 0, (sx * sy * sz + cy * cz));
        temp.SetParam(0, 1, (cx * sz));
        temp.SetParam(0, 2, (sx * cy * sz - sy * cz));
        temp.SetParam(1, 0, (sx * sy * cz - cy * sz));
        temp.SetParam(1, 1, (cx * cz));
        temp.SetParam(1, 2, (sx * cz));
        temp.SetParam(2, 0, (cx * sy));
        temp.SetParam(2, 1, (-sx));
        temp.SetParam(2, 2, (cx * cy));
        break;
    case EulerOrder::YZX:
        temp.SetParam(0, 0, (cy * cz));
        temp.SetParam(0, 1, (sz));
        temp.SetParam(0, 2, (-sy * cz));
        temp.SetParam(1, 0, (-cx * cy * sz + sx * sy));
        temp.SetParam(1, 1, (cx * cz));
        temp.SetParam(1, 2, (cx * sy * sz + sx * cy));
        temp.SetParam(2, 0, (sx * cy * sz + cx * sy));
        temp.SetParam(2, 1, (-sx * cz));
        temp.SetParam(2, 2, (-sx * sy * sz + cx * cy));
        break;
    case EulerOrder::ZXY:
        temp.SetParam(0, 0, (-sx * sy * sz + cy * cz));
        temp.SetParam(0, 1, (sx * sy * cz + cy * sz));
        temp.SetParam(0, 2, (-cx * sy));
        temp.SetParam(1, 0, (-cx * sz));
        temp.SetParam(1, 1, (cx * cz));
        temp.SetParam(1, 2, (sx));
        temp.SetParam(2, 0, (sx * cy * sz + sy * cz));
        temp.SetParam(2, 1, (-sx * cy * cz + sy * sz));
        temp.SetParam(2, 2, (cx * cy));
        break;
    case EulerOrder::ZYX:
        temp.SetParam(0, 0, (cy * cz));
        temp.SetParam(0, 1, (cy * sz));
        temp.SetParam(0, 2, (-sy));
        temp.SetParam(1, 0, (sx * sy * cz - cx * sz));
        temp.SetParam(1, 1, (sx * sy * sz + cx * cz));
        temp.SetParam(1, 2, (sx * cy));
        temp.SetParam(2, 0, (cx * sy * cz + sx * sz));
        temp.SetParam(2, 1, (cx * sy * sz - sx * cz));
        temp.SetParam(2, 2, (cx * cy));
        break;
    default:
        temp = MyMatrix::GetDeactivate();
        break;
    }
    return temp;
}
#endif // USE_MY_MATRIX

#ifdef USE_MY_QUATERNION
MyQuaternion MyEulerAngle_D::ToQtn() const
{
    MyQuaternion temp(0.0, 0.0, 0.0, 0.0);
    auto cx = cos(0.5 * this->mfParamX);
    auto sx = sin(0.5 * this->mfParamX);
    auto cy = cos(0.5 * this->mfParamY);
    auto sy = sin(0.5 * this->mfParamY);
    auto cz = cos(0.5 * this->mfParamZ);
    auto sz = sin(0.5 * this->mfParamZ);
    switch (this->mnOrder)
    {
    case EulerOrder::XYZ:
        temp = MyQuaternion(
            cx * sy * sz + sx * cy * cz,
            -sx * cy * sz + cx * sy * cz,
            cx * cy * sz + sx * sy * cz,
            -sx * sy * sz + cx * cy * cz);
        break;
    case EulerOrder::XZY:
        temp = MyQuaternion(
            -cx * sy * sz + sx * cy * cz,
            cx * sy * cz - sx * cy * sz,
            sx * sy * cz + cx * cy * sz,
            sx * sy * sz + cx * cy * cz);
        break;
    case EulerOrder::YXZ:
        temp = MyQuaternion(
            cx * sy * sz + sx * cy * cz,
            -sx * cy * sz + cx * sy * cz,
            cx * cy * sz - sx * sy * cz,
            sx * sy * sz + cx * cy * cz);
        break;
    case EulerOrder::YZX:
        temp = MyQuaternion(
            sx * cy * cz + cx * sy * sz,
            sx * cy * sz + cx * sy * cz,
            -sx * sy * cz + cx * cy * sz,
            -sx * sy * sz + cx * cy * cz);
        break;
    case EulerOrder::ZXY:
        temp = MyQuaternion(
            -cx * sy * sz + sx * cy * cz,
            cx * sy * cz + sx * cy * sz,
            sx * sy * cz + cx * cy * sz,
            -sx * sy * sz + cx * cy * cz);
        break;
    case EulerOrder::ZYX:
        temp = MyQuaternion(
            sx * cy * cz - cx * sy * sz,
            sx * cy * sz + cx * sy * cz,
            -sx * sy * cz + cx * cy * sz,
            sx * sy * sz + cx * cy * cz);
        break;
    default:
        break;
    }
    return temp;
}
#endif // USE_MY_QUATERNION
#endif // USE_MY_EULERANGLE

#endif // USE_MY_ROTATION