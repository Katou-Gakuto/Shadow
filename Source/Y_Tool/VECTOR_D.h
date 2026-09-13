#pragma once

// ヘッダファイルの読み込み
#include "UseDeclaration.h"

// クラスの前方宣言
#ifdef USE_MY_VECTOR2D
class VECTOR2D_D;
#endif // USE_MY_VECTOR2D

#ifdef USE_MY_VECTOR3D
class VECTOR3D_D;
#endif // USE_MY_VECTOR3D

#ifdef USE_MY_MATRIX
class MyMatrix_D;
#endif // USE_MY_MATRIX

#ifdef USE_MY_VECTOR2D
// 2次元ベクトルを表すクラス(double型)
class VECTOR2D_D
{
private:
    double mfParamX;                                                                                                                    // 2次元ベクトルのx成分
    double mfParamY;                                                                                                                    // 2次元ベクトルのy成分

public:
    VECTOR2D_D();                                                                                                                       // コンストラクタ
    VECTOR2D_D(double xParam, double yParam);                                                                                             // コンストラクタ
    ~VECTOR2D_D();                                                                                                                      // デストラクタ

    VECTOR2D_D Add(const VECTOR2D_D &vec) const;                                                                                        // ベクトルの足し算
    VECTOR2D_D Sub(const VECTOR2D_D &vec) const;                                                                                        // ベクトルの引き算
    VECTOR2D_D Scale(const double size) const;                                                                                          // ベクトルの大きさの掛け算
    double Magnitude() const;                                                                                                           // ベクトルの大きさを取得する(√あり)
    double MagnitudeNoSqrt() const;                                                                                                     // ベクトルの大きさを取得する(√なし)
    VECTOR2D_D Normalize() const;                                                                                                       // ベクトルの正規化(√あり)
    VECTOR2D_D NormalizeNoSqrt() const;                                                                                                 // ベクトルの正規化(√なし)※本来のサイズよりかなり小さいベクトルになります。ベクトル同士の比較などにのみ使用してください
    VECTOR2D_D SetMagnitude(const double size) const;                                                                                   // ベクトルの正規化→大きさの掛け算(√あり)
    VECTOR2D_D SetMagnitudeNoSqrt(const double size) const;                                                                             // ベクトルの正規化→大きさの掛け算(√なし)※使用非推奨の関数です

    static VECTOR2D_D GetZero();                                                                                                        // 全ての値が0のVECTORを取得する関数
    static VECTOR2D_D GetLeft();                                                                                                        // 方向成分が左のみの単位ベクトルを取得する関数
    static VECTOR2D_D GetRight();                                                                                                       // 方向成分が右のみの単位ベクトルを取得する関数
    static VECTOR2D_D GetUp();                                                                                                          // 方向成分が上のみの単位ベクトルを取得する関数
    static VECTOR2D_D GetDown();                                                                                                        // 方向成分が下のみの単位ベクトルを取得する関数

    static double Dot(const VECTOR2D_D &vecL, const VECTOR2D_D &vecR);                                                                  // ベクトルの内積を取得する関数
    static double Cross(const VECTOR2D_D &vec1, const VECTOR2D_D &vec2);                                                                // 二つの2Dベクトルに直行するベクトルのz軸の値を取得する
    static bool EqualInRange(const VECTOR2D_D &vec1, const VECTOR2D_D &vec2, const double range);                                       // 指定の範囲内でベクトルが等しいか。
    static VECTOR2D_D GetNewrPosOnLine(const VECTOR2D_D &pointPos, const VECTOR2D_D &startPos, const VECTOR2D_D &endPos);               // 点pと直線ABから線上最近点を求め、世界座標を返す
    static VECTOR2D_D GetTargetDirectionNewrVec(const VECTOR2D_D &targetVec, const VECTOR2D_D &vecA, const VECTOR2D_D &vecB);           // vecAの方向成分とvecBの方向成分のどちらがよりtargetVecのの方向成分に近いかを確認する関数。AとBのより方向成分が近い方のベクトルを返す
    static VECTOR2D_D GetLerpPos(const VECTOR2D_D &startPos, const VECTOR2D_D &targetPos, double rate);                                 // 線形補間
    static VECTOR2D_D GetSlerpPos(const VECTOR2D_D &centerPos, const VECTOR2D_D &startPos, const VECTOR2D_D &targetPos, double rate);   // 円形補間
    static int CheckAxisVecPlusOrMinus(const VECTOR2D_D &targetVec, const VECTOR2D_D &axisVec);                                         // ベクトルAがベクトルBを軸に+方向か-方向か、それともまったく別の方向に向かっているのかを判断する関数。プラスなら1、マイナスなら-1、別ベクトルなら0を返す
    static int CheckPointInVector(const VECTOR2D_D &targetPointPos, const VECTOR2D_D &vecStartPos, VECTOR2D_D &vecEndPos);              // 点pが直線ABの中にあるかを判断する関数(直線ABの中にあるなら1、AからBの延長線上にはあるが直線内にないなら0、そもそもその延長線上にすらないなら-1を返す)
    static VECTOR2D_D GetSlide(const VECTOR2D_D &moveVec, const VECTOR2D_D &normVec);                                                   // moveVecに加える"スライド移動の分"の移動ベクトルを取得する関数。normVecには√ありで正規化したVECTOR2D_Dを渡してください
    static VECTOR2D_D GetOnUnitCirclePos(const char planeX, const char planeY, double radian);                                          // 特定の平面上にある単位円上の点の座標を、ラジアン角から取得する関数※この関数は性質上ゼロベクトルが返ってくる場合はエラーです。余裕があるならエラーチェックもしてください。

