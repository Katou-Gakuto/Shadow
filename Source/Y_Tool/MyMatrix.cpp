#include "MyMatrix.h"

#if (USE_MY_VECTOR2D || USE_MY_VECTOR3D)
#include "VECTOR.h"
#include "VECTOR_D.h"
#endif // (USE_MY_VECTOR2D || USE_MY_VECTOR3D)

#ifdef USE_MY_QUATERNION
#include "MyQuaternion.h"
#endif // USE_MY_QUATERNION

#ifdef USE_MY_EULERANGLE
#include "MyEulerAngle.h"
#endif // USE_MY_EULERANGLE

#ifdef USE_MY_MATRIX

// 行列の行コンテキスト
class MyMatrix::MyMatrixRow
{
private:
    float *mpRowBox;

public:
    MyMatrixRow(const MyMatrix &mat, unsigned long row);                            // 行コンテキストのコンストラクタ
    ~MyMatrixRow();                                                                   // 行コンテキストのデストラクタ
    float &operator[](unsigned long index);                                             // 行の[]オペレータ
    const float &operator[](unsigned long index) const;                                 // 行の[]オペレータ
};

// 行コンテキストのコンストラクタ
MyMatrix::MyMatrixRow::MyMatrixRow(const MyMatrix &mat, unsigned long row) :
    mpRowBox(nullptr)
{
    // 1行目1列のデータ開始位置を取得(offset)
    float *offset = (float *)(mat.mlParam.Memory());

    // offsetから目的の行の開始位置までの距離を取得する
    auto distance = sizeof(float) * (mat.mnRowCount * static_cast<unsigned long long>(row));

    // offsetとdistanceを加えて、目的の行の開始位置を取得する
    this->mpRowBox = offset + distance;
}

// 行コンテキストのデストラクタ
MyMatrix::MyMatrixRow::~MyMatrixRow()
{
    this->mpRowBox = nullptr;
}

// 行の[]オペレータ
float &MyMatrix::MyMatrixRow::operator[](unsigned long index)
{
    return this->mpRowBox[index];
}

// 行の[]オペレータ
const float &MyMatrix::MyMatrixRow::operator[](unsigned long index) const
{
    return this->mpRowBox[index];
}

// 行列の列コンテキスト
class MyMatrix::MyMatrixColumn
{
private:
    MallocData<float> mlColumnBox;

public:
    MyMatrixColumn(const MyMatrix &mat, unsigned long column);                      // 列コンテキストのコンストラクタ
    ~MyMatrixColumn();                                                                // 行コンテキストのデストラクタ
    float &operator[](unsigned long index);                                             // 列の[]オペレータ
    const float &operator[](unsigned long index) const;                                 // 列の[]オペレータ
};

// 列コンテキストのコンストラクタ
MyMatrix::MyMatrixColumn::MyMatrixColumn(const MyMatrix &mat, unsigned long column) :
    mlColumnBox(mat.mnColumnCount, 0.0f)
{
    // 行を進めていく
    for (unsigned i = 0; i < mat.mnRowCount;)
    {
        // 自身の配列に、指定の列の値を代入する
        this->mlColumnBox[i] = mat.mlParam[mat.mnRowCount * i + column];
    }
}

// 列コンテキストのデストラクタ
MyMatrix::MyMatrixColumn::~MyMatrixColumn()
{
    // この関数が終わったタイミングでMallocDataを破棄します
}

// 列の[]オペレータ
float &MyMatrix::MyMatrixColumn::operator[](unsigned long index)
{
    return this->mlColumnBox[index];
}

// 列の[]オペレータ
const float &MyMatrix::MyMatrixColumn::operator[](unsigned long index) const
{
    return this->mlColumnBox[index];
}

// コンストラクタ
MyMatrix::MyMatrix(unsigned long rowCount, unsigned long columnCount) :
    mnRowCount(rowCount),
    mnColumnCount(columnCount),
    mlParam((int)(rowCount * columnCount))
{
    for (int i = 0; i < (int)(rowCount * columnCount); i++)
    {
        this->mlParam[i] = 0;
    }
}

// コピーコンストラクタ
MyMatrix::MyMatrix(const MyMatrix &srcMatrix) :
    mnRowCount(srcMatrix.mnRowCount),
    mnColumnCount(srcMatrix.mnColumnCount),
    mlParam()
{
    // 行列の中身である、配列部もコピーする
    this->mlParam.Copy(srcMatrix.mlParam);
}

// ムーブコンストラクタ
MyMatrix::MyMatrix(MyMatrix &&srcMatrix) noexcept :
    mnRowCount(srcMatrix.mnRowCount),
    mnColumnCount(srcMatrix.mnColumnCount),
    mlParam()
{
    // 行列の中身である、配列部もムーブする
    this->mlParam.Move(srcMatrix.mlParam);
}

// デストラクタ
MyMatrix::~MyMatrix()
{
    // この関数が終わったタイミングでMallocDataを破棄します
}

// コピー
MyMatrix &MyMatrix::Copy(const MyMatrix &mat)
{
    // 行の個数をコピーする
    this->mnRowCount = mat.mnRowCount;

    // 列の個数をコピーする
    this->mnColumnCount = mat.mnColumnCount;

    // 配列部をコピーする
    this->mlParam.Copy(mat.mlParam);

    // 返り値を返す
    return *this;
}

// ムーブ
MyMatrix &MyMatrix::Move(MyMatrix &mat) noexcept
{
    // 行の個数をコピーする
    this->mnRowCount = mat.mnRowCount;

    // 列の個数をコピーする
    this->mnColumnCount = mat.mnColumnCount;

    // 配列部をコピーする
    this->mlParam.Move(mat.mlParam);

    // 返り値を返す
    return *this;
}

// ムーブ
MyMatrix &MyMatrix::Move(MyMatrix &&mat) noexcept
{
    // 行の個数をコピーする
    this->mnRowCount = mat.mnRowCount;

    // 列の個数をコピーする
    this->mnColumnCount = mat.mnColumnCount;

    // 配列部をコピーする
    this->mlParam.Move(mat.mlParam);

    // 返り値を返す
    return *this;
}

// この行列が有効なものかを確認する関数
bool MyMatrix::CheckActive() const
{
    return ((this->mnRowCount != 0) && (this->mnColumnCount != 0));
}

// この行列が正方行列かを確認する関数
bool MyMatrix::CheckSquare() const
{
    return this->mnRowCount == this->mnColumnCount;
}

// この行列の要素数の合計を取得する関数
unsigned long MyMatrix::GetSize() const
{
    return this->mnRowCount * this->mnColumnCount;
}

// 行列の足し算をする関数。
// ※行列の足し算は互いの行の個数と列の個数が等しい場合のみ可能。計算できない場合は無効な行列を返す。
MyMatrix MyMatrix::Add(const MyMatrix &matRight) const
{
    if (this->CheckActive() &&
        matRight.CheckActive() &&
        this->mnRowCount == matRight.mnRowCount &&
        this->mnColumnCount == matRight.mnColumnCount)
    {
        MyMatrix temp(this->mnRowCount, this->mnColumnCount);
        for (int i = 0; i < (int)(this->GetSize()); i++)
        {
            temp.mlParam[i] = this->mlParam[i] + matRight.mlParam[i];
        }
        return temp;
    }
    
    // 無効な行列を返す
    return MyMatrix::GetDeactivate();
}

// 行列の引き算をする関数。
// ※行列の引き算は互いの行の個数と列の個数が等しい場合のみ可能。計算できない場合は無効な行列を返す。
MyMatrix MyMatrix::Sub(const MyMatrix &matRight) const
{
    if (this->CheckActive() &&
        matRight.CheckActive() &&
        this->mnRowCount == matRight.mnRowCount &&
        this->mnColumnCount == matRight.mnColumnCount)
    {
        MyMatrix temp(this->mnRowCount, this->mnColumnCount);
        for (int i = 0; i < (int)(this->GetSize()); i++)
        {
            temp.mlParam[i] = this->mlParam[i] - matRight.mlParam[i];
        }
        return temp;
    }

    // 無効な行列を返す
    return MyMatrix::GetDeactivate();
}

// 行列のスカラー倍をする関数。
// ※スカラー倍の関数はどんな行列とも計算が可能。全てのパラメータとスカラーをかける。
MyMatrix MyMatrix::ScaleScalar(float scalar) const
{
    if (this->CheckActive())
    {
        MyMatrix temp(this->mnRowCount, this->mnColumnCount);
        for (int i = 0; i < (int)(this->GetSize()); i++)
        {
            temp.mlParam[i] = this->mlParam[i] * scalar;
        }
        return temp;
    }

    // 無効な行列を返す
    return MyMatrix::GetDeactivate();
}

// 行列同士の掛け算をする関数。
// ※行列の掛け算は (leftの列数) == (rightの行) の場合のみ可能。計算できない場合は無効な行列を返す。
MyMatrix MyMatrix::ScaleMatrix(const MyMatrix &matRight) const
{
    if (this->CheckActive() &&
        matRight.CheckActive() &&
        this->mnColumnCount == matRight.mnRowCount)
    {
        MyMatrix temp(this->mnRowCount, matRight.mnColumnCount);
        float param = 0;
        for (unsigned long currentRow = 0; currentRow < temp.mnRowCount; currentRow++)
        {
            for (unsigned long currentColumn = 0; currentColumn < temp.mnColumnCount; currentColumn++)
            {
                for (unsigned long count = 0; count < this->mnColumnCount; count++)
                {
                    param += this->GetParam(currentRow, count) * matRight.GetParam(count, currentColumn);
                }
                temp.SetParam(currentRow, currentColumn, param);
                param = 0;
            }
        }

        return temp;
    }

    // 無効な行列を返す
    return MyMatrix::GetDeactivate();
}

// 行列同士のアダマール積をする関数。
// ※途中、計算に失敗した場合は何も行わない
MyMatrix MyMatrix::ScaleElement(const MyMatrix &matRight) const
{
    if (this->CheckActive() &&
        matRight.CheckActive() &&
        this->mnRowCount == matRight.mnRowCount &&
        this->mnColumnCount == matRight.mnColumnCount)
    {
        MyMatrix temp(this->mnRowCount, this->mnColumnCount);
        for (int i = 0; i < (int)(this->GetSize()); i++)
        {
            temp.mlParam[i] = this->mlParam[i] * matRight.mlParam[i];
        }
        return temp;
    }

    // 無効な行列を返す
    return MyMatrix::GetDeactivate();
}

// 行列の足し算を行い、自身の値に加える。
// ※途中、計算に失敗した場合は何も行わない
MyMatrix &MyMatrix::AddAss(const MyMatrix &matRight)
{
    MyMatrix temp = this->Add(matRight);
    if (temp.CheckActive())
    {
        *this = temp;
    }
    return *this;
}

// 行列の引き算を行い、自身の値に加える。
// ※途中、計算に失敗した場合は何も行わない
MyMatrix &MyMatrix::SubAss(const MyMatrix &matRight)
{
    MyMatrix temp = this->Sub(matRight);
    if (temp.CheckActive())
    {
        *this = temp;
    }
    return *this;
}

// 行列のスカラー倍を行い、自身の値に加える。
// ※途中、計算に失敗した場合は何も行わない
MyMatrix &MyMatrix::ScaleScalarAss(float scalar)
{
    MyMatrix temp = this->ScaleScalar(scalar);
    if (temp.CheckActive())
    {
        *this = temp;
    }
    return *this;
}

// 行列同士の掛け算を行い、自身の値に加える。
// ※途中、計算に失敗した場合は何も行わない
MyMatrix &MyMatrix::ScaleMatrixAss(const MyMatrix &matRight)
{
    MyMatrix temp = this->ScaleMatrix(matRight);
    if (temp.CheckActive())
    {
        *this = temp;
    }
    return *this;
}

