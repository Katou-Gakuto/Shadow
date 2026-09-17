#pragma once

#include "BaseComponent.h"

// オブジェクトを掴めるようにする拡張パーツ
class HoldObjectController : public BaseComponent
{
public:
    // 
    HoldObjectController(GameObject *myObject, bool doubleHold);

    // 
    ~HoldObjectController() override;

    // 
    int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision) override;

    // 
    bool CheckHold() const;


private:
    // 2人で掴まなければいけない場合はtrue
    bool mbDoubleHold;
};