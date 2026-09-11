#pragma once

// ヘッダファイルの読み込み
#include "UseDeclaration.h"

// クラスの前方宣言
#ifdef USE_MY_VECTOR2D
class VECTOR2D;
#endif // USE_MY_VECTOR2D

#ifdef USE_MY_VECTOR3D
class VECTOR3D;
#endif // USE_MY_VECTOR3D

#ifdef USE_MY_MATRIX
class MyMatrix;
#endif // USE_MY_MATRIX

#ifdef USE_MY_VECTOR2D
// 2次元ベクトルを表すクラス(float型)
class VECTOR2D
{
private:
    float mfParamX;                                                                                                             // 2次元ベクトルのx成分
    float mfParamY;                                                                                                             // 2次元ベクトルのy成分

public:
    // コンストラクタ
    VECTOR2D();
    
    // コンストラクタ
    VECTOR2D(float xParam, float yParam);                                                                                       
    
    // デストラクタ
    ~VECTOR2D();                                                                                                                

    // ベクトルの足し算
    VECTOR2D Add(const VECTOR2D &vec) const;

    // ベクトルの引き算
    VECTOR2D Sub(const VECTOR2D &vec) const;
    
    // ベクトルの大きさの掛け算
    VECTOR2D Scale(const float size) const;
    
    // ベクトルの大きさを取得する(√あり)
    float Magnitude() const;
    
    // ベクトルの大きさを取得する(√なし)
    float MagnitudeNoSqrt() const;
    
    // ベクトルの正規化(√あり)
    VECTOR2D Normalize() const;
    
    // ベクトルの正規化(√なし)※本来のサイズよりかなり小さいベクトルになります。ベクトル同士の比較などにのみ使用してください
    VECTOR2D NormalizeNoSqrt() const;
    
    // ベクトルの正規化→大きさの掛け算(√あり)
    VECTOR2D SetMagnitude(const float size) const;

    // ベクトルの正規化→大きさの掛け算(√なし)※使用非推奨の関数です
    VECTOR2D SetMagnitudeNoSqrt(const float size) const;


    // 全ての値が0のVECTORを取得する関数
    static VECTOR2D GetZero();
    
    // 方向成分が左のみの単位ベクトルを取得する関数
    static VECTOR2D GetLeft();

    // 方向成分が右のみの単位ベクトルを取得する関数
    static VECTOR2D GetRight();

    // 方向成分が上のみの単位ベクトルを取得する関数
    static VECTOR2D GetUp();
    
    // 方向成分が下のみの単位ベクトルを取得する関数
    static VECTOR2D GetDown();

    // ベクトルの内積を取得する関数
    static float Dot(const VECTOR2D &vecL, const VECTOR2D &vecR);

    // 二つの2Dベクトルに直行するベクトルのz軸の値を取得する関数
    static float Cross(const VECTOR2D &vec1, const VECTOR2D &vec2);                                                             

    // 入射ベクトルが鏡に当たった際の反射ベクトルを取得する関数
    static VECTOR2D Reflect(const VECTOR2D &vec, const VECTOR2D &mirrorNorm);

    // 指定の範囲内でベクトルが等しいか。
    static bool EqualInRange(const VECTOR2D &vec1, const VECTOR2D &vec2, const float range);

    // 点pと直線ABから線上最近点を求め、世界座標を返す
    static VECTOR2D GetNewrPosOnLine(const VECTOR2D &pointPos, const VECTOR2D &startPos, const VECTOR2D &endPos);

    // vecAの方向成分とvecBの方向成分のどちらがよりtargetVecのの方向成分に近いかを確認する関数。AとBのより方向成分が近い方のベクトルを返す
    static VECTOR2D GetTargetDirectionNewrVec(const VECTOR2D &targetVec, const VECTOR2D &vecA, const VECTOR2D &vecB);

    // 線形補間
    static VECTOR2D GetLerpPos(const VECTOR2D &startPos, const VECTOR2D &targetPos, float rate);

    // 円形補間
    static VECTOR2D GetSlerpPos(const VECTOR2D &centerPos, const VECTOR2D &startPos, const VECTOR2D &targetPos, float rate);