// 行列同士のアダマール積を行い、自身の値に加える。
// ※途中、計算に失敗した場合は何も行わない
MyMatrix &MyMatrix::ScaleElementAss(const MyMatrix &matRight)
{
    MyMatrix temp = this->ScaleElement(matRight);
    if (temp.CheckActive())
    {
        *this = temp;
    }
    return *this;
}

// 行列式を行う関数
float MyMatrix::GetDeterminant() const
{
    // この計算式は(https://www.krrk0.com/determinant-calculation/)を参考にしています。
    float temp = 0;
    if (this->CheckActive() &&
        this->CheckSquare())
    {
        if (this->mnRowCount == 2)
        {
            temp = (this->GetParam(0, 0) * this->GetParam(1, 1)) - (this->GetParam(0, 1) * this->GetParam(1, 0));
        }
        else if (this->mnRowCount == 3)
        {
            temp = this->GetParam(0, 0) * this->GetParam(1, 1) * this->GetParam(2, 2);
            temp += this->GetParam(1, 0) * this->GetParam(2, 1) * this->GetParam(0, 2);
            temp += this->GetParam(2, 0) * this->GetParam(0, 1) * this->GetParam(1, 2);
            temp -= this->GetParam(2, 0) * this->GetParam(1, 1) * this->GetParam(0, 2);
            temp -= this->GetParam(1, 0) * this->GetParam(0, 1) * this->GetParam(2, 2);
            temp -= this->GetParam(0, 0) * this->GetParam(2, 1) * this->GetParam(1, 2);
        }
        else if (this->mnRowCount == 4)
        {
            // 余因子展開を使った方法
            // とりあえず4でとどめておきます。

            // 余因子展開を行う
            MyMatrix mat0 = this->GetCofactorExpansion(0, 0);
            MyMatrix mat1 = this->GetCofactorExpansion(1, 0);
            MyMatrix mat2 = this->GetCofactorExpansion(2, 0);
            MyMatrix mat3 = this->GetCofactorExpansion(3, 0);

            // 作った3次行列の行列式を解く
            float mat0Determinant = mat0.GetDeterminant();
            float mat1Determinant = mat1.GetDeterminant();
            float mat2Determinant = mat2.GetDeterminant();
            float mat3Determinant = mat3.GetDeterminant();

            // 行列式の答えを全て加算したものをtempに代入する。
            temp = mat0Determinant + mat1Determinant + mat2Determinant + mat3Determinant;
            
            /*
            // (https://thira.plavox.info/blog/2008/06/_c.html#google_vignette)を参考にした、四次以上の方法
            // エラーが起きるらしい。多分0除算のエラーっぽい？
            float buffer = 0;
            MyMatrix mat = *this;
            unsigned long dimension = this->mnRowCount;
            for (unsigned long i = 0; i < dimension; i++)
            {
                for (unsigned long j = 0; j < dimension; j++)
                {
                    if (i < j)
                    {
                        buffer = mat.GetParam(j, i) / mat.GetParam(i, i);
                        for (unsigned long k = 0; k < dimension; k++)
                        {
                            mat.SetParam(j, k, (mat.GetParam(j, k) - (mat.GetParam(i, k) * buffer)));
                        }
                    }
                }
            }

            // 答えを出すための準備
            temp = 1;

            //対角部分の積で答えを出す
            for (unsigned long i = 0; i < dimension; i++)
            {
                temp *= mat.GetParam(i, i);
            }
            */
        }
    }

    // 返り値を返す
    return temp;
}

// 余因子行列を取得する関数
MyMatrix MyMatrix::GetAdjugate(unsigned long targetRow, unsigned long targetColumn) const
{
    if (this->CheckSquare())
    {
        unsigned long rowSize = this->mnRowCount - 1;
        unsigned long columnSize = this->mnColumnCount - 1;
        MyMatrix abjMatrix(rowSize, columnSize);

        unsigned long useRow = 0;
        unsigned long useColumn = 0;
        for (unsigned long currentRow = 0; currentRow < rowSize; currentRow++)
        {
            if (currentRow == targetRow)
            {
                useRow++;
            }

            for (unsigned long currentColumn = 0; currentColumn < columnSize; currentColumn++)
            {
                if (currentColumn == targetColumn)
                {
                    useColumn++;
                }

                abjMatrix.SetParam(currentRow, currentColumn, this->GetParam(useRow, useColumn));
                useColumn++;
            }

            useColumn = 0;

            useRow++;
        }

        return abjMatrix;
    }
    
    // 無効な行列を返す
    return MyMatrix::GetDeactivate();
}

// 余因子展開を行う関数
MyMatrix MyMatrix::GetCofactorExpansion(unsigned long targetRow, unsigned long targetColumn) const
{
    // 余因子行列を取得する
    MyMatrix temp = this->GetAdjugate(targetRow, targetColumn);

    // 行列が有効なものかを確認する
    if (temp.CheckActive())
    {
        // 注目している数字を取得する
        float currentParam = this->GetParam(targetRow, targetColumn);

        // 注目している数字で掛け算を行う
        MyMatrix abj = temp.ScaleScalar(currentParam);

        // targetRow + targetcolumn の結果が奇数あれば、値を反転させる
        if (((targetRow + targetColumn) % 2) == 1)
        {
            abj = -temp;
        }

        // 値を返す
        return  abj;
    }

    // 無効な行列を返す
    return MyMatrix::GetDeactivate();
}

// 逆行列を取得する関数。
MyMatrix MyMatrix::GetInverse() const
{
    if (this->CheckActive() &&
        this->GetDeterminant() != 0.0f)
    {
        // 渡された行列が正方行列だった場合は処理を行う
        if (this->CheckSquare())
        {
            // この計算式は(https://thira.plavox.info/blog/2008/06/_c.html#google_vignette)を参考にしています。

            // 行列の次元を保持しておく
            unsigned long dimension = this->GetRowCount();

            // 逆行列が入る行列
            MyMatrix inv = GetIdentity(dimension);
            MyMatrix temp = *this;

            // その他作業用変数
            float buffer = 0.0f;

            //掃き出し法
            for (unsigned long i = 0; i < dimension; i++)
            {
                buffer = 1 / temp.GetParam(i, i);
                for (unsigned long j = 0; j < dimension; j++)
                {
                    temp.SetParam(i, j, (temp.GetParam(i, j) * buffer));
                    inv.SetParam(i, j, (inv.GetParam(i, j) * buffer));
                }
                for (unsigned long j = 0; j < dimension; j++)
                {
                    if (i != j)
                    {
                        buffer = temp.GetParam(j, i);
                        for (unsigned long k = 0; k < dimension; k++)
                        {
                            temp.SetParam(j, k, (temp.GetParam(j, k) - (temp.GetParam(i, k) * buffer)));
                            inv.SetParam(j, k, (inv.GetParam(j, k) - (inv.GetParam(i, k) * buffer)));
                        }
                    }
                }
            }

            return inv;
        }
    }

    // 無効な行列を返す
    return MyMatrix::GetDeactivate();
}

// 転置行列を取得する関数
MyMatrix MyMatrix::GetTranspose() const
{
    if (this->CheckActive() == false)
    {
        return MyMatrix::GetDeactivate();
    }

    MyMatrix temp(this->mnColumnCount, this->mnRowCount);
    for (unsigned long thisRow = 0; thisRow < this->mnRowCount; thisRow++)
    {
        for (unsigned long thisColumn = 0; thisColumn < this->mnColumnCount; thisColumn++)
        {
            temp.SetParam(thisColumn, thisRow, this->GetParam(thisRow, thisColumn));
        }
    }

    return temp;
}

// 実際の値を設定する関数
void MyMatrix::SetParam(unsigned long row, unsigned long column, float param)
{
    unsigned long index = (row * this->mnColumnCount) + column;
    this->mlParam[(int)(index)] = param;
}

// 行の数を取得する関数
unsigned long MyMatrix::GetRowCount() const
{
    return this->mnRowCount;
}

// 列の数を取得する関数
unsigned long MyMatrix::GetColumnCount() const
{
    return this->mnColumnCount;
}

// 実際の値を取得する関数
float MyMatrix::GetParam(unsigned long row, unsigned long column) const
{
    unsigned long index = (row * this->mnColumnCount) + column;
    return this->mlParam[(int)(index)];
}

// 実際の値が入っているメモリのアドレスを取得する関数
float *MyMatrix::GetMemory() const
{
    return this->mlParam.Memory();
}

// 無効な行列を取得する関数
MyMatrix MyMatrix::GetDeactivate()
{
    return MyMatrix(0, 0);
}

// 単位行列を取得する関数
MyMatrix MyMatrix::GetIdentity(unsigned long dimension)
{
    if (2 <= dimension)
    {
        MyMatrix temp(dimension, dimension);
        for (unsigned long i = 0; i < dimension; i++)
        {
            for (unsigned long j = 0; j < dimension; j++)
            {
                if (i == j)
                {
                    temp.SetParam(i, j, 1);
                }
                else
                {
                    temp.SetParam(i, j, 0);
                }
            }
        }
        return temp;
    }

    // 無効な行列を返す
    return MyMatrix::GetDeactivate();
}

// 零行列を取得する関数
MyMatrix MyMatrix::GetZero(unsigned long dimension)
{
    if (2 <= dimension)
    {
        MyMatrix temp(dimension, dimension);
        for (unsigned long i = 0; i < dimension; i++)
        {
            for (unsigned long j = 0; j < dimension; j++)
            {
                temp.SetParam(i, j, 0);
            }
        }
        return temp;
    }

    // 無効な行列を返す
    return MyMatrix::GetDeactivate();
}

// VECTOR2Dの拡大・縮小が出来る行列を取得する関数
MyMatrix MyMatrix::GetScaling2D(float x, float y)
{
    MyMatrix temp(3, 3);
    temp.SetParam(0, 0, x);
    temp.SetParam(1, 1, y);
    temp.SetParam(2, 2, 1);
    return temp;
}

// VECTOR2Dの回転(2DなのでZ軸のみ)が出来る行列を取得する関数
MyMatrix MyMatrix::GetRotation2D(float radian)
{
    MyMatrix temp(3, 3);
    temp.SetParam(0, 0, cosf(radian));
    temp.SetParam(0, 1, sinf(radian));
    temp.SetParam(1, 0, -sinf(radian));
    temp.SetParam(1, 1, cosf(radian));
    temp.SetParam(2, 2, 1);
    return temp;
}

// VECTOR2Dの平行移動が出来る行列を取得する関数
MyMatrix MyMatrix::GetTransportion2D(float x, float y)
{
    MyMatrix temp(3, 3);
    temp.SetParam(2, 0, x);
    temp.SetParam(2, 1, y);
    temp.SetParam(2, 2, 1);
    return temp;
}

// ベクトルの拡大・縮小が出来る行列を取得する関数
MyMatrix MyMatrix::GetScaling3D(float x, float y, float z)
{
    MyMatrix temp = MyMatrix::GetIdentity(4);
    temp.SetParam(0, 0, x);
    temp.SetParam(1, 1, y);
    temp.SetParam(2, 2, z);
    temp.SetParam(3, 3, 1);
    return temp;
}

// VECTOR3DのX軸回転が出来る行列を取得する関数
MyMatrix MyMatrix::GetRotation3DX(float radian)
{
    MyMatrix temp = MyMatrix::GetIdentity(4);
    temp.SetParam(0, 0, 1);
    temp.SetParam(1, 1, cosf(radian));
    temp.SetParam(1, 2, sinf(radian));
    temp.SetParam(2, 1, -sinf(radian));
    temp.SetParam(2, 2, cosf(radian));
    temp.SetParam(3, 3, 1);
    return temp;

}

// VECTOR3DのY軸回転が出来る行列を取得する関数
MyMatrix MyMatrix::GetRotation3DY(float radian)
{
    MyMatrix temp = MyMatrix::GetIdentity(4);
    temp.SetParam(0, 0, cosf(radian));
    temp.SetParam(0, 2, -sinf(radian));
    temp.SetParam(1, 1, 1);
    temp.SetParam(2, 0, sinf(radian));
    temp.SetParam(2, 2, cosf(radian));
    temp.SetParam(3, 3, 1);
    return temp;
}

