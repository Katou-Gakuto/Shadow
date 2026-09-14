#pragma once

#include <vector>

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
    virtual bool CheckSumilate() = 0;

    // シミュレーションしてきたギミックを実行に移すかを判断する関数
    // これもCheckSumilate()と同様に、継承先のメンバの値によって変える形で
    virtual bool CheckExecute() = 0;
};

// パズルギミックの内容をシミュレーションするクラス
// どのようなギミックかは継承先によります
// シミュレーションの出力結果は継承先のクラスの
class BaseGimmickSumilater
{
public:
    // デフォルトコンストラクタ
    BaseGimmickSumilater() = default;

    // デフォルトデストラクタ
    ~BaseGimmickSumilater() = default;

    // シミュレーションを行うかをCheckerの結果も踏まえたうえで判断する関数
    virtual bool CheckSumilate(bool checkerResult) = 0;

    // シミュレーションを行う関数
    virtual int Sumilate() = 0;

    // 実行に移すかをCheckerの結果も踏まえたうえで判断する関数
    virtual int CheckExecute() = 0;

    // シミュレートの結果を実行を行う関数
    virtual int Execute() = 0;
};

// パズルのギミックに必要なデータをまとめた構造体
// ギミックを起動するかをmpCheckerが、ギミックの内容をmpSumilaterが担う
struct PuzzleGimmickData
{
    // シミュレーションを行うかや、それを実行に移すか判断するメンバ
    BaseGimmickChecker *mpChecker;

    // シミュレーションを行うメンバ
    BaseGimmickSumilater *mpSumilater;
};