    // ベクトルAがベクトルBを軸に+方向か-方向か、それともまったく別の方向に向かっているのかを判断する関数。
    // プラスなら1、マイナスなら-1、別ベクトルなら0を返す
    static int CheckAxisVecPlusOrMinus(const VECTOR2D &targetVec, const VECTOR2D &axisVec);

    // 点pが直線ABの中にあるかを判断する関数
    // (直線ABの中にあるなら1、AからBの延長線上にはあるが直線内にないなら0、そもそもその延長線上にすらないなら-1を返す)
    static int CheckPointInVector(const VECTOR2D &targetPointPos, const VECTOR2D &vecStartPos, VECTOR2D &vecEndPos);

    // moveVecに加える"スライド移動の分"の移動ベクトルを取得する関数。
    // normVecには√ありで正規化したVECTOR2Dを渡してください
    static VECTOR2D GetSlide(const VECTOR2D &moveVec, const VECTOR2D &normVec);

    // 特定の平面上にある単位円上の点の座標を、ラジアン角から取得する関数
    // ※この関数は性質上ゼロベクトルが返ってくる場合エラーが起こっています。余裕があるならエラーチェックもしてください。
    static VECTOR2D GetOnUnitCirclePos(const char planeX, const char planeY, float radian);

    void SetX(float x);
    void SetY(float y);
    void SetXY(float x, float y);

    float GetX() const;
    float GetY() const;

    VECTOR2D operator +() const;
    VECTOR2D operator -() const;
    void operator =(const VECTOR2D &mat);
    void operator +=(const VECTOR2D &mat);
    void operator -=(const VECTOR2D &mat);
    void operator *=(const float &scaler);
    void operator /=(const float &scaler);

#ifdef USE_MY_VECTOR3D
    // VECTOR2DからVECOTR3Dに変換する関数
    VECTOR3D To3D(float zParam) const;                                                                                          

    // VECTOR2DからVECOTR3Dに変換する関数
    VECTOR3D To3D(float newAxisParam, char y3D, char z3D) const;

    // VECTOR2DからVECOTR3Dに変換する関数
    VECTOR3D To3D(char x3D, float newAxisParam, char z3D) const;

    // VECTOR2DからVECOTR3Dに変換する関数
    VECTOR3D To3D(char x3D, char y3D, float newAxisParam) const;


#endif // USE_MY_VECTOR3D

#ifdef USE_MY_MATRIX
    // VECTOR2Dを計算できる行列に変換する関数
    MyMatrix ToMat() const;                                                                                                     

    // 行列を用いてVECTOR2Dの拡大・縮小(Scaling)、回転(Rotation)、平行移動(Transportion)を行う関数
    VECTOR2D MatrixOperation(const VECTOR2D &s, float r, const VECTOR2D &t) const;                                              
#endif // USE_MY_MATRIX
};

VECTOR2D operator +(const VECTOR2D &vecL, const VECTOR2D &vecR);
VECTOR2D operator -(const VECTOR2D &vecL, const VECTOR2D &vecR);
VECTOR2D operator *(const VECTOR2D &vecL, float scaler);
VECTOR2D operator /(const VECTOR2D &vecL, float scaler);
bool operator ==(const VECTOR2D &vecL, const VECTOR2D &vecR);
bool operator !=(const VECTOR2D &vecL, const VECTOR2D &vecR);

#ifdef USE_MY_MATRIX
MyMatrix operator *(const VECTOR2D &vec, const MyMatrix &mat);
#endif // USE_MY_MATRIX

#endif //USE_MY_VECTOR2D

#ifdef USE_MY_VECTOR3D
// 3次元ベクトルを表すクラス(float型)
class VECTOR3D
{
private:
    float mfParamX;                                                                                                             // 3次元ベクトルのx成分
    float mfParamY;                                                                                                             // 3次元ベクトルのy成分
    float mfParamZ;                                                                                                             // 3次元ベクトルのz成分

public:
    VECTOR3D();                                                                                                                 // コンストラクタ
    VECTOR3D(float xParam, float yParam, float zParam);                                                                         // コンストラクタ
    ~VECTOR3D();                                                                                                                // デストラクタ