// VECTOR3DのZ軸回転が出来る行列を取得する関数
MyMatrix MyMatrix::GetRotation3DZ(float radian)
{
    MyMatrix temp = MyMatrix::GetIdentity(4);
    temp.SetParam(0, 0, cosf(radian));
    temp.SetParam(0, 1, sinf(radian));
    temp.SetParam(1, 0, -sinf(radian));
    temp.SetParam(1, 1, cosf(radian));
    temp.SetParam(2, 2, 1);
    temp.SetParam(3, 3, 1);
    return temp;
}

// 行列を用いてVECTOR3Dの任意の軸の回転(クォータニオンでの回転)を行う関数
MyMatrix MyMatrix::GetRotationQuaternion3D(float axisX, float axisY, float axisZ, float radian)
{
    // (https://qiita.com/MENDY/items/0508916a98688b1fb0cc)←これの任意軸回りを参考に
    // ↑は嘘。参考にしたのはココ→(http://marupeke296.com/DXG_No58_RotQuaternionTrans.html)

    // 計算方法は以下の通り
    // 対角成分のみ(対応するベクトルの要素の二乗) * (1 - cosf(radian))    +   cos(radian)
    // それ以外は  (ベクトルの要素二つ)           * (1 - cosf(radian)) + or - 残ったベクトルの要素 * sinf(radian)
    // っていう感じ。綺麗だね。

    MyMatrix temp = MyMatrix::GetIdentity(4);
    VECTOR3D axis;
    axis.SetXYZ(axisX, axisY, axisZ);
    VECTOR3D axisNorm = axis.Normalize();
    axisNorm.Normalize(); // XMMatrixRotationAxis と同様に正規化を行う

    float paramX = axisNorm.GetX();
    float paramY = axisNorm.GetY();
    float paramZ = axisNorm.GetZ();

    float cosA = cosf(radian);
    float sinA = sinf(radian);
    float oneMinusCos = 1.0f - cosA;

    temp.SetParam(0, 0, oneMinusCos * paramX * paramX + cosA);
    temp.SetParam(0, 1, oneMinusCos * paramX * paramY + sinA * paramZ);
    temp.SetParam(0, 2, oneMinusCos * paramX * paramZ - sinA * paramY);

    temp.SetParam(1, 0, oneMinusCos * paramY * paramX - sinA * paramZ);
    temp.SetParam(1, 1, oneMinusCos * paramY * paramY + cosA);
    temp.SetParam(1, 2, oneMinusCos * paramY * paramZ + sinA * paramX);

    temp.SetParam(2, 0, oneMinusCos * paramZ * paramX + sinA * paramY);
    temp.SetParam(2, 1, oneMinusCos * paramZ * paramY - sinA * paramX);
    temp.SetParam(2, 2, oneMinusCos * paramZ * paramZ + cosA);

    return temp;
}

// ベクトルの平行移動が出来る行列を取得する関数
MyMatrix MyMatrix::GetTransportion3D(float x, float y, float z)
{
    MyMatrix temp = MyMatrix::GetIdentity(4);
    temp.SetParam(3, 0, x);
    temp.SetParam(3, 1, y);
    temp.SetParam(3, 2, z);
    temp.SetParam(3, 3, 1);
    return temp;
}

// 全ての符号を+にした行列を取得する
MyMatrix MyMatrix::operator +() const
{
    return *this;
}

// 全ての符号を-にした行列を取得する
MyMatrix MyMatrix::operator -() const
{
    if (this->CheckActive())
    {
        MyMatrix temp = *this;
        for (int i = 0; i < (int)(this->GetSize()); i++)
        {
            temp.mlParam[i] = -(this->mlParam[i]);
        }

        return temp;
    }

    // 無効な行列を返す
    return MyMatrix::GetDeactivate();
}

// コピー
MyMatrix &MyMatrix::operator =(const MyMatrix &mat)
{
    // 内部でコピーをするだけ
    return this->Copy(mat);
}

// ムーブ
MyMatrix &MyMatrix::operator =(MyMatrix &&mat) noexcept
{
    // 内部でムーブをするだけ
    return this->Move(mat);
}

MyMatrix &MyMatrix::operator +=(const MyMatrix &mat)
{
    return this->AddAss(mat);
}

MyMatrix &MyMatrix::operator -=(const MyMatrix &mat)
{
    return this->SubAss(mat);
}

MyMatrix &MyMatrix::operator *=(const float &scaler)
{
    return this->ScaleScalarAss(scaler);
}

MyMatrix &MyMatrix::operator *=(const MyMatrix &mat)
{
    return this->ScaleMatrixAss(mat);
}

MyMatrix &MyMatrix::operator %=(const MyMatrix &mat)
{
    return this->ScaleElementAss(mat);
}

MyMatrix::MyMatrixRow MyMatrix::operator [](unsigned long row)
{
    // 自身の行列を参考に、行を抜粋する
    return MyMatrixRow(*this, row);
}

#ifdef USE_MY_VECTOR2D
// 行列を2次元ベクトルに変換する関数
VECTOR2D MyMatrix::To2VD() const
{
    VECTOR2D temp(0.0f, 0.0f);
    if (this->CheckActive() &&
        (this->mnRowCount == 1 &&
            this->mnColumnCount == 3))
    {
        temp.SetXY(this->mlParam[0], this->mlParam[1]);
    }
    return temp;
}

// VECTOR2Dの拡大・縮小が出来る行列を取得する関数
MyMatrix MyMatrix::GetScaling2D(const VECTOR2D &scaling)
{
    MyMatrix temp(3, 3);
    temp.SetParam(0, 0, scaling.GetX());
    temp.SetParam(1, 1, scaling.GetY());
    temp.SetParam(2, 2, 1);
    return temp;
}

// VECTOR2Dの平行移動が出来る行列を取得する関数
MyMatrix MyMatrix::GetTransportion2D(const VECTOR2D &transportion)
{
    MyMatrix temp(3, 3);
    temp.SetParam(2, 0, transportion.GetX());
    temp.SetParam(2, 1, transportion.GetY());
    temp.SetParam(2, 2, 1);
    return temp;
}
#endif // USE_MY_VECTOR2D

#ifdef USE_MY_VECTOR3D
// 行列を3次元ベクトルに変換する関数
VECTOR3D MyMatrix::To3VD() const
{
    VECTOR3D temp(0.0f, 0.0f, 0.0f);
    if (this->CheckActive() &&
        (this->mnRowCount == 1 &&
            (this->mnColumnCount == 3 ||
                this->mnColumnCount == 4)))
    {
        temp.SetXYZ(this->mlParam[0], this->mlParam[1], this->mlParam[2]);
    }
    return temp;
}

// 3Dベクトルの拡大・縮小が出来る行列を取得する関数
MyMatrix MyMatrix::GetScaling3D(const VECTOR3D &scaling)
{
    MyMatrix temp = MyMatrix::GetIdentity(4);
    temp.SetParam(0, 0, scaling.GetX());
    temp.SetParam(1, 1, scaling.GetY());
    temp.SetParam(2, 2, scaling.GetZ());
    temp.SetParam(3, 3, 1);
    return temp;
}

#ifdef USE_MY_ROTATION
// 3Dベクトルのx軸, y軸, z軸の回転がまとめて出来る行列を取得する関数
MyMatrix MyMatrix::GetRotation3D(const VECTOR3D &rotateRadian)
{
    return MyMatrix::GetRotation3D(rotateRadian, globalOrder);
}

// 3Dベクトルのx軸, y軸, z軸の回転がまとめて出来る行列を取得する関数
MyMatrix MyMatrix::GetRotation3D(const VECTOR3D &rotateRadian, EulerOrder order)
{
    MyMatrix temp(4, 4);
    switch (globalOrder)
    {
    case EulerOrder::XYZ:
        temp =
            MyMatrix::GetRotation3DX(rotateRadian.GetX()) *
            MyMatrix::GetRotation3DY(rotateRadian.GetY()) *
            MyMatrix::GetRotation3DZ(rotateRadian.GetZ());
        break;
    case EulerOrder::XZY:
        temp =
            MyMatrix::GetRotation3DX(rotateRadian.GetX()) *
            MyMatrix::GetRotation3DZ(rotateRadian.GetZ()) *
            MyMatrix::GetRotation3DY(rotateRadian.GetY());
        break;
    case EulerOrder::YXZ:
        temp =
            MyMatrix::GetRotation3DY(rotateRadian.GetY()) *
            MyMatrix::GetRotation3DX(rotateRadian.GetX()) *
            MyMatrix::GetRotation3DZ(rotateRadian.GetZ());
        break;
    case EulerOrder::YZX:
        temp =
            MyMatrix::GetRotation3DY(rotateRadian.GetY()) *
            MyMatrix::GetRotation3DZ(rotateRadian.GetZ()) *
            MyMatrix::GetRotation3DX(rotateRadian.GetX());
        break;
    case EulerOrder::ZXY:
        temp =
            MyMatrix::GetRotation3DZ(rotateRadian.GetZ()) *
            MyMatrix::GetRotation3DX(rotateRadian.GetX()) *
            MyMatrix::GetRotation3DY(rotateRadian.GetY());
        break;
    case EulerOrder::ZYX:
        temp =
            MyMatrix::GetRotation3DZ(rotateRadian.GetZ()) *
            MyMatrix::GetRotation3DY(rotateRadian.GetY()) *
            MyMatrix::GetRotation3DX(rotateRadian.GetX());
        break;
    default:
        temp = MyMatrix::GetDeactivate();
        break;
    }
    
    // 結果を返す
    return temp;
}

// 姿勢行列を取得する関数
MyMatrix MyMatrix::GetLookAt(const VECTOR3D &forword, const VECTOR3D &localUp)
{
    VECTOR3D forwordNorm = forword.Normalize();
    VECTOR3D upNorm = localUp.Normalize();
    VECTOR3D rightNorm = VECTOR3D::Cross(upNorm, forwordNorm);
    upNorm = VECTOR3D::Cross(forwordNorm, rightNorm);

    MyMatrix temp = MyMatrix::GetIdentity(4);
    temp.SetParam(0, 0, rightNorm.GetX());
    temp.SetParam(0, 1, rightNorm.GetY());
    temp.SetParam(0, 2, rightNorm.GetZ());
    temp.SetParam(1, 0, upNorm.GetX());
    temp.SetParam(1, 1, upNorm.GetY());
    temp.SetParam(1, 2, upNorm.GetZ());
    temp.SetParam(2, 0, forwordNorm.GetX());
    temp.SetParam(2, 1, forwordNorm.GetY());
    temp.SetParam(2, 2, forwordNorm.GetZ());
    return temp;
}
#endif // USE_MY_ROTATION

// 3Dベクトルの任意軸の回転が出来る行列を取得する関数
MyMatrix MyMatrix::GetRotationQuaternion3D(const VECTOR3D &axisVec, float radian)
{
    // (https://qiita.com/MENDY/items/0508916a98688b1fb0cc)←これの任意軸回りを参考に
    // ↑は嘘。参考にしたのはココ→(http://marupeke296.com/DXG_No58_RotQuaternionTrans.html)

    // 計算方法は以下の通り
    // 対角成分のみ(対応するベクトルの要素の二乗) * (1 - cosf(radian))    +   cos(radian)
    // それ以外は  (ベクトルの要素二つ)           * (1 - cosf(radian)) + or - 残ったベクトルの要素 * sinf(radian)
    // っていう感じ。綺麗だね。

    MyMatrix temp = MyMatrix::GetIdentity(4);

    VECTOR3D axisNorm = axisVec.Normalize();
    axisNorm.Normalize(); // XMMatrixRotationAxis と同様に正規化を行う

    float paramX = axisNorm.GetX();
    float paramY = axisNorm.GetY();
    float paramZ = axisNorm.GetZ();

    float cosA = cosf(radian);
    float sinA = sinf(radian);
    float oneMinusCos = 1.0f - cosA;

    temp.SetParam(0, 0, oneMinusCos * paramX * paramX + cosA);
    temp.SetParam(0, 1, oneMinusCos * paramX * paramY + sinA * paramZ);
    temp.SetParam(0, 2, oneMinusCos * paramX * paramZ - sinA * paramY);

    temp.SetParam(1, 0, oneMinusCos * paramY * paramX - sinA * paramZ);
    temp.SetParam(1, 1, oneMinusCos * paramY * paramY + cosA);
    temp.SetParam(1, 2, oneMinusCos * paramY * paramZ + sinA * paramX);

    temp.SetParam(2, 0, oneMinusCos * paramZ * paramX + sinA * paramY);
    temp.SetParam(2, 1, oneMinusCos * paramZ * paramY - sinA * paramX);
    temp.SetParam(2, 2, oneMinusCos * paramZ * paramZ + cosA);

    return temp;
}

