#pragma once
/*
＜行列とは＞
行列とは簡単に言うと数字を並べたもの。
例えば
3, 5, 9
7, 0, 10
の行列があるとする。
この行列は『2行、3列の行列』である。
横の数が行の数であり、縦の数が列の数。

＜正方行列・単位行列と零行列とは＞
この行の数と列の数が同じ行列のことを『正方行列』という。
対角成分(左上から右下に向けて斜めの場所)のみ1で、他の値が全て0の行列を『単位行列』という。掛け算の1のような役割を持つ。
全ての値が0の行列を『零行列』という。掛け算で0のような役割を持つ。

＜逆行列・正則行列・得意行列・行列式とは＞
簡単に言うと↓のような感じ。
行列A * 行列B == 単位行列
が成立する場合、行列Bは行列Aの『逆行列』といい、逆行列が存在する正方行列を『正則行列』という。
また、逆行列が存在しない正方行列を『特異行列』という。
逆行列が存在するかどうかは『行列式』というものを用いることで確認できる。
行列式の答えが0になる場合のみ、逆行列は存在しない。

＜行列の余因子とは＞
行列の余因子とは基本的に『n行m列の余因子』と呼ばれることが多い。
たとえば 3 * 3 の行列があったして、2行3列の余因子といえば↓のような物になる。
1, 2, 3     1, 2   
4, 5, 6  => 7, 8 =>(計算) n 
7, 8, 9
みたいな。
この計算部分がややこしいので今は割愛しますが、こういうのを余因子という。

＜転置行列とは＞
行列Aの行の数と列の数、そしてパラメータを逆にさせたもの。
2行3列
1,2,3
4,5,6
の行列があった場合、この行列の転置行列は
1,4
2,5
3,6
となる。わかりやすいでしょ。

以上のことを念頭に置いて、以下のコードを読み進めてくださいね。
ちなみにこの.hおよび.cppのゴールは逆行列を求められるようになり、ベクトルとの掛け算に応用できるようにすることです。
クラスの関数についている「Ass」はAssignment(代入、割り当て)の意味です
*/

// ヘッダファイルの読み込み
#include "UseDeclaration.h"

#include <string>
#include "MallocData.h"

// クラスの前方宣言
#ifdef USE_MY_VECTOR2D
class VECTOR2D;
class VECTOR2D_D;
#endif // USE_MY_VECTOR2D

#ifdef USE_MY_VECTOR3D
class VECTOR3D;
class VECTOR3D_D;
#endif // USE_MY_VECTOR3D

#ifdef USE_MY_ROTATION
#include "EulerOrder.h"

#ifdef USE_MY_QUATERNION
class MyQuaternion;
#endif // USE_MY_QUATERNION

#ifdef USE_MY_EULERANGLE
class MyEulerAngle;
#endif // USE_MY_EULERANGLE

#endif // USE_MY_ROTATION

#ifdef USE_MY_MATRIX
// 行の個数、列の個数を自由に決められる行列のクラス(float型)
class MyMatrix
{
private:
    unsigned long mnRowCount;                                                                       // 行の数
    unsigned long mnColumnCount;                                                                    // 列の数
    MallocData<float> mlParam;                                                                      // それぞれの値の数(中途半端にヒープ領域を使いたくなかったのでstd::vectorにしています)

public:
    // 自身の行コンテキスト
    class MyMatrixRow;

    // 自身の列コンテキスト
    class MyMatrixColumn;

    MyMatrix(unsigned long rowCount, unsigned long columnCount);                                        // コンストラクタ
    MyMatrix(const MyMatrix &srcMatrix);                                                                // コピーコンストラクタ
    MyMatrix(MyMatrix &&srcMatrix) noexcept;                                                            // ムーブコンストラクタ
    ~MyMatrix();                                                                                        // デストラクタ

    MyMatrix &Copy(const MyMatrix &mat);                                                                // コピー
    MyMatrix &Move(MyMatrix &mat) noexcept;                                                             // ムーブ
    MyMatrix &Move(MyMatrix &&mat) noexcept;                                                            // ムーブ