    VECTOR3D Add(const VECTOR3D &vec2) const;                                                                                   // ベクトルの足し算
    VECTOR3D Sub(const VECTOR3D &vec2) const;                                                                                   // ベクトルの引き算
    VECTOR3D Scale(const float size) const;                                                                                     // ベクトルの大きさの掛け算
    float Magnitude() const;                                                                                                    // ベクトルの大きさを取得する(√あり)
    float MagnitudeNoSqrt() const;                                                                                              // ベクトルの大きさを取得する(√なし)
    VECTOR3D Normalize() const;                                                                                                 // ベクトルの正規化(√あり)
    VECTOR3D NormalizeNoSqrt() const;                                                                                           // ベクトルの正規化(√なし)※本来のサイズよりかなり小さいベクトルになります。ベクトル同士の比較などにのみ使用してください
    VECTOR3D SetMagnitude(const float size) const;                                                                              // ベクトルの正規化→大きさの掛け算(√あり)
    VECTOR3D SetMagnitudeNoSqrt(const float size) const;                                                                        // ベクトルの正規化→大きさの掛け算(√なし)※使用非推奨の関数です

    static VECTOR3D GetZero();                                                                                                  // 全ての値が0のVECTORを取得する関数
    static VECTOR3D GetLeft();                                                                                                  // 方向成分が左のみの単位ベクトルを取得する関数
    static VECTOR3D GetRight();                                                                                                 // 方向成分が右のみの単位ベクトルを取得する関数
    static VECTOR3D GetUp();                                                                                                    // 方向成分が上のみの単位ベクトルを取得する関数
    static VECTOR3D GetDown();                                                                                                  // 方向成分が下のみの単位ベクトルを取得する関数
    static VECTOR3D GetFront();                                                                                                 // 方向成分が前のみの単位ベクトルを取得する関数
    static VECTOR3D GetBack();                                                                                                  // 方向成分が後のみの単位ベクトルを取得する関数

    static float Dot(const VECTOR3D &vecL, const VECTOR3D &vecR);                                                               // ベクトルの内積を取得する関数
    static VECTOR3D Cross(const VECTOR3D &vec1, const VECTOR3D &vec2);                                                          // ベクトルの外積を取得する関数
    static bool EqualInRange(const VECTOR3D &vec1, const VECTOR3D &vec2, const float range);                                    // 指定の範囲内でベクトルが等しいか。
    static VECTOR3D GetNewrPosOnLine(const VECTOR3D &pointPos, const VECTOR3D &startPos, const VECTOR3D &endPos);               // 点pと直線ABから線上最近点を求め、世界座標を返す
    static VECTOR3D GetTargetDirectionNewrVec(const VECTOR3D &targetVec, const VECTOR3D &vecA, const VECTOR3D &vecB);           // vecAの方向成分とvecBの方向成分のどちらがよりtargetVecのの方向成分に近いかを確認する関数。AとBのより方向成分が近い方のベクトルを返す
    static VECTOR3D GetLerpPos(const VECTOR3D &startPos, const VECTOR3D &targetPos, float rate);                                // 線形補間
    static VECTOR3D GetSlerpPos(const VECTOR3D &centerPos, const VECTOR3D &startPos, const VECTOR3D &targetPos, float rate);    // 円形補間
    static int CheckAxisVecPlusOrMinus(const VECTOR3D &targetVec, const VECTOR3D &axisVec);                                     // ベクトルAがベクトルBを軸に+方向か-方向か、それともまったく別の方向に向かっているのかを判断する関数。プラスなら1、マイナスなら-1、別ベクトルなら0を返す
    static int CheckPointInVector(const VECTOR3D &targetPointPos, const VECTOR3D &vecStartPos, const VECTOR3D &vecEndPos);      // 点pが直線ABの中にあるかを判断する関数(直線ABの中にあるなら1、AからBの延長線上にはあるが直線内にないなら0、そもそもその延長線上にすらないなら-1を返す)
    static VECTOR3D GetSlide(const VECTOR3D &moveVec, const VECTOR3D &normVec);                                                 // moveVecに加える"スライド移動の分"の移動ベクトルを取得する関数。normVecには√ありで正規化したVECTOR2Dを渡してください
    static float GetPointToPlaneDistance(const VECTOR3D &pointPos, const VECTOR3D &onPlanePos, const VECTOR3D &planeNorm);      // ある点から無限平面までの距離を求める関数
    static float GetVECTOR3DToVECTOR3DNewrMagnitude();                                                                          // 二つの直線の最短距離を調べる関数
    static bool GetHitEndlessLineToEndlessPlane(const VECTOR3D &lineVec, const VECTOR3D &praneNorm);                            // 無限線と無限平面の衝突判定を求める関数
    static bool GetHitLineToEndlessPlane(                                                                                       // 線分と無限平面の衝突判定を求める関数
        const VECTOR3D &lineStart, const VECTOR3D &lineEnd,
        const VECTOR3D &planePos, const VECTOR3D &praneNorm);
    static bool GetHitLineToPolygon();                                                                                          // 線分と板ポリゴンの衝突判定を求める関数
    static VECTOR3D GetPerpendicular(const VECTOR3D &point, const VECTOR3D &lineStart, const VECTOR3D &lineEnd);                // ある点からある線への垂線を取得する関数
    static int CheckPointAndLine(const VECTOR3D &pointPos, const VECTOR3D &lineStartPos, const VECTOR3D &lineEndPos);           // 点と線分の関係性を返す関数。返り値が0の場合は点は線分の中、1の場合は点は線分のend方向の延長線上、-1の場合は点は線分のstart方向の延長線上、-2の場合は線分とは関係がないという意味。
    static float GetSegmentToSegmentNewrPos(                                                                                    // 二つの線分の最短距離を取得する関数(2本の線分の最近点c1, c2は出力先)。
        const VECTOR3D &aStart, const VECTOR3D &aEnd,
        const VECTOR3D &bStart, const VECTOR3D &bEnd,
        VECTOR3D &outA, VECTOR3D &outB);
    static float GetSegmentToTriangleNewrPos(                                                                                   // 線分と三角形の最短距離と最近点を取得する関数
        const VECTOR3D &segStart, const VECTOR3D &segEnd,
        const VECTOR3D &vertex0, const VECTOR3D &vertex1, const VECTOR3D &vertex2,
        VECTOR3D &outOnSeg, VECTOR3D &outOnTri);
    static bool CheckABB3DToPoint(const VECTOR3D &aabbMin, const VECTOR3D &aabbMax, const VECTOR3D point);                      // AABBの中に点が入っているか


