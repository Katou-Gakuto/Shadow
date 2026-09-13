#pragma once

#include "PuzzleGimmickData.h"

// 
class ButtonObjectController;

// ボタンの状態を確認し、シュミレーションを行うかを判断するクラス
class ButtonSumilationChecker : public BaseGimmickChecker
{
public:
    // 
    ButtonSumilationChecker();

    // 
    ~ButtonSumilationChecker() override;

    // 
    bool CheckSumilate() override;

    // 
    bool CheckExecute()override;

    // 
    bool AddButton(ButtonObjectController *button);

private:
    // これらのやつが全てFlagとなるので
    std::vector<ButtonObjectController *> mlFlagButtonBox;
};
