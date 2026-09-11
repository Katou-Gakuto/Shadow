#pragma once
/*
クォータニオンとは、ざっくりいうと任意の軸を基準とした回転量のこと

このサイトを参考に作っています(https://qiita.com/amanatsutouko/items/51f39363d195079e1cc8)

Qtn.SetElr() <=> Qtn.ToElr()
ここからQtnとElrは同じ左手系と推察

Qtn.ToMat() == Elr.ToMat()
QtnのMat変換とElr変換のMatは同じ値だった。これからも二つは同じ左手系であると思われ、Matの回転行列も左手系のものだと思われる

○ Mat.ToElr()
× Mat.ToQtn()
Mat.ToElr()が変換に成功していたため、問題があるのはMat.ToQtn()の中身だと推察される
*/
#include "UseDeclaration.h"

#ifdef USE_MY_ROTATION
#include "EulerOrder.h"

#ifdef USE_MY_VECTOR3D
class VECTOR3D;
#endif // USE_MY_VECTOR3D

#ifdef USE_MY_MATRIX
class MyMatrix;
#endif // USE_MY_MATRIX

#ifdef USE_MY_EULERANGLE
class MyEulerAngle;
#endif // USE_MY_EULERANGLE

#ifdef USE_MY_VECTOR3D
#ifdef USE_MY_QUATERNION
// クォータニオンを表すクラス(Unity準拠なのでおそらく左手系)
class MyQuaternion
{
private:
    float mfParamX;
    float mfParamY;
    float mfParamZ;
    float mfParamW;

public:
    MyQuaternion();                                                                                 // デフォルトコンストラクタ
    MyQuaternion(float x, float y, float z, float w);                                               // コンストラクタ
    ~MyQuaternion();                                                                                // デストラクタ

    MyQuaternion GetInverse() const;                                                                // 自身の逆クォータニオンを取得する関数
    MyQuaternion Normalize() const;                                                                 // 自身に正規化を行ったクォータニオンを新たに取得する関数
    MyQuaternion GetAngleAndAxis() const;                                                           // 回転を座標に対する角度の値(AngleAxis)に変換する。返り値であるMyQuaternionの中身はx,y,z,までが座標、wは角度の値です
    VECTOR3D GetEulerRadian() const;                                                                // 自身の値をオイラー角に直して返す関数
    void SetEulerRadian(const VECTOR3D &eulerRadian);                                               // オイラー角から自身の値を設定し直す関数。// ※x軸デグリー90°に加えて他の軸にさらに値が入ってしまうとジンバルロックが発生します。
    void SetFromToRotation(const VECTOR3D &from, const VECTOR3D &to);                               // fromからtoへの回転を作成し、自身に代入する関数
    void SetLookRotation(VECTOR3D view);                                                            // 指定した forward と upwards 方向に回転する関数

    float GetX() const;
    float GetY() const;
    float GetZ() const;
    float GetW() const;

    static float GetDot(const MyQuaternion &qtnL, const MyQuaternion &qtnR);                        // 二つのクォータニオンの内積を取得する
    static float GetAngle(const MyQuaternion &qtnA, const MyQuaternion &qtnB);                      //  2つの回転a、b間の角度を返す
    static MyQuaternion Lerp(const MyQuaternion &a, const MyQuaternion &b, float rate);             // aとbの間をtで補間して正規化する関数。rateは 0～1 の範囲のクランプされる
    static MyQuaternion LerpUnclamped(const MyQuaternion &a, const MyQuaternion &b, float rate);    // aとbの間をrateで補間して正規化する関数。rateは 0～1 の範囲にクランプされない
    static MyQuaternion Slerp(const MyQuaternion &a, const MyQuaternion &b, float rate);            // 球面線形補完を得る関数。rateは 0～1 の範囲にクランプされる
    static MyQuaternion SlerpUnclamped(const MyQuaternion &a, const MyQuaternion &b, float rate);   // 球面線形補完を得る関数。rateは 0～1 の範囲にクランプされない
    
    static MyQuaternion GetIdentity();                                                              // 単位クォータニオンを取得する関数
    static MyQuaternion GetQtnAixs(const VECTOR3D &axis, float radian);                             // 軸となるベクトルと回転量から新しいクォータニオンを取得する関数
    static MyQuaternion GetQtnEuler(const VECTOR3D &eulerAxisRadian);                               // x軸, y軸, z軸の回転量から新しいクォータニオンを取得する関数
    static MyQuaternion GetQtnFromToRotation(const VECTOR3D &from, const VECTOR3D &to);             // fromからtoへの回転を作成して返す関数
    static MyQuaternion GetQtnLookRotation(const VECTOR3D &forward);                                // 引数のupwardsを省略した場合に、Vector3.upがデフォルト引数となるようにオーバーロード
    static MyQuaternion GetQtnLookRotation(const VECTOR3D &forward, const VECTOR3D &upwards);       // オブジェクトの正面(forward)を引数のforwardの向きに回転させる回転を生成する

#ifdef USE_MY_MATRIX
    MyMatrix ToMat() const;
#endif // USE_MY_MATRIX

#ifdef USE_MY_EULERANGLE
    MyEulerAngle ToEulerAngle() const;
    MyEulerAngle ToEulerAngle(EulerOrder order) const;
#endif // USE_MY_EULERANGLE
};

MyQuaternion operator *(const MyQuaternion &qtnL, const MyQuaternion &qtnR);                        // クォータニオン同士の掛け算。回転Aに回転Bを組み合わせる場合は 回転B * 回転A とすること。
VECTOR3D operator * (const MyQuaternion &qtn, const VECTOR3D &vec);
#endif // USE_MY_VECTOR3D
#endif // USE_MY_QUATERNION
#endif // USE_MY_ROTATION