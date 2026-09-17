#include "HoldTrigger.h"

#include "../C_Component/Player.h"
#include "../C_Component/HoldObjectController.h"

#include "../Z_Except/Master.h"

// 
HoldTrigger::HoldTrigger(HoldObjectController *myHold) :
    mpMyHold(myHold),
    BaseGimmickTrigger()
{
}

// 
HoldTrigger::~HoldTrigger()
{
}

// シミュレーションしてきたギミックを実行に移すかを判断する関数
// ※GameObject::Update()のタイミングで呼ばれ、自身と紐づけられてるExecutor::Update()の直前で呼ばれます
bool HoldTrigger::GetSignal()
{
    // 
    if (this->mpMyHold == nullptr)
    {
        // 
        return false;
    }

    // 掴まれているかを確認し、それをそのまま返す
    return this->mpMyHold->CheckHold();
}