    void SetX(float x);
    void SetY(float y);
    void SetZ(float z);
    void SetXYZ(float x, float y, float z);
    float GetX() const;
    float GetY() const;
    float GetZ() const;

    VECTOR3D operator +() const;
    VECTOR3D operator -() const;
    void operator =(const VECTOR3D &mat);
    void operator +=(const VECTOR3D &mat);
    void operator -=(const VECTOR3D &mat);
    void operator *=(const float &scaler);
    void operator /=(const float &scaler);

#ifdef USE_MY_VECTOR2D
    VECTOR2D To2D(char toX, char toY) const;                                                                                        // VECTOR3DからVECOTR2Dに変換する関数
#endif // USE_MY_VECTOR2D

#ifdef USE_MY_MATRIX
    MyMatrix ToMat() const;                                                                                                         // VECTOR3Dを計算できる行列に変換する関数
    VECTOR3D MatrixOperation(const VECTOR3D &s, const VECTOR3D &r, const VECTOR3D &t) const;                                        // 行列を用いてVECTOR3Dの拡大・縮小(Scaling)、回転(Rotation)、平行移動(Transportion)を行う関数
    static MyMatrix GetBaseVecToTarget(const VECTOR3D &baseVec, const VECTOR3D &targetStart, const VECTOR3D &targetEnd, float distance);   // BaseLine(一つの軸の成分のみ1の単位ベクトル)をtargetと同じベクトルにする変換行列を取得する関数
#endif // USE_MY_MATRIX
};

VECTOR3D operator +(const VECTOR3D &vecL, const VECTOR3D &vecR);
VECTOR3D operator -(const VECTOR3D &vecL, const VECTOR3D &vecR);
VECTOR3D operator *(const VECTOR3D &vecL, float scaler);
VECTOR3D operator /(const VECTOR3D &vecL, float scaler);
bool operator ==(const VECTOR3D &vecL, const VECTOR3D &vecR);
bool operator !=(const VECTOR3D &vecL, const VECTOR3D &vecR);

#ifdef USE_MY_MATRIX
MyMatrix operator *(const VECTOR3D &vec, const MyMatrix &mat);
#endif // USE_MY_MATRIX
#endif //USE_MY_VECTOR3D