    bool CheckActive() const;                                                                           // この行列が有効なものかを確認する関数
    bool CheckSquare() const;                                                                           // この行列が正方行列かを確認する関数
    unsigned long GetSize() const;                                                                      // この行列の要素数の合計を取得する関数

    MyMatrix Add(const MyMatrix &matRight) const;                                                       // 行列の足し算をする関数。※行列の足し算は互いの行の個数と列の個数が等しい場合のみ可能。計算できない場合は無効な行列を返す。
    MyMatrix Sub(const MyMatrix &matRight) const;                                                       // 行列の引き算をする関数。※行列の引き算は互いの行の個数と列の個数が等しい場合のみ可能。計算できない場合は無効な行列を返す。
    MyMatrix ScaleScalar(float scalar) const;                                                           // 行列のスカラー倍をする関数。※スカラー倍は使用する行列が無効な行列以外、計算が可能。計算できない場合は無効な行列を返す。
    MyMatrix ScaleMatrix(const MyMatrix &matRight) const;                                               // 行列同士の掛け算をする関数。※行列の掛け算は (leftの列数) == (rightの行) の場合のみ可能。計算できない場合は無効な行列を返す。
    MyMatrix ScaleElement(const MyMatrix &matRight) const;                                              // 行列同士のアダマール積をする関数。※途中、計算に失敗した場合は何も行わない

    MyMatrix &AddAss(const MyMatrix &matRight);                                                         // 行列の足し算を行い、自身の値に加える。※途中、計算に失敗した場合は何も行わない
    MyMatrix &SubAss(const MyMatrix &matRight);                                                         // 行列の引き算を行い、自身の値に加える。※途中、計算に失敗した場合は何も行わない
    MyMatrix &ScaleScalarAss(float scalar);                                                             // 行列のスカラー倍を行い、自身の値に加える。※途中、計算に失敗した場合は何も行わない
    MyMatrix &ScaleMatrixAss(const MyMatrix &matRight);                                                 // 行列同士の掛け算を行い、自身の値に加える。※途中、計算に失敗した場合は何も行わない
    MyMatrix &ScaleElementAss(const MyMatrix &matRight);                                                // 行列同士のアダマール積を行い、自身の値に加える。※途中、計算に失敗した場合は何も行わない

    float GetDeterminant() const;                                                                       // 行列式を行う関数。※行列式は正方行列の場合以外に使わないでください。    
    MyMatrix GetAdjugate(unsigned long targetRow, unsigned long targetColumn) const;                    // 余因子行列を取得する関数。※この関数は正方行列の場合のみ可能。計算できない場合は無効な行列を返す。
    MyMatrix GetCofactorExpansion(unsigned long targetRow, unsigned long targetColumn) const;           // 余因子展開を行う関数。※この関数は正方行列の場合のみ可能。計算できない場合は無効な行列を返す。
    MyMatrix GetInverse() const;                                                                        // 逆行列を取得する関数(掃き出し法)
    MyMatrix GetTranspose() const;                                                                      // 転置行列を取得する関数

    void SetParam(unsigned long row, unsigned long column, float param);                                // 実際の値を設定する関数

    unsigned long GetRowCount() const;                                                                  // 行の数を取得する関数
    unsigned long GetColumnCount() const;                                                               // 列の数を取得する関数
    float GetParam(unsigned long row, unsigned long column) const;                                      // 実際の値を取得する関数
    float *GetMemory() const;                                                                           // 実際の値が入っているメモリのアドレスを取得する関数