// 3Dベクトルの平行移動が出来る行列を取得する関数
MyMatrix MyMatrix::GetTransportion3D(const VECTOR3D &transportion)
{
    MyMatrix temp = MyMatrix::GetIdentity(4);
    temp.SetParam(3, 0, transportion.GetX());
    temp.SetParam(3, 1, transportion.GetY());
    temp.SetParam(3, 2, transportion.GetZ());
    return temp;
}


#endif // USE_MY_VECTOR3D

#ifdef USE_MY_ROTATION
#ifdef USE_MY_QUATERNION
MyQuaternion MyMatrix::ToQtn() const
{
    /*
    MyQuaternion temp(0.0f, 0.0f, 0.0f, 0.0f);
    float px = this->GetParam(0, 0) - this->GetParam(1, 1) - this->GetParam(2, 2) + 1;
    float py = -this->GetParam(0, 0) + this->GetParam(1, 1) - this->GetParam(2, 2) + 1;
    float pz = -this->GetParam(0, 0) - this->GetParam(1, 1) + this->GetParam(2, 2) + 1;
    float pw = this->GetParam(0, 0) + this->GetParam(1, 1) + this->GetParam(2, 2) + 1;

    auto selected = 0;
    auto max = px;
    if (max < py) {
        selected = 1;
        max = py;
    }
    if (max < pz) {
        selected = 2;
        max = pz;
    }
    if (max < pw) {
        selected = 3;
        max = pw;
    }

    if (selected == 0)
    {
        float x = sqrtf(px) * 0.5f;
        float d = 1 / (4 * x);
        temp = MyQuaternion(
            x,
            (this->GetParam(1, 0) + this->GetParam(0, 1)) * d,
            (this->GetParam(0, 2) + this->GetParam(2, 0)) * d,
            (this->GetParam(2, 1) - this->GetParam(1, 2)) * d
        );
    }
    else if (selected == 1)
    {
        float y = sqrtf(py) * 0.5f;
        float d = 1 / (4 * y);
        temp = MyQuaternion(
            (this->GetParam(1, 0) + this->GetParam(0, 1)) * d,
            y,
            (this->GetParam(2, 1) + this->GetParam(1, 2)) * d,
            (this->GetParam(0, 2) - this->GetParam(2, 0)) * d
        );
    }
    else if (selected == 2)
    {
        float z = sqrtf(pz) * 0.5f;
        float d = 1 / (4 * z);
        temp = MyQuaternion(
            (this->GetParam(0, 2) + this->GetParam(2, 0)) * d,
            (this->GetParam(2, 1) + this->GetParam(1, 2)) * d,
            z,
            (this->GetParam(1, 0) - this->GetParam(0, 1)) * d
        );
    }
    else if (selected == 3)
    {
        float w = sqrtf(pw) * 0.5f;
        float d = 1 / (4 * w);
        temp = MyQuaternion(
            (this->GetParam(2, 1) - this->GetParam(1, 2)) * d,
            (this->GetParam(0, 2) - this->GetParam(2, 0)) * d,
            (this->GetParam(1, 0) - this->GetParam(0, 1)) * d,
            w
        );
    }
    */

    // ↓の計算は(http://marupeke296.com/DXG_No58_RotQuaternionTrans.html)を参考にした

    // 最大成分を検索
    float elem[4]; // 0:x, 1:y, 2:z, 3:w
    elem[0] = this->GetParam(0, 0) - this->GetParam(1, 1) - this->GetParam(2, 2) + 1.0f;
    elem[1] = -this->GetParam(0, 0) + this->GetParam(1, 1) - this->GetParam(2, 2) + 1.0f;
    elem[2] = -this->GetParam(0, 0) - this->GetParam(1, 1) + this->GetParam(2, 2) + 1.0f;
    elem[3] = this->GetParam(0, 0) + this->GetParam(1, 1) + this->GetParam(2, 2) + 1.0f;

    unsigned biggestIndex = 0;
    for (int i = 1; i < 4; i++)
    {
        if (elem[i] > elem[biggestIndex])
        {
            biggestIndex = i;
        }
    }
    if (elem[biggestIndex] < 0.0f)
    {
        return MyQuaternion(0.0f, 0.0f, 0.0f, 1.0f); // 引数の行列に間違いあり！
    }

    // 最大要素の値を算出
    float q[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    float v = sqrtf(elem[biggestIndex]) * 0.5f;
    q[biggestIndex] = v;
    float mult = 0.25f / v;
    switch (biggestIndex)
    {
    case 0: // x
        q[1] = (this->GetParam(0, 1) + this->GetParam(1, 0)) * mult;
        q[2] = (this->GetParam(2, 0) + this->GetParam(0, 2)) * mult;
        q[3] = (this->GetParam(1, 2) - this->GetParam(2, 1)) * mult;
        break;
    case 1: // y
        q[0] = (this->GetParam(0, 1) + this->GetParam(1, 0)) * mult;
        q[2] = (this->GetParam(1, 2) + this->GetParam(2, 1)) * mult;
        q[3] = (this->GetParam(2, 0) - this->GetParam(0, 2)) * mult;
        break;
    case 2: // z
        q[0] = (this->GetParam(2, 0) + this->GetParam(0, 2)) * mult;
        q[1] = (this->GetParam(1, 2) + this->GetParam(2, 1)) * mult;
        q[3] = (this->GetParam(0, 1) - this->GetParam(1, 0)) * mult;
        break;
    case 3: // w
        q[0] = (this->GetParam(1, 2) - this->GetParam(2, 1)) * mult;
        q[1] = (this->GetParam(2, 0) - this->GetParam(0, 2)) * mult;
        q[2] = (this->GetParam(0, 1) - this->GetParam(1, 0)) * mult;
        break;
    }
    MyQuaternion temp(q[0], q[1], q[2], q[3]);
    return temp;
}
#endif // USE_MY_QUATERNION

#ifdef USE_MY_EULERANGLE
MyEulerAngle MyMatrix::ToEulerAngle() const
{
    return this->ToEulerAngle(globalOrder);
}

MyEulerAngle MyMatrix::ToEulerAngle(EulerOrder order) const
{
    MyEulerAngle temp(order, 0.0f, 0.0f, 0.0f);
    if (order == EulerOrder::XYZ) 
    {
        auto sy = this->GetParam(0, 2);
        auto unlocked = fabsf(sy) < 0.99999f;
        temp.SetParam(
            order,
            unlocked ? atan2f(-this->GetParam(1, 2), this->GetParam(2, 2)) : atan2f(this->GetParam(2, 1), this->GetParam(1, 1)),
            asinf(sy),
            unlocked ? atan2f(-this->GetParam(0, 1), this->GetParam(0, 0)) : 0);
    }
    else if (order == EulerOrder::XZY)
    {
        auto sz = -this->GetParam(0, 1);
        auto unlocked = fabsf(sz) < 0.99999f;
        temp.SetParam(
            order,
            unlocked ? atan2f(this->GetParam(2, 1), this->GetParam(1, 1)) : atan2f(-this->GetParam(1, 2), this->GetParam(2, 2)),
            unlocked ? atan2f(this->GetParam(0, 2), this->GetParam(0, 0)) : 0,
            asinf(sz));
    }
    else if (order == EulerOrder::YXZ)
    {
        auto sx = -this->GetParam(1, 2);
        auto unlocked = fabsf(sx) < 0.99999f;
        temp.SetParam(
            order,
            asinf(sx),
            unlocked ? atan2f(this->GetParam(0, 2), this->GetParam(2, 2)) : atan2f(-this->GetParam(2, 0), this->GetParam(0, 0)),
            unlocked ? atan2f(this->GetParam(1, 0), this->GetParam(1, 1)) : 0);
    }
    else if (order == EulerOrder::YZX)
    {
        auto sz = this->GetParam(1, 0);
        auto unlocked = fabsf(sz) < 0.99999f;
        temp.SetParam(
            order,
            unlocked ? atan2f(-this->GetParam(1, 2), this->GetParam(1, 1)) : 0,
            unlocked ? atan2f(-this->GetParam(2, 0), this->GetParam(0, 0)) : atan2f(this->GetParam(0, 2), this->GetParam(2, 2)),
            asinf(sz));
    }
    else if (order == EulerOrder::ZXY)
    {
        auto sx = this->GetParam(2, 1);
        auto unlocked = fabsf(sx) < 0.99999f;
        temp.SetParam(
            order,
            asinf(sx),
            unlocked ? atan2f(-this->GetParam(2, 0), this->GetParam(2, 2)) : 0,
            unlocked ? atan2f(-this->GetParam(0, 1), this->GetParam(1, 1)) : atan2f(this->GetParam(1, 0), this->GetParam(0, 0)));
    }
    else if (order == EulerOrder::ZYX)
    {
        auto sy = -this->GetParam(2, 0);
        auto unlocked = fabsf(sy) < 0.99999f;
        temp.SetParam(
            order,
            unlocked ? atan2f(this->GetParam(2, 1), this->GetParam(2, 2)) : 0,
            asinf(sy),
            unlocked ? atan2f(this->GetParam(1, 0), this->GetParam(0, 0)) : atan2f(-this->GetParam(0, 1), this->GetParam(1, 1)));
    }
    return temp;
}
#endif // USE_MY_EULERANGLE
#endif // USE_MY_ROTATION

MyMatrix operator +(const MyMatrix &matL, const MyMatrix &matR)
{
    return matL.Add(matR);
}

MyMatrix operator -(const MyMatrix &matL, const MyMatrix &matR)
{
    return matL.Sub(matR);
}

MyMatrix operator *(const MyMatrix &mat, const float &scaler)
{
    return mat.ScaleScalar(scaler);
}

MyMatrix operator *(const MyMatrix &matL, const MyMatrix &matR)
{
    return matL.ScaleMatrix(matR);
}

MyMatrix operator %(const MyMatrix &matL, const MyMatrix &matR)
{
    return matL.ScaleElement(matR);
}

//----------ここからdouble----------

// 行列の行コンテキスト
class MyMatrix_D::MyMatrixRow_D
{
private:
    double *mpRowBox;

public:
    MyMatrixRow_D(const MyMatrix_D &mat, unsigned long row);                            // 行コンテキストのコンストラクタ
    ~MyMatrixRow_D();                                                                   // 行コンテキストのデストラクタ
    double &operator[](unsigned long index);                                             // 行の[]オペレータ
    const double &operator[](unsigned long index) const;                                 // 行の[]オペレータ
};

// 行コンテキストのコンストラクタ
MyMatrix_D::MyMatrixRow_D::MyMatrixRow_D(const MyMatrix_D &mat, unsigned long row) :
    mpRowBox(nullptr)
{
    // 1行目1列のデータ開始位置を取得(offset)
    double *offset = (double *)(mat.mlParam.Memory());

    // offsetから目的の行の開始位置までの距離を取得する
    auto distance = sizeof(double) * (mat.mnRowCount * static_cast<unsigned long long>(row));

    // offsetとdistanceを加えて、目的の行の開始位置を取得する
    this->mpRowBox = offset + distance;
}

// 行コンテキストのデストラクタ
MyMatrix_D::MyMatrixRow_D::~MyMatrixRow_D()
{
    this->mpRowBox = nullptr;
}

// 行の[]オペレータ
double &MyMatrix_D::MyMatrixRow_D::operator[](unsigned long index)
{
    return this->mpRowBox[index];
}

// 行の[]オペレータ
const double &MyMatrix_D::MyMatrixRow_D::operator[](unsigned long index) const
{
    return this->mpRowBox[index];
}

// 行列の列コンテキスト
class MyMatrix_D::MyMatrixColumn_D
{
private:
    MallocData<double> mlColumnBox;

public:
    MyMatrixColumn_D(const MyMatrix_D &mat, unsigned long column);                      // 列コンテキストのコンストラクタ
    ~MyMatrixColumn_D();                                                                // 行コンテキストのデストラクタ
    double &operator[](unsigned long index);                                             // 列の[]オペレータ
    const double &operator[](unsigned long index) const;                                 // 列の[]オペレータ
};

// 列コンテキストのコンストラクタ
MyMatrix_D::MyMatrixColumn_D::MyMatrixColumn_D(const MyMatrix_D &mat, unsigned long column) :
    mlColumnBox(mat.mnColumnCount, 0.0)
{
    // 行を進めていく
    for (unsigned i = 0; i < mat.mnRowCount;)
    {
        // 自身の配列に、指定の列の値を代入する
        this->mlColumnBox[i] = mat.mlParam[mat.mnRowCount * i + column];
    }
}

// 列コンテキストのデストラクタ
MyMatrix_D::MyMatrixColumn_D::~MyMatrixColumn_D()
{
    // この関数が終わったタイミングでMallocDataを破棄します
}

// 列の[]オペレータ
double &MyMatrix_D::MyMatrixColumn_D::operator[](unsigned long index)
{
    return this->mlColumnBox[index];
}

// 列の[]オペレータ
const double &MyMatrix_D::MyMatrixColumn_D::operator[](unsigned long index) const
{
    return this->mlColumnBox[index];
}

// コンストラクタ
MyMatrix_D::MyMatrix_D(unsigned long rowCount, unsigned long columnCount) :
    mnRowCount(rowCount),
    mnColumnCount(columnCount),
    mlParam((int)(rowCount *columnCount))
{
    for (int i = 0; i < (int)(rowCount * columnCount); i++)
    {
        this->mlParam[i] = 0;
    }
}

// コピーコンストラクタ
MyMatrix_D::MyMatrix_D(const MyMatrix_D &srcMatrix) :
    mnRowCount(srcMatrix.mnRowCount),
    mnColumnCount(srcMatrix.mnColumnCount),
    mlParam()
{
    // 行列の中身である、配列部もコピーする
    this->mlParam.Copy(srcMatrix.mlParam);
}

// ムーブコンストラクタ
MyMatrix_D::MyMatrix_D(MyMatrix_D &&srcMatrix) noexcept :
    mnRowCount(srcMatrix.mnRowCount),
    mnColumnCount(srcMatrix.mnColumnCount),
    mlParam()
{
    // 行列の中身である、配列部もムーブする
    this->mlParam.Move(srcMatrix.mlParam);
}

// デストラクタ
MyMatrix_D::~MyMatrix_D()
{
    // この関数が終わったタイミングでMallocDataを破棄します
}

// コピー
MyMatrix_D &MyMatrix_D::Copy(const MyMatrix_D &mat)
{
    // 行の個数をコピーする
    this->mnRowCount = mat.mnRowCount;

    // 列の個数をコピーする
    this->mnColumnCount = mat.mnColumnCount;

    // 配列部をコピーする
    this->mlParam.Copy(mat.mlParam);

    // 返り値を返す
    return *this;
}

// ムーブ
MyMatrix_D &MyMatrix_D::Move(MyMatrix_D &mat) noexcept
{
    // 行の個数をコピーする
    this->mnRowCount = mat.mnRowCount;

    // 列の個数をコピーする
    this->mnColumnCount = mat.mnColumnCount;

    // 配列部をコピーする
    this->mlParam.Move(mat.mlParam);

    // 返り値を返す
    return *this;
}

// ムーブ
MyMatrix_D &MyMatrix_D::Move(MyMatrix_D &&mat) noexcept
{
    // 行の個数をコピーする
    this->mnRowCount = mat.mnRowCount;

    // 列の個数をコピーする
    this->mnColumnCount = mat.mnColumnCount;

    // 配列部をコピーする
    this->mlParam.Move(mat.mlParam);

    // 返り値を返す
    return *this;
}

// この行列が有効なものかを確認する関数
bool MyMatrix_D::CheckActive() const
{
    return ((this->mnRowCount != 0) && (this->mnColumnCount != 0));
}

// この行列が正方行列かを確認する関数
bool MyMatrix_D::CheckSquare() const
{
    return this->mnRowCount == this->mnColumnCount;
}

// この行列の要素数の合計を取得する関数
unsigned long MyMatrix_D::GetSize() const
{
    return this->mnRowCount * this->mnColumnCount;
}

// 行列の足し算をする関数。
// ※行列の足し算は互いの行の個数と列の個数が等しい場合のみ可能。計算できない場合は無効な行列を返す。
MyMatrix_D MyMatrix_D::Add(const MyMatrix_D &matRight) const
{
    if (this->CheckActive() &&
        matRight.CheckActive() &&
        this->mnRowCount == matRight.mnRowCount &&
        this->mnColumnCount == matRight.mnColumnCount)
    {
        MyMatrix_D temp(this->mnRowCount, this->mnColumnCount);
        for (int i = 0; i < (int)(this->GetSize()); i++)
        {
            temp.mlParam[i] = this->mlParam[i] + matRight.mlParam[i];
        }
        return temp;
    }

    // 無効な行列を返す
    return MyMatrix_D::GetDeactivate();
}

// 行列の引き算をする関数。
// ※行列の引き算は互いの行の個数と列の個数が等しい場合のみ可能。計算できない場合は無効な行列を返す。
MyMatrix_D MyMatrix_D::Sub(const MyMatrix_D &matRight) const
{
    if (this->CheckActive() &&
        matRight.CheckActive() &&
        this->mnRowCount == matRight.mnRowCount &&
        this->mnColumnCount == matRight.mnColumnCount)
    {
        MyMatrix_D temp(this->mnRowCount, this->mnColumnCount);
        for (int i = 0; i < (int)(this->GetSize()); i++)
        {
            temp.mlParam[i] = this->mlParam[i] - matRight.mlParam[i];
        }
        return temp;
    }

    // 無効な行列を返す
    return MyMatrix_D::GetDeactivate();
}

// 行列のスカラー倍をする関数。
// ※スカラー倍の関数はどんな行列とも計算が可能。全てのパラメータとスカラーをかける。
MyMatrix_D MyMatrix_D::ScaleScalar(double scalar) const
{
    if (this->CheckActive())
    {
        MyMatrix_D temp(this->mnRowCount, this->mnColumnCount);
        for (int i = 0; i < (int)(this->GetSize()); i++)
        {
            temp.mlParam[i] = this->mlParam[i] * scalar;
        }
        return temp;
    }

    // 無効な行列を返す
    return MyMatrix_D::GetDeactivate();
}

// 行列同士の掛け算をする関数。
// ※行列の掛け算は (leftの列数) == (rightの行) の場合のみ可能。計算できない場合は無効な行列を返す。
MyMatrix_D MyMatrix_D::ScaleMatrix(const MyMatrix_D &matRight) const
{
    if (this->CheckActive() &&
        matRight.CheckActive() &&
        this->mnColumnCount == matRight.mnRowCount)
    {
        MyMatrix_D temp(this->mnRowCount, matRight.mnColumnCount);
        double param = 0;
        for (unsigned long currentRow = 0; currentRow < temp.mnRowCount; currentRow++)
        {
            for (unsigned long currentColumn = 0; currentColumn < temp.mnColumnCount; currentColumn++)
            {
                for (unsigned long count = 0; count < this->mnColumnCount; count++)
                {
                    param += this->GetParam(currentRow, count) * matRight.GetParam(count, currentColumn);
                }
                temp.SetParam(currentRow, currentColumn, param);
                param = 0;
            }
        }

        return temp;
    }

    // 無効な行列を返す
    return MyMatrix_D::GetDeactivate();
}

// 行列同士のアダマール積をする関数。
// ※途中、計算に失敗した場合は何も行わない
MyMatrix_D MyMatrix_D::ScaleElement(const MyMatrix_D &matRight) const
{
    if (this->CheckActive() &&
        matRight.CheckActive() &&
        this->mnRowCount == matRight.mnRowCount &&
        this->mnColumnCount == matRight.mnColumnCount)
    {
        MyMatrix_D temp(this->mnRowCount, this->mnColumnCount);
        for (int i = 0; i < (int)(this->GetSize()); i++)
        {
            temp.mlParam[i] = this->mlParam[i] * matRight.mlParam[i];
        }
        return temp;
    }

    // 無効な行列を返す
    return MyMatrix_D::GetDeactivate();
}

// 行列の足し算を行い、自身の値に加える。
// ※途中、計算に失敗した場合は何も行わない
MyMatrix_D &MyMatrix_D::AddAss(const MyMatrix_D &matRight)
{
    MyMatrix_D temp = this->Add(matRight);
    if (temp.CheckActive())
    {
        *this = temp;
    }
    return *this;
}

// 行列の引き算を行い、自身の値に加える。
// ※途中、計算に失敗した場合は何も行わない
MyMatrix_D &MyMatrix_D::SubAss(const MyMatrix_D &matRight)
{
    MyMatrix_D temp = this->Sub(matRight);
    if (temp.CheckActive())
    {
        *this = temp;
    }
    return *this;
}

// 行列のスカラー倍を行い、自身の値に加える。
// ※途中、計算に失敗した場合は何も行わない
MyMatrix_D &MyMatrix_D::ScaleScalarAss(double scalar)
{
    MyMatrix_D temp = this->ScaleScalar(scalar);
    if (temp.CheckActive())
    {
        *this = temp;
    }
    return *this;
}

// 行列同士の掛け算を行い、自身の値に加える。
// ※途中、計算に失敗した場合は何も行わない
MyMatrix_D &MyMatrix_D::ScaleMatrixAss(const MyMatrix_D &matRight)
{
    MyMatrix_D temp = this->ScaleMatrix(matRight);
    if (temp.CheckActive())
    {
        *this = temp;
    }
    return *this;
}

// 行列同士のアダマール積を行い、自身の値に加える。
// ※途中、計算に失敗した場合は何も行わない
MyMatrix_D &MyMatrix_D::ScaleElementAss(const MyMatrix_D &matRight)
{
    MyMatrix_D temp = this->ScaleElement(matRight);
    if (temp.CheckActive())
    {
        *this = temp;
    }
    return *this;
}

// 行列式を行う関数
double MyMatrix_D::GetDeterminant() const
{
    // この計算式は(https://www.krrk0.com/determinant-calculation/)を参考にしています。
    double temp = 0;
    if (this->CheckActive() &&
        this->CheckSquare())
    {
        if (this->mnRowCount == 2)
        {
            temp = (this->GetParam(0, 0) * this->GetParam(1, 1)) - (this->GetParam(0, 1) * this->GetParam(1, 0));
        }
        else if (this->mnRowCount == 3)
        {
            temp = this->GetParam(0, 0) * this->GetParam(1, 1) * this->GetParam(2, 2);
            temp += this->GetParam(1, 0) * this->GetParam(2, 1) * this->GetParam(0, 2);
            temp += this->GetParam(2, 0) * this->GetParam(0, 1) * this->GetParam(1, 2);
            temp -= this->GetParam(2, 0) * this->GetParam(1, 1) * this->GetParam(0, 2);
            temp -= this->GetParam(1, 0) * this->GetParam(0, 1) * this->GetParam(2, 2);
            temp -= this->GetParam(0, 0) * this->GetParam(2, 1) * this->GetParam(1, 2);
        }
        else if (this->mnRowCount == 4)
        {
            // 余因子展開を使った方法
            // とりあえず4でとどめておきます。

            // 余因子展開を行う
            MyMatrix_D mat0 = this->GetCofactorExpansion(0, 0);
            MyMatrix_D mat1 = this->GetCofactorExpansion(1, 0);
            MyMatrix_D mat2 = this->GetCofactorExpansion(2, 0);
            MyMatrix_D mat3 = this->GetCofactorExpansion(3, 0);

            // 作った3次行列の行列式を解く
            double mat0Determinant = mat0.GetDeterminant();
            double mat1Determinant = mat1.GetDeterminant();
            double mat2Determinant = mat2.GetDeterminant();
            double mat3Determinant = mat3.GetDeterminant();

            // 行列式の答えを全て加算したものをtempに代入する。
            temp = mat0Determinant + mat1Determinant + mat2Determinant + mat3Determinant;

            /*
            // (https://thira.plavox.info/blog/2008/06/_c.html#google_vignette)を参考にした、四次以上の方法
            // エラーが起きるらしい。多分0除算のエラーっぽい？
            double buffer = 0;
            MyMatrix mat = *this;
            unsigned long dimension = this->mnRowCount;
            for (unsigned long i = 0; i < dimension; i++)
            {
                for (unsigned long j = 0; j < dimension; j++)
                {
                    if (i < j)
                    {
                        buffer = mat.GetParam(j, i) / mat.GetParam(i, i);
                        for (unsigned long k = 0; k < dimension; k++)
                        {
                            mat.SetParam(j, k, (mat.GetParam(j, k) - (mat.GetParam(i, k) * buffer)));
                        }
                    }
                }
            }

            // 答えを出すための準備
            temp = 1;

            //対角部分の積で答えを出す
            for (unsigned long i = 0; i < dimension; i++)
            {
                temp *= mat.GetParam(i, i);
            }
            */
        }
    }

    // 返り値を返す
    return temp;
}

// 余因子行列を取得する関数
MyMatrix_D MyMatrix_D::GetAdjugate(unsigned long targetRow, unsigned long targetColumn) const
{
    if (this->CheckSquare())
    {
        unsigned long rowSize = this->mnRowCount - 1;
        unsigned long columnSize = this->mnColumnCount - 1;
        MyMatrix_D abjMatrix(rowSize, columnSize);

        unsigned long useRow = 0;
        unsigned long useColumn = 0;
        for (unsigned long currentRow = 0; currentRow < rowSize; currentRow++)
        {
            if (currentRow == targetRow)
            {
                useRow++;
            }

            for (unsigned long currentColumn = 0; currentColumn < columnSize; currentColumn++)
            {
                if (currentColumn == targetColumn)
                {
                    useColumn++;
                }

                abjMatrix.SetParam(currentRow, currentColumn, this->GetParam(useRow, useColumn));
                useColumn++;
            }

            useColumn = 0;

            useRow++;
        }

        return abjMatrix;
    }

    // 無効な行列を返す
    return MyMatrix_D::GetDeactivate();
}

// 余因子展開を行う関数
MyMatrix_D MyMatrix_D::GetCofactorExpansion(unsigned long targetRow, unsigned long targetColumn) const
{
    // 余因子行列を取得する
    MyMatrix_D temp = this->GetAdjugate(targetRow, targetColumn);

    // 行列が有効なものかを確認する
    if (temp.CheckActive())
    {
        // 注目している数字を取得する
        double currentParam = this->GetParam(targetRow, targetColumn);

        // 注目している数字で掛け算を行う
        MyMatrix_D abj = temp.ScaleScalar(currentParam);

        // targetRow + targetcolumn の結果が奇数あれば、値を反転させる
        if (((targetRow + targetColumn) % 2) == 1)
        {
            abj = -temp;
        }

        // 値を返す
        return  abj;
    }

    // 無効な行列を返す
    return MyMatrix_D::GetDeactivate();
}

// 逆行列を取得する関数。
MyMatrix_D MyMatrix_D::GetInverse() const
{
    if (this->CheckActive() &&
        this->GetDeterminant() != 0.0)
    {
        // 渡された行列が正方行列だった場合は処理を行う
        if (this->CheckSquare())
        {
            // この計算式は(https://thira.plavox.info/blog/2008/06/_c.html#google_vignette)を参考にしています。

            // 行列の次元を保持しておく
            unsigned long dimension = this->GetRowCount();

            // 逆行列が入る行列
            MyMatrix_D inv = GetIdentity(dimension);
            MyMatrix_D temp = *this;

            // その他作業用変数
            double buffer = 0.0;

            //掃き出し法
            for (unsigned long i = 0; i < dimension; i++)
            {
                buffer = 1 / temp.GetParam(i, i);
                for (unsigned long j = 0; j < dimension; j++)
                {
                    temp.SetParam(i, j, (temp.GetParam(i, j) * buffer));
                    inv.SetParam(i, j, (inv.GetParam(i, j) * buffer));
                }
                for (unsigned long j = 0; j < dimension; j++)
                {
                    if (i != j)
                    {
                        buffer = temp.GetParam(j, i);
                        for (unsigned long k = 0; k < dimension; k++)
                        {
                            temp.SetParam(j, k, (temp.GetParam(j, k) - (temp.GetParam(i, k) * buffer)));
                            inv.SetParam(j, k, (inv.GetParam(j, k) - (inv.GetParam(i, k) * buffer)));
                        }
                    }
                }
            }

            return inv;
        }
    }

    // 無効な行列を返す
    return MyMatrix_D::GetDeactivate();
}

// 転置行列を取得する関数
MyMatrix_D MyMatrix_D::GetTranspose() const
{
    if (this->CheckActive() == false)
    {
        return MyMatrix_D::GetDeactivate();
    }

    MyMatrix_D temp(this->mnColumnCount, this->mnRowCount);
    for (unsigned long thisRow = 0; thisRow < this->mnRowCount; thisRow++)
    {
        for (unsigned long thisColumn = 0; thisColumn < this->mnColumnCount; thisColumn++)
        {
            temp.SetParam(thisColumn, thisRow, this->GetParam(thisRow, thisColumn));
        }
    }

    return temp;
}

// 実際の値を設定する関数
void MyMatrix_D::SetParam(unsigned long row, unsigned long column, double param)
{
    unsigned long index = (row * this->mnColumnCount) + column;
    this->mlParam[(int)(index)] = param;
}

// 行の数を取得する関数
unsigned long MyMatrix_D::GetRowCount() const
{
    return this->mnRowCount;
}

// 列の数を取得する関数
unsigned long MyMatrix_D::GetColumnCount() const
{
    return this->mnColumnCount;
}

// 実際の値を取得する関数
double MyMatrix_D::GetParam(unsigned long row, unsigned long column) const
{
    unsigned long index = (row * this->mnColumnCount) + column;
    return this->mlParam[(int)(index)];
}

// 実際の値が入っているメモリのアドレスを取得する関数
double *MyMatrix_D::GetMemory() const
{
    return this->mlParam.Memory();
}

// 実際の値が入っているメモリのアドレスを取得する関数
double *MyMatrix_D::GetMemoryRow(unsigned long row) const
{
    double *offset = this->mlParam.Memory();
    unsigned long long distance = sizeof(double) * (row * this->mnColumnCount);
    return offset + distance;
}

// 実際の値が入っているメモリのアドレスを取得する関数
MallocData<double *> MyMatrix_D::GetMemoryColumn(unsigned long column) const
{
    // 指定の列をまとめたもの
    MallocData<double *> returnValue(this->mnColumnCount);

    // 自身の実部をまとめたもの
    double *offset = this->mlParam.Memory();

    // 探す
    unsigned long long distance = 0;

    // 列を走査するループ
    for (unsigned long i = 0; i < this->mnColumnCount; i++)
    {
        // i行目、指定列のポイントを探す
        distance = sizeof(double) * ((i * this->mnColumnCount) + column);

        // ポイントのアドレスを取得する
        returnValue.AddParam(offset + distance);
    }

    // 結果を返す
    return returnValue;
}

// 実際の値が入っているメモリのアドレスを取得する関数
double *MyMatrix_D::GetMemoryPoint(unsigned long row, unsigned long column) const
{
    double *offset = this->mlParam.Memory();
    unsigned long long distance = sizeof(double) * ((row * this->mnColumnCount) + column);
    return offset + distance;
}


// 無効な行列を取得する関数
MyMatrix_D MyMatrix_D::GetDeactivate()
{
    return MyMatrix_D(0, 0);
}

// 単位行列を取得する関数
MyMatrix_D MyMatrix_D::GetIdentity(unsigned long dimension)
{
    if (2 <= dimension)
    {
        MyMatrix_D temp(dimension, dimension);
        for (unsigned long i = 0; i < dimension; i++)
        {
            for (unsigned long j = 0; j < dimension; j++)
            {
                if (i == j)
                {
                    temp.SetParam(i, j, 1);
                }
                else
                {
                    temp.SetParam(i, j, 0);
                }
            }
        }
        return temp;
    }

    // 無効な行列を返す
    return MyMatrix_D::GetDeactivate();
}

// 零行列を取得する関数
MyMatrix_D MyMatrix_D::GetZero(unsigned long dimension)
{
    if (2 <= dimension)
    {
        MyMatrix_D temp(dimension, dimension);
        for (unsigned long i = 0; i < dimension; i++)
        {
            for (unsigned long j = 0; j < dimension; j++)
            {
                temp.SetParam(i, j, 0);
            }
        }
        return temp;
    }

    // 無効な行列を返す
    return MyMatrix_D::GetDeactivate();
}

// VECTOR2Dの拡大・縮小が出来る行列を取得する関数
MyMatrix_D MyMatrix_D::GetScaling2D(double x, double y)
{
    MyMatrix_D temp(3, 3);
    temp.SetParam(0, 0, x);
    temp.SetParam(1, 1, y);
    temp.SetParam(2, 2, 1);
    return temp;
}

// VECTOR2Dの回転(2DなのでZ軸のみ)が出来る行列を取得する関数
MyMatrix_D MyMatrix_D::GetRotation2D(double radian)
{
    MyMatrix_D temp(3, 3);
    temp.SetParam(0, 0, cos(radian));
    temp.SetParam(0, 1, sin(radian));
    temp.SetParam(1, 0, -sin(radian));
    temp.SetParam(1, 1, cos(radian));
    temp.SetParam(2, 2, 1);
    return temp;
}

// VECTOR2Dの平行移動が出来る行列を取得する関数
MyMatrix_D MyMatrix_D::GetTransportion2D(double x, double y)
{
    MyMatrix_D temp(3, 3);
    temp.SetParam(2, 0, x);
    temp.SetParam(2, 1, y);
    temp.SetParam(2, 2, 1);
    return temp;
}

// ベクトルの拡大・縮小が出来る行列を取得する関数
MyMatrix_D MyMatrix_D::GetScaling3D(double x, double y, double z)
{
    MyMatrix_D temp = MyMatrix_D::GetIdentity(4);
    temp.SetParam(0, 0, x);
    temp.SetParam(1, 1, y);
    temp.SetParam(2, 2, z);
    temp.SetParam(3, 3, 1);
    return temp;
}

// VECTOR3DのX軸回転が出来る行列を取得する関数
MyMatrix_D MyMatrix_D::GetRotation3DX(double radian)
{
    MyMatrix_D temp = MyMatrix_D::GetIdentity(4);
    temp.SetParam(0, 0, 1);
    temp.SetParam(1, 1, cos(radian));
    temp.SetParam(1, 2, sin(radian));
    temp.SetParam(2, 1, -sin(radian));
    temp.SetParam(2, 2, cos(radian));
    temp.SetParam(3, 3, 1);
    return temp;

}

// VECTOR3DのY軸回転が出来る行列を取得する関数
MyMatrix_D MyMatrix_D::GetRotation3DY(double radian)
{
    MyMatrix_D temp = MyMatrix_D::GetIdentity(4);
    temp.SetParam(0, 0, cos(radian));
    temp.SetParam(0, 2, -sin(radian));
    temp.SetParam(1, 1, 1);
    temp.SetParam(2, 0, sin(radian));
    temp.SetParam(2, 2, cos(radian));
    temp.SetParam(3, 3, 1);
    return temp;
}

// VECTOR3DのZ軸回転が出来る行列を取得する関数
MyMatrix_D MyMatrix_D::GetRotation3DZ(double radian)
{
    MyMatrix_D temp = MyMatrix_D::GetIdentity(4);
    temp.SetParam(0, 0, cos(radian));
    temp.SetParam(0, 1, sin(radian));
    temp.SetParam(1, 0, -sin(radian));
    temp.SetParam(1, 1, cos(radian));
    temp.SetParam(2, 2, 1);
    temp.SetParam(3, 3, 1);
    return temp;
}

// 行列を用いてVECTOR3Dの任意の軸の回転(クォータニオンでの回転)を行う関数
MyMatrix_D MyMatrix_D::GetRotationQuaternion3D(double axisX, double axisY, double axisZ, double radian)
{
    // (https://qiita.com/MENDY/items/0508916a98688b1fb0cc)←これの任意軸回りを参考に
    // ↑は嘘。参考にしたのはココ→(http://marupeke296.com/DXG_No58_RotQuaternionTrans.html)

    // 計算方法は以下の通り
    // 対角成分のみ(対応するベクトルの要素の二乗) * (1 - cos(radian))    +   cos(radian)
    // それ以外は  (ベクトルの要素二つ)           * (1 - cos(radian)) + or - 残ったベクトルの要素 * sin(radian)
    // っていう感じ。綺麗だね。

    MyMatrix_D temp = MyMatrix_D::GetIdentity(4);
    VECTOR3D axis;
    axis.SetXYZ(axisX, axisY, axisZ);
    VECTOR3D axisNorm = axis.Normalize();
    axisNorm.Normalize(); // XMMatrixRotationAxis と同様に正規化を行う

    double paramX = axisNorm.GetX();
    double paramY = axisNorm.GetY();
    double paramZ = axisNorm.GetZ();

    double cosA = cos(radian);
    double sinA = sin(radian);
    double oneMinusCos = 1.0 - cosA;

    temp.SetParam(0, 0, oneMinusCos * paramX * paramX + cosA);
    temp.SetParam(0, 1, oneMinusCos * paramX * paramY + sinA * paramZ);
    temp.SetParam(0, 2, oneMinusCos * paramX * paramZ - sinA * paramY);

    temp.SetParam(1, 0, oneMinusCos * paramY * paramX - sinA * paramZ);
    temp.SetParam(1, 1, oneMinusCos * paramY * paramY + cosA);
    temp.SetParam(1, 2, oneMinusCos * paramY * paramZ + sinA * paramX);

    temp.SetParam(2, 0, oneMinusCos * paramZ * paramX + sinA * paramY);
    temp.SetParam(2, 1, oneMinusCos * paramZ * paramY - sinA * paramX);
    temp.SetParam(2, 2, oneMinusCos * paramZ * paramZ + cosA);

    return temp;
}

// ベクトルの平行移動が出来る行列を取得する関数
MyMatrix_D MyMatrix_D::GetTransportion3D(double x, double y, double z)
{
    MyMatrix_D temp = MyMatrix_D::GetIdentity(4);
    temp.SetParam(3, 0, x);
    temp.SetParam(3, 1, y);
    temp.SetParam(3, 2, z);
    temp.SetParam(3, 3, 1);
    return temp;
}

// 全ての符号を+にした行列を取得する
MyMatrix_D MyMatrix_D::operator +() const
{
    return *this;
}

// 全ての符号を-にした行列を取得する
MyMatrix_D MyMatrix_D::operator -() const
{
    if (this->CheckActive())
    {
        MyMatrix_D temp = *this;
        for (int i = 0; i < (int)(this->GetSize()); i++)
        {
            temp.mlParam[i] = -(this->mlParam[i]);
        }

        return temp;
    }

    // 無効な行列を返す
    return MyMatrix_D::GetDeactivate();
}

// コピー
MyMatrix_D &MyMatrix_D::operator =(const MyMatrix_D &mat)
{
    // 内部でコピーをするだけ
    return this->Copy(mat);
}

// ムーブ
MyMatrix_D &MyMatrix_D::operator =(MyMatrix_D &&mat) noexcept
{
    // 内部でムーブをするだけ
    return this->Move(mat);
}

MyMatrix_D &MyMatrix_D::operator +=(const MyMatrix_D &mat)
{
    return this->AddAss(mat);
}

MyMatrix_D &MyMatrix_D::operator -=(const MyMatrix_D &mat)
{
    return this->SubAss(mat);
}

MyMatrix_D &MyMatrix_D::operator *=(const double &scaler)
{
    return this->ScaleScalarAss(scaler);
}

MyMatrix_D &MyMatrix_D::operator *=(const MyMatrix_D &mat)
{
    return this->ScaleMatrixAss(mat);
}

MyMatrix_D &MyMatrix_D::operator %=(const MyMatrix_D &mat)
{
    return this->ScaleElementAss(mat);
}

MyMatrix_D::MyMatrixRow_D MyMatrix_D::operator [](unsigned long row)
{
    // 自身の行列を参考に、行を抜粋する
    return MyMatrixRow_D(*this, row);
}

#ifdef USE_MY_VECTOR2D
// 行列を2次元ベクトルに変換する関数
VECTOR2D_D MyMatrix_D::To2VD() const
{
    VECTOR2D_D temp(0.0, 0.0);
    if (this->CheckActive() &&
        (this->mnRowCount == 1 &&
            this->mnColumnCount == 3))
    {
        temp.SetXY(this->mlParam[0], this->mlParam[1]);
    }
    return temp;
}

// VECTOR2Dの拡大・縮小が出来る行列を取得する関数
MyMatrix_D MyMatrix_D::GetScaling2D(const VECTOR2D_D &scaling)
{
    MyMatrix_D temp(3, 3);
    temp.SetParam(0, 0, scaling.GetX());
    temp.SetParam(1, 1, scaling.GetY());
    temp.SetParam(2, 2, 1);
    return temp;
}

// VECTOR2Dの平行移動が出来る行列を取得する関数
MyMatrix_D MyMatrix_D::GetTransportion2D(const VECTOR2D_D &transportion)
{
    MyMatrix_D temp(3, 3);
    temp.SetParam(2, 0, transportion.GetX());
    temp.SetParam(2, 1, transportion.GetY());
    temp.SetParam(2, 2, 1);
    return temp;
}
#endif // USE_MY_VECTOR2D

#ifdef USE_MY_VECTOR3D
// 行列を3次元ベクトルに変換する関数
VECTOR3D_D MyMatrix_D::To3VD() const
{
    VECTOR3D_D temp(0.0, 0.0, 0.0);
    if (this->CheckActive() &&
        (this->mnRowCount == 1 &&
            (this->mnColumnCount == 3 ||
                this->mnColumnCount == 4)))
    {
        temp.SetXYZ(this->mlParam[0], this->mlParam[1], this->mlParam[2]);
    }
    return temp;
}

// 3Dベクトルの拡大・縮小が出来る行列を取得する関数
MyMatrix_D MyMatrix_D::GetScaling3D(const VECTOR3D_D &scaling)
{
    MyMatrix_D temp = MyMatrix_D::GetIdentity(4);
    temp.SetParam(0, 0, scaling.GetX());
    temp.SetParam(1, 1, scaling.GetY());
    temp.SetParam(2, 2, scaling.GetZ());
    temp.SetParam(3, 3, 1);
    return temp;
}

#ifdef USE_MY_ROTATION
// 3Dベクトルのx軸, y軸, z軸の回転がまとめて出来る行列を取得する関数
MyMatrix_D MyMatrix_D::GetRotation3D(const VECTOR3D_D &rotateRadian)
{
    return MyMatrix_D::GetRotation3D(rotateRadian, globalOrder);
}

// 3Dベクトルのx軸, y軸, z軸の回転がまとめて出来る行列を取得する関数
MyMatrix_D MyMatrix_D::GetRotation3D(const VECTOR3D_D &rotateRadian, EulerOrder order)
{
    MyMatrix_D temp(4, 4);
    switch (globalOrder)
    {
    case EulerOrder::XYZ:
        temp =
            MyMatrix_D::GetRotation3DX(rotateRadian.GetX()) *
            MyMatrix_D::GetRotation3DY(rotateRadian.GetY()) *
            MyMatrix_D::GetRotation3DZ(rotateRadian.GetZ());
        break;
    case EulerOrder::XZY:
        temp =
            MyMatrix_D::GetRotation3DX(rotateRadian.GetX()) *
            MyMatrix_D::GetRotation3DZ(rotateRadian.GetZ()) *
            MyMatrix_D::GetRotation3DY(rotateRadian.GetY());
        break;
    case EulerOrder::YXZ:
        temp =
            MyMatrix_D::GetRotation3DY(rotateRadian.GetY()) *
            MyMatrix_D::GetRotation3DX(rotateRadian.GetX()) *
            MyMatrix_D::GetRotation3DZ(rotateRadian.GetZ());
        break;
    case EulerOrder::YZX:
        temp =
            MyMatrix_D::GetRotation3DY(rotateRadian.GetY()) *
            MyMatrix_D::GetRotation3DZ(rotateRadian.GetZ()) *
            MyMatrix_D::GetRotation3DX(rotateRadian.GetX());
        break;
    case EulerOrder::ZXY:
        temp =
            MyMatrix_D::GetRotation3DZ(rotateRadian.GetZ()) *
            MyMatrix_D::GetRotation3DX(rotateRadian.GetX()) *
            MyMatrix_D::GetRotation3DY(rotateRadian.GetY());
        break;
    case EulerOrder::ZYX:
        temp =
            MyMatrix_D::GetRotation3DZ(rotateRadian.GetZ()) *
            MyMatrix_D::GetRotation3DY(rotateRadian.GetY()) *
            MyMatrix_D::GetRotation3DX(rotateRadian.GetX());
        break;
    default:
        temp = MyMatrix_D::GetDeactivate();
        break;
    }

    // 結果を返す
    return temp;
}

// 姿勢行列を取得する関数
MyMatrix_D MyMatrix_D::GetLookAt(const VECTOR3D_D &forword, const VECTOR3D_D &localUp)
{
    VECTOR3D_D forwordNorm = forword.Normalize();
    VECTOR3D_D upNorm = localUp.Normalize();
    VECTOR3D_D rightNorm = VECTOR3D_D::Cross(upNorm, forwordNorm);
    upNorm = VECTOR3D_D::Cross(forwordNorm, rightNorm);

    MyMatrix_D temp = MyMatrix_D::GetIdentity(4);
    temp.SetParam(0, 0, rightNorm.GetX());
    temp.SetParam(0, 1, rightNorm.GetY());
    temp.SetParam(0, 2, rightNorm.GetZ());
    temp.SetParam(1, 0, upNorm.GetX());
    temp.SetParam(1, 1, upNorm.GetY());
    temp.SetParam(1, 2, upNorm.GetZ());
    temp.SetParam(2, 0, forwordNorm.GetX());
    temp.SetParam(2, 1, forwordNorm.GetY());
    temp.SetParam(2, 2, forwordNorm.GetZ());
    return temp;
}
#endif // USE_MY_ROTATION

// 3Dベクトルの任意軸の回転が出来る行列を取得する関数
MyMatrix_D MyMatrix_D::GetRotationQuaternion3D(const VECTOR3D_D &axisVec, double radian)
{
    // (https://qiita.com/MENDY/items/0508916a98688b1fb0cc)←これの任意軸回りを参考に
    // ↑は嘘。参考にしたのはココ→(http://marupeke296.com/DXG_No58_RotQuaternionTrans.html)

    // 計算方法は以下の通り
    // 対角成分のみ(対応するベクトルの要素の二乗) * (1 - cos(radian))    +   cos(radian)
    // それ以外は  (ベクトルの要素二つ)           * (1 - cos(radian)) + or - 残ったベクトルの要素 * sin(radian)
    // っていう感じ。綺麗だね。

    MyMatrix_D temp = MyMatrix_D::GetIdentity(4);

    VECTOR3D_D axisNorm = axisVec.Normalize();
    axisNorm.Normalize(); // XMMatrixRotationAxis と同様に正規化を行う

    double paramX = axisNorm.GetX();
    double paramY = axisNorm.GetY();
    double paramZ = axisNorm.GetZ();

    double cosA = cos(radian);
    double sinA = sin(radian);
    double oneMinusCos = 1.0 - cosA;

    temp.SetParam(0, 0, oneMinusCos * paramX * paramX + cosA);
    temp.SetParam(0, 1, oneMinusCos * paramX * paramY + sinA * paramZ);
    temp.SetParam(0, 2, oneMinusCos * paramX * paramZ - sinA * paramY);

    temp.SetParam(1, 0, oneMinusCos * paramY * paramX - sinA * paramZ);
    temp.SetParam(1, 1, oneMinusCos * paramY * paramY + cosA);
    temp.SetParam(1, 2, oneMinusCos * paramY * paramZ + sinA * paramX);

    temp.SetParam(2, 0, oneMinusCos * paramZ * paramX + sinA * paramY);
    temp.SetParam(2, 1, oneMinusCos * paramZ * paramY - sinA * paramX);
    temp.SetParam(2, 2, oneMinusCos * paramZ * paramZ + cosA);

    return temp;
}

// 3Dベクトルの平行移動が出来る行列を取得する関数
MyMatrix_D MyMatrix_D::GetTransportion3D(const VECTOR3D_D &transportion)
{
    MyMatrix_D temp = MyMatrix_D::GetIdentity(4);
    temp.SetParam(3, 0, transportion.GetX());
    temp.SetParam(3, 1, transportion.GetY());
    temp.SetParam(3, 2, transportion.GetZ());
    return temp;
}

#endif // USE_MY_VECTOR3D

#ifdef USE_MY_ROTATION
#ifdef USE_MY_QUATERNION
MyQuaternion MyMatrix_D::ToQtn() const
{
    /*
    MyQuaternion temp(0.0, 0.0, 0.0, 0.0);
    double px = this->GetParam(0, 0) - this->GetParam(1, 1) - this->GetParam(2, 2) + 1;
    double py = -this->GetParam(0, 0) + this->GetParam(1, 1) - this->GetParam(2, 2) + 1;
    double pz = -this->GetParam(0, 0) - this->GetParam(1, 1) + this->GetParam(2, 2) + 1;
    double pw = this->GetParam(0, 0) + this->GetParam(1, 1) + this->GetParam(2, 2) + 1;

    auto selected = 0;
    auto max = px;
    if (max < py) {
        selected = 1;
        max = py;
    }
    if (max < pz) {
        selected = 2;
        max = pz;
    }
    if (max < pw) {
        selected = 3;
        max = pw;
    }

    if (selected == 0)
    {
        double x = sqrt(px) * 0.5;
        double d = 1 / (4 * x);
        temp = MyQuaternion(
            x,
            (this->GetParam(1, 0) + this->GetParam(0, 1)) * d,
            (this->GetParam(0, 2) + this->GetParam(2, 0)) * d,
            (this->GetParam(2, 1) - this->GetParam(1, 2)) * d
        );
    }
    else if (selected == 1)
    {
        double y = sqrt(py) * 0.5;
        double d = 1 / (4 * y);
        temp = MyQuaternion(
            (this->GetParam(1, 0) + this->GetParam(0, 1)) * d,
            y,
            (this->GetParam(2, 1) + this->GetParam(1, 2)) * d,
            (this->GetParam(0, 2) - this->GetParam(2, 0)) * d
        );
    }
    else if (selected == 2)
    {
        double z = sqrt(pz) * 0.5;
        double d = 1 / (4 * z);
        temp = MyQuaternion(
            (this->GetParam(0, 2) + this->GetParam(2, 0)) * d,
            (this->GetParam(2, 1) + this->GetParam(1, 2)) * d,
            z,
            (this->GetParam(1, 0) - this->GetParam(0, 1)) * d
        );
    }
    else if (selected == 3)
    {
        double w = sqrt(pw) * 0.5;
        double d = 1 / (4 * w);
        temp = MyQuaternion(
            (this->GetParam(2, 1) - this->GetParam(1, 2)) * d,
            (this->GetParam(0, 2) - this->GetParam(2, 0)) * d,
            (this->GetParam(1, 0) - this->GetParam(0, 1)) * d,
            w
        );
    }
    */

    // ↓の計算は(http://marupeke296.com/DXG_No58_RotQuaternionTrans.html)を参考にした

    // 最大成分を検索
    double elem[4]; // 0:x, 1:y, 2:z, 3:w
    elem[0] = this->GetParam(0, 0) - this->GetParam(1, 1) - this->GetParam(2, 2) + 1.0;
    elem[1] = -this->GetParam(0, 0) + this->GetParam(1, 1) - this->GetParam(2, 2) + 1.0;
    elem[2] = -this->GetParam(0, 0) - this->GetParam(1, 1) + this->GetParam(2, 2) + 1.0;
    elem[3] = this->GetParam(0, 0) + this->GetParam(1, 1) + this->GetParam(2, 2) + 1.0;

    unsigned biggestIndex = 0;
    for (int i = 1; i < 4; i++)
    {
        if (elem[i] > elem[biggestIndex])
        {
            biggestIndex = i;
        }
    }
    if (elem[biggestIndex] < 0.0)
    {
        return MyQuaternion(0.0, 0.0, 0.0, 1.0); // 引数の行列に間違いあり！
    }

    // 最大要素の値を算出
    double q[4] = { 0.0, 0.0, 0.0, 0.0 };
    double v = sqrt(elem[biggestIndex]) * 0.5;
    q[biggestIndex] = v;
    double mult = 0.25 / v;
    switch (biggestIndex)
    {
    case 0: // x
        q[1] = (this->GetParam(0, 1) + this->GetParam(1, 0)) * mult;
        q[2] = (this->GetParam(2, 0) + this->GetParam(0, 2)) * mult;
        q[3] = (this->GetParam(1, 2) - this->GetParam(2, 1)) * mult;
        break;
    case 1: // y
        q[0] = (this->GetParam(0, 1) + this->GetParam(1, 0)) * mult;
        q[2] = (this->GetParam(1, 2) + this->GetParam(2, 1)) * mult;
        q[3] = (this->GetParam(2, 0) - this->GetParam(0, 2)) * mult;
        break;
    case 2: // z
        q[0] = (this->GetParam(2, 0) + this->GetParam(0, 2)) * mult;
        q[1] = (this->GetParam(1, 2) + this->GetParam(2, 1)) * mult;
        q[3] = (this->GetParam(0, 1) - this->GetParam(1, 0)) * mult;
        break;
    case 3: // w
        q[0] = (this->GetParam(1, 2) - this->GetParam(2, 1)) * mult;
        q[1] = (this->GetParam(2, 0) - this->GetParam(0, 2)) * mult;
        q[2] = (this->GetParam(0, 1) - this->GetParam(1, 0)) * mult;
        break;
    }
    MyQuaternion temp(q[0], q[1], q[2], q[3]);
    return temp;
}
#endif // USE_MY_QUATERNION

#ifdef USE_MY_EULERANGLE
MyEulerAngle MyMatrix_D::ToEulerAngle() const
{
    return this->ToEulerAngle(globalOrder);
}

MyEulerAngle MyMatrix_D::ToEulerAngle(EulerOrder order) const
{
    MyEulerAngle temp(order, 0.0, 0.0, 0.0);
    if (order == EulerOrder::XYZ)
    {
        auto sy = this->GetParam(0, 2);
        auto unlocked = fabs(sy) < 0.99999;
        temp.SetParam(
            order,
            unlocked ? atan2(-this->GetParam(1, 2), this->GetParam(2, 2)) : atan2(this->GetParam(2, 1), this->GetParam(1, 1)),
            asin(sy),
            unlocked ? atan2(-this->GetParam(0, 1), this->GetParam(0, 0)) : 0);
    }
    else if (order == EulerOrder::XZY)
    {
        auto sz = -this->GetParam(0, 1);
        auto unlocked = fabs(sz) < 0.99999;
        temp.SetParam(
            order,
            unlocked ? atan2(this->GetParam(2, 1), this->GetParam(1, 1)) : atan2(-this->GetParam(1, 2), this->GetParam(2, 2)),
            unlocked ? atan2(this->GetParam(0, 2), this->GetParam(0, 0)) : 0,
            asin(sz));
    }
    else if (order == EulerOrder::YXZ)
    {
        auto sx = -this->GetParam(1, 2);
        auto unlocked = fabs(sx) < 0.99999;
        temp.SetParam(
            order,
            asin(sx),
            unlocked ? atan2(this->GetParam(0, 2), this->GetParam(2, 2)) : atan2(-this->GetParam(2, 0), this->GetParam(0, 0)),
            unlocked ? atan2(this->GetParam(1, 0), this->GetParam(1, 1)) : 0);
    }
    else if (order == EulerOrder::YZX)
    {
        auto sz = this->GetParam(1, 0);
        auto unlocked = fabs(sz) < 0.99999;
        temp.SetParam(
            order,
            unlocked ? atan2(-this->GetParam(1, 2), this->GetParam(1, 1)) : 0,
            unlocked ? atan2(-this->GetParam(2, 0), this->GetParam(0, 0)) : atan2(this->GetParam(0, 2), this->GetParam(2, 2)),
            asin(sz));
    }
    else if (order == EulerOrder::ZXY)
    {
        auto sx = this->GetParam(2, 1);
        auto unlocked = fabs(sx) < 0.99999;
        temp.SetParam(
            order,
            asin(sx),
            unlocked ? atan2(-this->GetParam(2, 0), this->GetParam(2, 2)) : 0,
            unlocked ? atan2(-this->GetParam(0, 1), this->GetParam(1, 1)) : atan2(this->GetParam(1, 0), this->GetParam(0, 0)));
    }
    else if (order == EulerOrder::ZYX)
    {
        auto sy = -this->GetParam(2, 0);
        auto unlocked = fabs(sy) < 0.99999;
        temp.SetParam(
            order,
            unlocked ? atan2(this->GetParam(2, 1), this->GetParam(2, 2)) : 0,
            asin(sy),
            unlocked ? atan2(this->GetParam(1, 0), this->GetParam(0, 0)) : atan2(-this->GetParam(0, 1), this->GetParam(1, 1)));
    }
    return temp;
}
#endif // USE_MY_EULERANGLE
#endif // USE_MY_ROTATION

MyMatrix_D operator +(const MyMatrix_D &matL, const MyMatrix_D &matR)
{
    return matL.Add(matR);
}

MyMatrix_D operator -(const MyMatrix_D &matL, const MyMatrix_D &matR)
{
    return matL.Sub(matR);
}

MyMatrix_D operator *(const MyMatrix_D &mat, const double &scaler)
{
    return mat.ScaleScalar(scaler);
}

MyMatrix_D operator *(const MyMatrix_D &matL, const MyMatrix_D &matR)
{
    return matL.ScaleMatrix(matR);
}

MyMatrix_D operator %(const MyMatrix_D &matL, const MyMatrix_D &matR)
{
    return matL.ScaleElement(matR);
}

#endif // USE_MY_MATRIX
