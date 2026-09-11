#pragma once
/*
(https://qiita.com/aa_debdeb/items/abe90a9bd0b4809813da)←このサイトを参考にしています

*/
#include "UseDeclaration.h"

#ifdef USE_MY_ROTATION
#include "EulerOrder.h"

#ifdef USE_MY_MATRIX
class MyMatrix;
#endif // USE_MY_MATRIX

#ifdef USE_MY_QUATERNION
class MyQuaternion;
#endif // USE_MY_QUATERNION

#ifdef USE_MY_EULERANGLE
// オイラー角を表すクラス
class MyEulerAngle
{
private:
    EulerOrder mnOrder;
    float mfParamX;
    float mfParamY;
    float mfParamZ;

public:
    MyEulerAngle();
    MyEulerAngle(float x, float y, float z);
    MyEulerAngle(EulerOrder order, float x, float y, float z);
    ~MyEulerAngle();

    void SetParam(float x, float y, float z);
    void SetParam(EulerOrder order, float x, float y, float z);

    EulerOrder GetOrder() const;
    float GetX() const;
    float GetY() const;
    float GetZ() const;

#ifdef USE_MY_MATRIX
    MyMatrix ToMat() const;
#endif // USE_MY_MATRIX

#ifdef USE_MY_QUATERNION
    MyQuaternion ToQtn() const;
#endif // USE_MY_QUATERNION
};
#endif // USE_MY_EULERANGLE


#ifdef USE_MY_EULERANGLE
// オイラー角を表すクラス
class MyEulerAngle_D
{
private:
    EulerOrder mnOrder;
    double mfParamX;
    double mfParamY;
    double mfParamZ;

public:
    MyEulerAngle_D();
    MyEulerAngle_D(double x, double y, double z);
    MyEulerAngle_D(EulerOrder order, double x, double y, double z);
    ~MyEulerAngle_D();

    void SetParam(double x, double y, double z);
    void SetParam(EulerOrder order, double x, double y, double z);

    EulerOrder GetOrder() const;
    double GetX() const;
    double GetY() const;
    double GetZ() const;

#ifdef USE_MY_MATRIX
    MyMatrix ToMat() const;
#endif // USE_MY_MATRIX

#ifdef USE_MY_QUATERNION
    MyQuaternion ToQtn() const;
#endif // USE_MY_QUATERNION
};
#endif // USE_MY_EULERANGLE
#endif // USE_MY_ROTATION