    static MyMatrix GetDeactivate();                                                                    // 無効な行列を取得する関数
    static MyMatrix GetIdentity(unsigned long dimension);                                               // 単位行列を取得する関数
    static MyMatrix GetZero(unsigned long dimension);                                                   // 零行列を取得する関数
    static MyMatrix GetScaling2D(float x, float y);                                                     // 2Dベクトルの拡大・縮小が出来る行列を取得する関数
    static MyMatrix GetRotation2D(float radian);                                                        // 2Dベクトルの回転(2DなのでZ軸のみ)が出来る行列を取得する関数
    static MyMatrix GetTransportion2D(float x, float y);                                                // 2Dベクトルの平行移動が出来る行列を取得する関数
    static MyMatrix GetScaling3D(float x, float y, float z);                                            // 3Dベクトルの拡大・縮小が出来る行列を取得する関数
    static MyMatrix GetRotation3DX(float radian);                                                       // 3DベクトルのX軸回転が出来る行列を取得する関数
    static MyMatrix GetRotation3DY(float radian);                                                       // 3DベクトルのY軸回転が出来る行列を取得する関数
    static MyMatrix GetRotation3DZ(float radian);                                                       // 3DベクトルのZ軸回転が出来る行列を取得する関数
    static MyMatrix GetRotationQuaternion3D(float axisX, float axisY, float axisZ, float radian);       // 3Dベクトルの任意軸の回転が出来る行列を取得する関数
    static MyMatrix GetTransportion3D(float x, float y, float z);                                       // 3Dベクトルの平行移動が出来る行列を取得する関数

    MyMatrix operator +() const;                                                                        // 全ての符号を+にした行列を取得する
    MyMatrix operator -() const;                                                                        // 全ての符号を-にした行列を取得する
    MyMatrix &operator =(const MyMatrix &mat);                                                          // コピー
    MyMatrix &operator =(MyMatrix &&mat) noexcept;                                                      // ムーブ

    MyMatrix &operator +=(const MyMatrix &mat);                                                         // 行列和を代入する
    MyMatrix &operator -=(const MyMatrix &mat);                                                         // 行列差を代入する
    MyMatrix &operator *=(const float &scaler);                                                         // スカラー積を代入する
    MyMatrix &operator *=(const MyMatrix &mat);                                                         // 行列積を代入する
    MyMatrix &operator %=(const MyMatrix &mat);                                                         // アダマール積を代入する

    MyMatrixRow operator [](unsigned long row);                                                         // 自身の行を抜粋する

#ifdef USE_MY_VECTOR2D
    VECTOR2D To2VD() const;                                                                             // 行列を2次元ベクトルに変換する関数
    static MyMatrix GetScaling2D(const VECTOR2D &scaling);                                              // VECTOR2Dの拡大・縮小が出来る行列を取得する関数
    static MyMatrix GetTransportion2D(const VECTOR2D &transportion);                                    // VECTOR2Dの平行移動が出来る行列を取得する関数
#endif // USE_MY_VECTOR2D

#ifdef USE_MY_VECTOR3D
    VECTOR3D To3VD() const;                                                                             // 行列を3次元ベクトルに変換する関数
    static MyMatrix GetScaling3D(const VECTOR3D &scaling);                                              // 3Dベクトルの拡大・縮小が出来る行列を取得する関数
#ifdef USE_MY_ROTATION
    static MyMatrix GetRotation3D(const VECTOR3D &rotateRadian);                                        // 3Dベクトルのx軸, y軸, z軸の回転がまとめて出来る行列を取得する関数
    static MyMatrix GetRotation3D(const VECTOR3D &rotateRadian, EulerOrder order);                      // 3Dベクトルのx軸, y軸, z軸の回転がまとめて出来る行列を取得する関数
    static MyMatrix GetLookAt(const VECTOR3D &forword, const VECTOR3D &localUp);                        // 姿勢行列を取得する関数
#endif // USE_MY_ROTATION
    static MyMatrix GetRotationQuaternion3D(const VECTOR3D &axisVec, float radian);                     // 3Dベクトルの任意軸の回転が出来る行列を取得する関数
    static MyMatrix GetTransportion3D(const VECTOR3D &transportion);                                    // 3Dベクトルの平行移動が出来る行列を取得する関数
#endif // USE_MY_VECTOR3D

#ifdef USE_MY_ROTATION
#ifdef USE_MY_QUATERNION
    MyQuaternion ToQtn() const;
#endif // USE_MY_QUATERNION

#ifdef USE_MY_EULERANGLE
    MyEulerAngle ToEulerAngle() const;
    MyEulerAngle ToEulerAngle(EulerOrder order) const;
#endif // USE_MY_EULERANGLE
#endif // USE_MY_ROTATION
};

