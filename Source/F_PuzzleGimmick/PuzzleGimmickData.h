#pragma once

#include <vector>

#include "../Y_Tool/FlagByte.h"

// シミュレーションに関するするクラス
// 何を条件にシミュレーションを行い、どのような出力をするのかは継承先によります
class BaseGimmickChecker
{
public:
    // デフォルトコンストラクタ
    BaseGimmickChecker() = default;

    // デフォルトデストラクタ
    virtual ~BaseGimmickChecker() = default;

    // シミュレーションを行うかを判断する関数
    // 基本的には継承先のメンバの値を更新してからこの関数を呼び出すことになると思います
    virtual bool GetSignalSumilate() = 0;

    // シミュレーションしてきたギミックを実行に移すかを判断する関数
    // これもCheckSumilate()と同様に、継承先のメンバの値によって変える形で
    virtual bool GetSignalExecute() = 0;
};

// ここで言うActiveというのはギミックが起動しているかということ
// たとえば逆再生中のMoveはActiveです。動いているので。
// なので完全にストップしたらDeactiveです。
class GimmickActiveParam
{
public:
    // 
    GimmickActiveParam() = default;

    // 
    ~GimmickActiveParam() = default;

    // 
    bool GetSignalNot() const { return this->mbFlags[0]; }

    // 
    bool GetOnePushActive() const { return this->mbFlags[1]; }

    // 
    void SetSignalNot(bool flag) { this->mbFlags[0] = flag; }

    // 
    void SetOnePushActive(bool flag) { this->mbFlags[1] = flag; }

private:
    // 0 ? NotFlag : Flag(トリガーの条件にNOT演算をするか)
    // 1 ? OnePushActive : HitFrame(細々した計算が必要ならtrueで、トリガーの条件がOffになったらOffがfalse)
    // 2 ? 
    // 3 ? 
    // 4 ? 
    // 5 ? 
    // 6 ? 
    // 7 ? 
    FlagByte mbFlags;


    // loopはMove固有の考え方
    // returnもMove固有の考え方
    // frameCountもMove固有の考え方
};

// パズルギミックの内容をシミュレーションするクラス
// どのようなギミックかは継承先によります
// シミュレーションの出力結果は継承先のクラスのメンバ変数です
class BaseGimmickSumilator
{
public:
    // デフォルトコンストラクタ
    BaseGimmickSumilator() = default;

    // デフォルトデストラクタ
    virtual ~BaseGimmickSumilator() = default;

    // シミュレーションを行うかをCheckerの結果も踏まえたうえで判断する関数
    virtual bool CheckSumilate(bool checkerSignal) const = 0;

    // シミュレーションを行う関数
    virtual int Sumilate() = 0;

    // 実行に移すかをCheckerの結果も踏まえたうえで判断する関数
    virtual int CheckExecute(bool checkerSignal) const = 0;

    // シミュレートの結果を実行を行う関数
    virtual int Execute() = 0;

    // シミュレーション内容を描画する関数
    // ※既に実行段階である場合は引数がtrueになります。実行段階では描画しない、あるいはその逆の場合はこの引数を使ってください。
    virtual int Draw(bool sumilating, bool executing) = 0;

protected:
    // 
    GimmickActiveParam mdActiveParam;
};

// パズルのギミックに必要なデータをまとめた構造体
// ギミックを起動するかをmpCheckerが、ギミックの内容をmpSumilaterが担う
struct PuzzleGimmickData
{
    // シミュレーションを行うかや、それを実行に移すか判断するメンバ
    BaseGimmickChecker *mpChecker;

    // シミュレーションを行うメンバ
    BaseGimmickSumilator *mpSumilater;

    // このフレーム時点でシミュレーション段階にあるか
    bool mbSumilating;

    // このフレーム時点で実行段階にあるか
    bool mbExecuting;
};
