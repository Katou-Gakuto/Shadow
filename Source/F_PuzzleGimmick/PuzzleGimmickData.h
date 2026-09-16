#pragma once

#include <vector>

#include "../Y_Tool/FlagByte.h"

// ギミックの内容を実行するかを判断するクラス
// 継承先のメンバーを更新してから関数を呼んでください
class BaseGimmickTrigger
{
public:
    // デフォルトコンストラクタ
    BaseGimmickTrigger() = default;

    // デフォルトデストラクタ
    virtual ~BaseGimmickTrigger() = default;

    // シミュレーションしてきたギミックを実行に移すかを判断する関数
    // ※GameObject::Update()のタイミングで呼ばれ、自身と紐づけられてるExecutor::Update()の直前で呼ばれます
    virtual bool GetSignal() = 0;
};

// ここで言うActiveというのはギミックが起動しているかということ
// たとえば逆再生中のMoveはActiveです。動いているので。
// なので完全にストップしたらDeactiveです。
class PuzzleGimmickActiveParam
{
public:
    // 
    PuzzleGimmickActiveParam() = default;

    // 
    ~PuzzleGimmickActiveParam() = default;

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
};

// パズルギミックの内容を実行するクラス
// どのようなギミックかは継承先により、ギミック実行の出力結果は継承先のクラスのメンバ変数です
class BaseGimmickExecutor
{
public:
    // デフォルトコンストラクタ
    BaseGimmickExecutor(PuzzleGimmickActiveParam param) :
        mdActiveParam(param)
    {
    }

    // デフォルトデストラクタ
    virtual ~BaseGimmickExecutor()
    {
    }

    // ギミックの内容を実行する関数
    // ※GameObject::EarlyUpdate()のタイミングで呼ばれます
    virtual int EarlyUpdate(bool triggerSignal) = 0;

    // ギミックの内容を実行する関数
    // ※GameObject::Update()のタイミングで呼ばれます
    virtual int Update(bool triggerSignal) = 0;

    // ギミックの内容を実行する関数
    // ※GameObject::LateUpdate()のタイミングで呼ばれます
    virtual int LateUpdate(bool triggerSignal) = 0;

    // ギミック内容を描画する関数
    // ※既に実行段階である場合は引数がtrueになります。実行段階では描画しない、あるいはその逆の場合はこの引数を使ってください。
    virtual int Draw(bool triggerSignal) = 0;

    // ギミックの内容を実行するかの判断材料を取得する関数
    const PuzzleGimmickActiveParam &GetActiveParam() const { return this->mdActiveParam; }

private:
    // ギミックの内容を実行するかの判断材料を取得する関数
    PuzzleGimmickActiveParam mdActiveParam;
};

// パズルのギミックに必要なデータをまとめた構造体
// ギミックを起動するかをmpTriggerが、ギミックの内容をmpSumilaterが担う
struct PuzzleGimmickData
{
    // シミュレーションを行うかや、それを実行に移すか判断するメンバ
    BaseGimmickTrigger *mpTrigger;

    // シミュレーションを行うメンバ
    BaseGimmickExecutor *mpSumilater;

    // このフレームでトリガーがどのようなシグナルを出したか
    bool mbTriggerSignal;
};
