#pragma once

#include <vector>

// シュミレーションに関するするクラス
// 何を条件にシュミレーションを行い、どのような出力をするのかは継承先によります
class BaseGimmickChecker
{
public:
    // デフォルトコンストラクタ
    BaseGimmickChecker() = default;

    // デフォルトデストラクタ
    virtual ~BaseGimmickChecker() = default;

    // シュミレーションを行うかを判断する関数
    // 基本的には継承先のメンバの値を更新してからこの関数を呼び出すことになると思います
    virtual bool CheckSumilate() = 0;

    // シュミレーションしてきたギミックを実行に移すかを判断する関数
    // これもCheckSumilate()と同様に、継承先のメンバの値によって変える形で
    virtual bool CheckExecute() = 0;
};

// パズルギミックの内容をシュミレーションするクラス
// どのようなギミックかは継承先によります
// シュミレーションの出力結果は継承先のクラスの
class BaseGimmickSumilater
{
public:
    // デフォルトコンストラクタ
    BaseGimmickSumilater() = default;

    // デフォルトデストラクタ
    ~BaseGimmickSumilater() = default;

    // シュミレーションを行う関数
    // 基本的には継承先のメンバの値を更新してからこの関数を呼び出すことになると思います
    virtual int Sumilate() = 0;
};

// パズルのギミックに必要なデータをまとめた構造体
// ギミックを起動するかをmpCheckerが、ギミックの内容をmpSumilaterが担う
struct PuzzleGimmickData
{
    // シュミレーションを行うかや、それを実行に移すか判断するメンバ
    BaseGimmickChecker *mpChecker;

    // シュミレーションを行うメンバ
    BaseGimmickSumilater *mpSumilater;
};
