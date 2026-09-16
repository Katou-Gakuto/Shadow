#pragma once

#include "PuzzleGimmickData.h"

// 
class ButtonObjectController;

// ボタンの状態を確認し、シミュレーションを行うかを判断するクラス
class ButtonTrigger : public BaseGimmickTrigger
{
public:
    // コンストラクタ
    ButtonTrigger();

    // デストラクタ
    ~ButtonTrigger() override;

    // シミュレーションしてきたギミックを実行に移すかを判断する関数
    // ※GameObject::Update()のタイミングで呼ばれ、自身と紐づけられてるExecutor::Update()の直前で呼ばれます
    bool GetSignal() override;

    // 
    bool AddButton(ButtonObjectController *button);

private:
    // これらのやつが全てFlagとなるので
    std::vector<ButtonObjectController *> mlFlagButtonBox;
};