    void SetX(double x);
    void SetY(double y);
    void SetXY(double x, double y);

    double GetX() const;
    double GetY() const;

    VECTOR2D_D operator +() const;
    VECTOR2D_D operator -() const;
    void operator =(const VECTOR2D_D &mat);
    void operator +=(const VECTOR2D_D &mat);
    void operator -=(const VECTOR2D_D &mat);
    void operator *=(const double &scaler);
    void operator /=(const double &scaler);

#ifdef USE_MY_VECTOR3D
    VECTOR3D_D To3D(double zParam) const;                                                                                                 // VECTOR2DからVECOTR3Dに変換する関数
    VECTOR3D_D To3D(double newAxisParam, char y3D, char z3D) const;                                                                       // VECTOR2DからVECOTR3Dに変換する関数
    VECTOR3D_D To3D(char x3D, double newAxisParam, char z3D) const;                                                                       // VECTOR2DからVECOTR3Dに変換する関数
    VECTOR3D_D To3D(char x3D, char y3D, double newAxisParam) const;                                                                       // VECTOR2DからVECOTR3Dに変換する関数

#endif // USE_MY_VECTOR3D

#ifdef USE_MY_MATRIX
    MyMatrix_D ToMat() const;                                                                                                           // VECTOR2Dを計算できる行列に変換する関数
    VECTOR2D_D MatrixOperation(const VECTOR2D_D &s, double r, const VECTOR2D_D &t) const;                                               // 行列を用いてVECTOR2Dの拡大・縮小(Scaling)、回転(Rotation)、平行移動(Transportion)を行う関数
#endif // USE_MY_MATRIX
};

VECTOR2D_D operator +(const VECTOR2D_D &vecL, const VECTOR2D_D &vecR);
VECTOR2D_D operator -(const VECTOR2D_D &vecL, const VECTOR2D_D &vecR);
VECTOR2D_D operator *(const VECTOR2D_D &vecL, double scaler);
VECTOR2D_D operator /(const VECTOR2D_D &vecL, double scaler);
bool operator ==(const VECTOR2D_D &vecL, const VECTOR2D_D &vecR);
bool operator !=(const VECTOR2D_D &vecL, const VECTOR2D_D &vecR);

#ifdef USE_MY_MATRIX
MyMatrix_D operator *(const VECTOR2D_D &vec, const MyMatrix_D &mat);
#endif // USE_MY_MATRIX
#endif //USE_MY_VECTOR2D

#ifdef USE_MY_VECTOR3D
// 3次元ベクトルを表すクラス(double型)
class VECTOR3D_D
{
private:
    double mfParamX;                                                                                                             // 3次元ベクトルのx成分
    double mfParamY;                                                                                                             // 3次元ベクトルのy成分
    double mfParamZ;                                                                                                             // 3次元ベクトルのz成分

public:
    VECTOR3D_D();                                                                                                                 // コンストラクタ
    VECTOR3D_D(double xParam, double yParam, double zParam);                                                                         // コンストラクタ
    ~VECTOR3D_D();                                                                                                                // デストラクタ

