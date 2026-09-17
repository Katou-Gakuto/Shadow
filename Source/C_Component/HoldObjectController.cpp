#include "HoldObjectController.h"

#include "Player.h"

#include "../A_GameObject/GameObject2D.h"

#include "../S_Collision/BaseCollisionList.h"

#include "../Z_Except/Master.h"

// コンストラクタ
HoldObjectController::HoldObjectController(GameObject *myObject, bool doubleHold) :
    mbDoubleHold(doubleHold),
    BaseComponent(myObject, ComponentTagAndOrder::CTAO_HoldController)
{
}

// デストラクタ
HoldObjectController::~HoldObjectController()
{
}

// 自身の当たり判定が何かと当たった場合に行う処理
int HoldObjectController::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    // 
    GameObject2D *hitObj = hitCollision->GetMyObject2D();

    // breakで一番下まで飛びたいだけ
    do
    {
        // 自身の当たり判定同士が当たっても何もしない
        if (myCollision->GetMyObject2D() == hitObj)
        {
            break;
        }

        // プレイヤーと当たった場合もこちらは何もしない
        if (hitObj == Master::mpPlayerLight->GetMyObject2D() &&
            hitObj == Master::mpPlayerShadow->GetMyObject2D())
        {
            break;
        }

        // 
        if (this->mbDoubleHold)
        {
            // 
            if (Master::mpPlayerLight->CheckHoldObject(this) &&
                Master::mpPlayerShadow->CheckHoldObject(this))
            {
                // 
                this->SetPlayerMoveVec(true);

                // 
                this->SetPlayerMoveVec(false);
            }
        }
        else
        {
            // 
            if (Master::mpPlayerLight->CheckHoldObject(this))
            {
                // 
                this->SetPlayerMoveVec(true);
            }
            else if (Master::mpPlayerShadow->CheckHoldObject(this))
            {
                // 
                this->SetPlayerMoveVec(false);
            }
        }
    } while (false);

    // 
    return 0;
}

// プレイヤーが呼び出す関数で、プレイヤーと移動ベクトルを同期する関数
bool HoldObjectController::SyncHoldMoveVec(const VECTOR2D &holdMoveVec)
{
    // 
    GameObject2D *obj = this->GetMyObject2D();
    if (obj == nullptr)
    {
        // 
        return false;
    }

    // 
    BaseCollisionList *list = obj->GetBaseCollisionList();
    if (list == nullptr)
    {
        // 
        return false;
    }

    // 
    obj->SetMoveVec(holdMoveVec);

    // 
    list->SetCollisionMoveVec(CollisionDimension::CollisionDimension_2D, &holdMoveVec);

    // 
    return true;
}

// こっちが呼び出す関数で、移動ベクトルをプレイヤーに同期させる関数
int HoldObjectController::SetPlayerMoveVec(bool lightSide)
{
    // 
    if (lightSide)
    {
        Master::mpPlayerLight->SyncPlayerMoveVec(this->GetMyObject2D()->GetMoveVec2D());
    }
    else
    {
        Master::mpPlayerShadow->SyncPlayerMoveVec(this->GetMyObject2D()->GetMoveVec2D());
    }

    // 
    return 0;
}

// 現在プレイヤーに掴まれているかを確認する関数
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
