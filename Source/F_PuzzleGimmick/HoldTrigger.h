#pragma once

#include "PuzzleGimmickData.h"

// 
class HoldObjectController;

// プレイヤーによって掴まれているかを確認するクラス
class HoldTrigger : public BaseGimmickTrigger
{
public:
    // 
    HoldTrigger(HoldObjectController *myHold);

    // 
    ~HoldTrigger();

    // シミュレーションしてきたギミックを実行に移すかを判断する関数
    // ※GameObject::Update()のタイミングで呼ばれ、自身と紐づけられてるExecutor::Update()の直前で呼ばれます
    bool GetSignal() override;

private:
    // 
    HoldObjectController *mpMyHold;
};