    VECTOR3D_D Add(const VECTOR3D_D &vec2) const;                                                                                   // ベクトルの足し算
    VECTOR3D_D Sub(const VECTOR3D_D &vec2) const;                                                                                   // ベクトルの引き算
    VECTOR3D_D Scale(const double size) const;                                                                                     // ベクトルの大きさの掛け算
    double Magnitude() const;                                                                                                    // ベクトルの大きさを取得する(√あり)
    double MagnitudeNoSqrt() const;                                                                                              // ベクトルの大きさを取得する(√なし)
    VECTOR3D_D Normalize() const;                                                                                                 // ベクトルの正規化(√あり)
    VECTOR3D_D NormalizeNoSqrt() const;                                                                                           // ベクトルの正規化(√なし)※本来のサイズよりかなり小さいベクトルになります。ベクトル同士の比較などにのみ使用してください
    VECTOR3D_D SetMagnitude(const double size) const;                                                                              // ベクトルの正規化→大きさの掛け算(√あり)
    VECTOR3D_D SetMagnitudeNoSqrt(const double size) const;                                                                        // ベクトルの正規化→大きさの掛け算(√なし)※使用非推奨の関数です

    static VECTOR3D_D GetZero();                                                                                                  // 全ての値が0のVECTORを取得する関数
    static VECTOR3D_D GetLeft();                                                                                                  // 方向成分が左のみの単位ベクトルを取得する関数
    static VECTOR3D_D GetRight();                                                                                                 // 方向成分が右のみの単位ベクトルを取得する関数
    static VECTOR3D_D GetUp();                                                                                                    // 方向成分が上のみの単位ベクトルを取得する関数
    static VECTOR3D_D GetDown();                                                                                                  // 方向成分が下のみの単位ベクトルを取得する関数
    static VECTOR3D_D GetFront();                                                                                                 // 方向成分が前のみの単位ベクトルを取得する関数
    static VECTOR3D_D GetBack();                                                                                                  // 方向成分が後のみの単位ベクトルを取得する関数

    static double Dot(const VECTOR3D_D &vecL, const VECTOR3D_D &vecR);                                                               // ベクトルの内積を取得する関数
    static VECTOR3D_D Cross(const VECTOR3D_D &vec1, const VECTOR3D_D &vec2);                                                          // ベクトルの外積を取得する関数
    static bool EqualInRange(const VECTOR3D_D &vec1, const VECTOR3D_D &vec2, const double range);                                    // 指定の範囲内でベクトルが等しいか。
    static VECTOR3D_D GetNewrPosOnLine(const VECTOR3D_D &pointPos, const VECTOR3D_D &startPos, const VECTOR3D_D &endPos);               // 点pと直線ABから線上最近点を求め、世界座標を返す
    static VECTOR3D_D GetTargetDirectionNewrVec(const VECTOR3D_D &targetVec, const VECTOR3D_D &vecA, const VECTOR3D_D &vecB);           // vecAの方向成分とvecBの方向成分のどちらがよりtargetVecのの方向成分に近いかを確認する関数。AとBのより方向成分が近い方のベクトルを返す
    static VECTOR3D_D GetLerpPos(const VECTOR3D_D &startPos, const VECTOR3D_D &targetPos, double rate);                                // 線形補間
    static VECTOR3D_D GetSlerpPos(const VECTOR3D_D &centerPos, const VECTOR3D_D &startPos, const VECTOR3D_D &targetPos, double rate);    // 円形補間
    static int CheckAxisVecPlusOrMinus(const VECTOR3D_D &targetVec, const VECTOR3D_D &axisVec);                                     // ベクトルAがベクトルBを軸に+方向か-方向か、それともまったく別の方向に向かっているのかを判断する関数。プラスなら1、マイナスなら-1、別ベクトルなら0を返す
    static int CheckPointInVector(const VECTOR3D_D &targetPointPos, const VECTOR3D_D &vecStartPos, const VECTOR3D_D &vecEndPos);      // 点pが直線ABの中にあるかを判断する関数(直線ABの中にあるなら1、AからBの延長線上にはあるが直線内にないなら0、そもそもその延長線上にすらないなら-1を返す)
    static VECTOR3D_D GetSlide(const VECTOR3D_D &moveVec, const VECTOR3D_D &normVec);                                                 // moveVecに加える"スライド移動の分"の移動ベクトルを取得する関数。normVecには√ありで正規化したVECTOR2Dを渡してください
    static double GetPointToPlaneDistance(const VECTOR3D_D &pointPos, const VECTOR3D_D &onPlanePos, const VECTOR3D_D &planeNorm);      // ある点から無限平面までの距離を求める関数
    static double GetVECTOR3DToVECTOR3DNewrMagnitude();                                                                          // 二つの直線の最短距離を調べる関数
    static bool GetHitEndlessLineToEndlessPlane(const VECTOR3D_D &lineVec, const VECTOR3D_D &praneNorm);                            // 無限線と無限平面の衝突判定を求める関数
    static bool GetHitLineToEndlessPlane(                                                                                       // 線分と無限平面の衝突判定を求める関数
        const VECTOR3D_D &lineStart, const VECTOR3D_D &lineEnd,
        const VECTOR3D_D &planePos, const VECTOR3D_D &praneNorm);
    static bool GetHitLineToPolygon();                                                                                          // 線分と板ポリゴンの衝突判定を求める関数
    static VECTOR3D_D GetPerpendicular(const VECTOR3D_D &point, const VECTOR3D_D &lineStart, const VECTOR3D_D &lineEnd);                // ある点からある線への垂線を取得する関数
    static int CheckPointAndLine(const VECTOR3D_D &pointPos, const VECTOR3D_D &lineStartPos, const VECTOR3D_D &lineEndPos);           // 点と線分の関係性を返す関数。返り値が0の場合は点は線分の中、1の場合は点は線分のend方向の延長線上、-1の場合は点は線分のstart方向の延長線上、-2の場合は線分とは関係がないという意味。
    static double GetSegmentToSegmentNewrPos(                                                                                    // 二つの線分の最短距離を取得する関数(2本の線分の最近点c1, c2は出力先)。
        const VECTOR3D_D &aStart, const VECTOR3D_D &aEnd,
        const VECTOR3D_D &bStart, const VECTOR3D_D &bEnd,
        VECTOR3D_D &outA, VECTOR3D_D &outB);
    static double GetSegmentToTriangleNewrPos(                                                                                   // 線分と三角形の最短距離と最近点を取得する関数
        const VECTOR3D_D &segStart, const VECTOR3D_D &segEnd,
        const VECTOR3D_D &vertex0, const VECTOR3D_D &vertex1, const VECTOR3D_D &vertex2,
        VECTOR3D_D &outOnSeg, VECTOR3D_D &outOnTri);
    static bool CheckABB3DToPoint(const VECTOR3D_D &aabbMin, const VECTOR3D_D &aabbMax, const VECTOR3D_D point);                      // AABBの中に点が入っているか