MyMatrix operator +(const MyMatrix &matL, const MyMatrix &matR);                                        // 行列和
MyMatrix operator -(const MyMatrix &matL, const MyMatrix &matR);                                        // 行列差
MyMatrix operator *(const MyMatrix &mat, const float &scaler);                                          // スカラー倍
MyMatrix operator *(const MyMatrix &matL, const MyMatrix &matR);                                        // 行列積
MyMatrix operator %(const MyMatrix &matL, const MyMatrix &matR);                                        // アダマール積

// 行の個数、列の個数を自由に決められる行列のクラス(double型)
class MyMatrix_D
{
private:
    unsigned long mnRowCount;                                                                           // 行の数
    unsigned long mnColumnCount;                                                                        // 列の数
    MallocData<double> mlParam;                                                                         // それぞれの値の数(中途半端にヒープ領域を使いたくなかったのでstd::vectorにしています)

public:
    // 自身の行コンテキスト
    class MyMatrixRow_D;

    // 自身の列コンテキスト
    class MyMatrixColumn_D;

    MyMatrix_D(unsigned long rowCount, unsigned long columnCount);                                      // コンストラクタ
    MyMatrix_D(const MyMatrix_D &srcMatrix);                                                            // コピーコンストラクタ
    MyMatrix_D(MyMatrix_D &&srcMatrix) noexcept;                                                        // ムーブコンストラクタ
    ~MyMatrix_D();                                                                                      // デストラクタ

    MyMatrix_D &Copy(const MyMatrix_D &mat);                                                            // コピー
    MyMatrix_D &Move(MyMatrix_D &mat) noexcept;                                                         // ムーブ
    MyMatrix_D &Move(MyMatrix_D &&mat) noexcept;                                                        // ムーブ

    bool CheckActive() const;                                                                           // この行列が有効なものかを確認する関数
    bool CheckSquare() const;                                                                           // この行列が正方行列かを確認する関数
    unsigned long GetSize() const;                                                                      // この行列の要素数の合計を取得する関数

    MyMatrix_D Add(const MyMatrix_D &matRight) const;                                                   // 行列の足し算をする関数。※行列の足し算は互いの行の個数と列の個数が等しい場合のみ可能。計算できない場合は無効な行列を返す。
    MyMatrix_D Sub(const MyMatrix_D &matRight) const;                                                   // 行列の引き算をする関数。※行列の引き算は互いの行の個数と列の個数が等しい場合のみ可能。計算できない場合は無効な行列を返す。
    MyMatrix_D ScaleScalar(double scalar) const;                                                        // 行列のスカラー倍をする関数。※スカラー倍は使用する行列が無効な行列以外、計算が可能。計算できない場合は無効な行列を返す。
    MyMatrix_D ScaleMatrix(const MyMatrix_D &matRight) const;                                           // 行列同士の掛け算をする関数。※行列の掛け算は (leftの列数) == (rightの行) の場合のみ可能。計算できない場合は無効な行列を返す。
    MyMatrix_D ScaleElement(const MyMatrix_D &matRight) const;                                          // 行列同士のアダマール積をする関数。※途中、計算に失敗した場合は何も行わない

