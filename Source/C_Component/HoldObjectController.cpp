#include "HoldObjectController.h"

#include "Player.h"

#include "../Z_Except/Master.h"

// 
HoldObjectController::HoldObjectController(GameObject *myObject, bool doubleHold) :
    mbDoubleHold(doubleHold),
    BaseComponent(myObject, ComponentTagAndOrder::CTAO_HoldController)
{
}

// 
HoldObjectController::~HoldObjectController()
{
}

// 
int HoldObjectController::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    // 
    return 0;
}

// 
bool HoldObjectController::CheckHold() const
{
    // 
    bool signal = false;

    // 光のプレイヤー
    bool p1hold = Master::mpPlayerLight->CheckHoldObject(this);

    // 影のプレイヤー
    bool p2hold = Master::mpPlayerShadow->CheckHoldObject(this);

    // 
    if (this->mbDoubleHold)
    {
        // 2人のプレイヤーに捕まれている場合は処理を行う
        signal = (p1hold && p2hold);
    }
    else
    {
        // 片方のみがtrueだったら1人に掴まれているので
        signal = (p1hold != p2hold);
    }

    // 
    return signal;
}
