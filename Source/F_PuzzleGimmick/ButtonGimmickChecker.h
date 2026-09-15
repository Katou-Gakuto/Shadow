#pragma once

#include "PuzzleGimmickData.h"

// 
class ButtonObjectController;

// ボタンの状態を確認し、シミュレーションを行うかを判断するクラス
class ButtonSumilationChecker : public BaseGimmickChecker
{
public:
    // 
    ButtonSumilationChecker();

    // 
    ~ButtonSumilationChecker() override;

    // 
    bool GetSignalSumilate() override;

    // 
    bool GetSignalExecute()override;

    // 
    bool AddButton(ButtonObjectController *button);

private:
    // これらのやつが全てFlagとなるので
    std::vector<ButtonObjectController *> mlFlagButtonBox;
};