    void SetX(double x);
    void SetY(double y);
    void SetZ(double z);
    void SetXYZ(double x, double y, double z);
    double GetX() const;
    double GetY() const;
    double GetZ() const;

    VECTOR3D_D operator +() const;
    VECTOR3D_D operator -() const;
    void operator =(const VECTOR3D_D &mat);
    void operator +=(const VECTOR3D_D &mat);
    void operator -=(const VECTOR3D_D &mat);
    void operator *=(const double &scaler);
    void operator /=(const double &scaler);

#ifdef USE_MY_VECTOR2D
    VECTOR2D_D To2D(char toX, char toY) const;                                                                                        // VECTOR3DからVECOTR2Dに変換する関数
#endif // USE_MY_VECTOR2D

#ifdef USE_MY_MATRIX
    MyMatrix_D ToMat() const;                                                                                                         // VECTOR3Dを計算できる行列に変換する関数
    VECTOR3D_D MatrixOperation(const VECTOR3D_D &s, const VECTOR3D_D &r, const VECTOR3D_D &t) const;                                        // 行列を用いてVECTOR3Dの拡大・縮小(Scaling)、回転(Rotation)、平行移動(Transportion)を行う関数
    static MyMatrix_D GetBaseVecToTarget(const VECTOR3D_D &baseVec, const VECTOR3D_D &targetStart, const VECTOR3D_D &targetEnd, double distance);   // BaseLine(一つの軸の成分のみ1の単位ベクトル)をtargetと同じベクトルにする変換行列を取得する関数
#endif // USE_MY_MATRIX
};

VECTOR3D_D operator +(const VECTOR3D_D &vecL, const VECTOR3D_D &vecR);
VECTOR3D_D operator -(const VECTOR3D_D &vecL, const VECTOR3D_D &vecR);
VECTOR3D_D operator *(const VECTOR3D_D &vecL, double scaler);
VECTOR3D_D operator /(const VECTOR3D_D &vecL, double scaler);
bool operator ==(const VECTOR3D_D &vecL, const VECTOR3D_D &vecR);
bool operator !=(const VECTOR3D_D &vecL, const VECTOR3D_D &vecR);

#ifdef USE_MY_MATRIX
MyMatrix_D operator *(const VECTOR3D_D &vec, const MyMatrix_D &mat);
#endif // USE_MY_MATRIX
#endif //USE_MY_VECTOR3D