    MyMatrix_D &AddAss(const MyMatrix_D &matRight);                                                     // 行列の足し算を行い、自身の値に加える。※途中、計算に失敗した場合は何も行わない
    MyMatrix_D &SubAss(const MyMatrix_D &matRight);                                                     // 行列の引き算を行い、自身の値に加える。※途中、計算に失敗した場合は何も行わない
    MyMatrix_D &ScaleScalarAss(double scalar);                                                          // 行列のスカラー倍を行い、自身の値に加える。※途中、計算に失敗した場合は何も行わない
    MyMatrix_D &ScaleMatrixAss(const MyMatrix_D &matRight);                                             // 行列同士の掛け算を行い、自身の値に加える。※途中、計算に失敗した場合は何も行わない
    MyMatrix_D &ScaleElementAss(const MyMatrix_D &matRight);                                            // 行列同士のアダマール積を行い、自身の値に加える。※途中、計算に失敗した場合は何も行わない

    double GetDeterminant() const;                                                                      // 行列式を行う関数。※行列式は正方行列の場合以外に使わないでください。    
    MyMatrix_D GetAdjugate(unsigned long targetRow, unsigned long targetColumn) const;                  // 余因子行列を取得する関数。※この関数は正方行列の場合のみ可能。計算できない場合は無効な行列を返す。
    MyMatrix_D GetCofactorExpansion(unsigned long targetRow, unsigned long targetColumn) const;         // 余因子展開を行う関数。※この関数は正方行列の場合のみ可能。計算できない場合は無効な行列を返す。
    MyMatrix_D GetInverse() const;                                                                      // 逆行列を取得する関数(掃き出し法)
    MyMatrix_D GetTranspose() const;                                                                    // 転置行列を取得する関数

    void SetParam(unsigned long row, unsigned long column, double param);                               // 実際の値を設定する関数

    unsigned long GetRowCount() const;                                                                  // 行の数を取得する関数
    unsigned long GetColumnCount() const;                                                               // 列の数を取得する関数
    double GetParam(unsigned long row, unsigned long column) const;                                     // 実際の値を取得する関数
    double *GetMemory() const;                                                                          // 実際の値が入っているメモリのアドレスを取得する関数
    double *GetMemoryRow(unsigned long row) const;                                                      // 実際の値が入っているメモリのアドレスを取得する関数
    MallocData<double *> GetMemoryColumn(unsigned long column) const;                                   // 実際の値が入っているメモリのアドレスを取得する関数
    double *GetMemoryPoint(unsigned long row, unsigned long column) const;                              // 実際の値が入っているメモリのアドレスを取得する関数

    static MyMatrix_D GetDeactivate();                                                                  // 無効な行列を取得する関数
    static MyMatrix_D GetIdentity(unsigned long dimension);                                             // 単位行列を取得する関数
    static MyMatrix_D GetZero(unsigned long dimension);                                                 // 零行列を取得する関数
    static MyMatrix_D GetScaling2D(double x, double y);                                                 // 2Dベクトルの拡大・縮小が出来る行列を取得する関数
    static MyMatrix_D GetRotation2D(double radian);                                                     // 2Dベクトルの回転(2DなのでZ軸のみ)が出来る行列を取得する関数
    static MyMatrix_D GetTransportion2D(double x, double y);                                            // 2Dベクトルの平行移動が出来る行列を取得する関数
    static MyMatrix_D GetScaling3D(double x, double y, double z);                                       // 3Dベクトルの拡大・縮小が出来る行列を取得する関数
    static MyMatrix_D GetRotation3DX(double radian);                                                    // 3DベクトルのX軸回転が出来る行列を取得する関数
    static MyMatrix_D GetRotation3DY(double radian);                                                    // 3DベクトルのY軸回転が出来る行列を取得する関数
    static MyMatrix_D GetRotation3DZ(double radian);                                                    // 3DベクトルのZ軸回転が出来る行列を取得する関数
    static MyMatrix_D GetRotationQuaternion3D(double axisX, double axisY, double axisZ, double radian); // 3Dベクトルの任意軸の回転が出来る行列を取得する関数
    static MyMatrix_D GetTransportion3D(double x, double y, double z);                                  // 3Dベクトルの平行移動が出来る行列を取得する関数

    MyMatrix_D operator +() const;                                                                      // 全ての符号を+にした行列を取得する
    MyMatrix_D operator -() const;                                                                      // 全ての符号を-にした行列を取得する
    MyMatrix_D &operator =(const MyMatrix_D &mat);                                                      // コピー
    MyMatrix_D &operator =(MyMatrix_D &&mat) noexcept;                                                  // ムーブ

    MyMatrix_D &operator +=(const MyMatrix_D &mat);                                                     // 行列和を代入する
    MyMatrix_D &operator -=(const MyMatrix_D &mat);                                                     // 行列差を代入する
    MyMatrix_D &operator *=(const double &scaler);                                                      // スカラー積を代入する
    MyMatrix_D &operator *=(const MyMatrix_D &mat);                                                     // 行列積を代入する
    MyMatrix_D &operator %=(const MyMatrix_D &mat);                                                     // アダマール積を代入する

    MyMatrixRow_D operator [](unsigned long row);                                                       // 自身の行を抜粋する

#ifdef USE_MY_VECTOR2D
    VECTOR2D_D To2VD() const;                                                                           // 行列を2次元ベクトルに変換する関数
    static MyMatrix_D GetScaling2D(const VECTOR2D_D &scaling);                                          // VECTOR2Dの拡大・縮小が出来る行列を取得する関数
    static MyMatrix_D GetTransportion2D(const VECTOR2D_D &transportion);                                // VECTOR2Dの平行移動が出来る行列を取得する関数
#endif // USE_MY_VECTOR2D

#ifdef USE_MY_VECTOR3D
    VECTOR3D_D To3VD() const;                                                                           // 行列を3次元ベクトルに変換する関数
    static MyMatrix_D GetScaling3D(const VECTOR3D_D &scaling);                                          // 3Dベクトルの拡大・縮小が出来る行列を取得する関数
#ifdef USE_MY_ROTATION
    static MyMatrix_D GetRotation3D(const VECTOR3D_D &rotateRadian);                                    // 3Dベクトルのx軸, y軸, z軸の回転がまとめて出来る行列を取得する関数
    static MyMatrix_D GetRotation3D(const VECTOR3D_D &rotateRadian, EulerOrder order);                  // 3Dベクトルのx軸, y軸, z軸の回転がまとめて出来る行列を取得する関数
    static MyMatrix_D GetLookAt(const VECTOR3D_D &forword, const VECTOR3D_D &localUp);                  // 姿勢行列を取得する関数
#endif // USE_MY_ROTATION
    static MyMatrix_D GetRotationQuaternion3D(const VECTOR3D_D &axisVec, double radian);                // 3Dベクトルの任意軸の回転が出来る行列を取得する関数
    static MyMatrix_D GetTransportion3D(const VECTOR3D_D &transportion);                                // 3Dベクトルの平行移動が出来る行列を取得する関数
#endif // USE_MY_VECTOR3D

#ifdef USE_MY_ROTATION
#ifdef USE_MY_QUATERNION
    MyQuaternion ToQtn() const;
#endif // USE_MY_QUATERNION

#ifdef USE_MY_EULERANGLE
    MyEulerAngle ToEulerAngle() const;
    MyEulerAngle ToEulerAngle(EulerOrder order) const;
#endif // USE_MY_EULERANGLE
#endif // USE_MY_ROTATION
};

MyMatrix_D operator +(const MyMatrix_D &matL, const MyMatrix_D &matR);                                  // 行列和
MyMatrix_D operator -(const MyMatrix_D &matL, const MyMatrix_D &matR);                                  // 行列差
MyMatrix_D operator *(const MyMatrix_D &mat, const double &scaler);                                     // スカラー倍
MyMatrix_D operator *(const MyMatrix_D &matL, const MyMatrix_D &matR);                                  // 行列積
MyMatrix_D operator %(const MyMatrix_D &matL, const MyMatrix_D &matR);                                  // アダマール積

#endif // USE_MY_MATRIX