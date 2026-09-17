#pragma once

#include "BaseComponent.h"

#include "../Y_Tool/VECTOR.h"

// オブジェクトを掴めるようにする拡張パーツ
class HoldObjectController : public BaseComponent
{
public:
    // コンストラクタ
    HoldObjectController(GameObject *myObject, bool doubleHold);

    // デストラクタ
    ~HoldObjectController() override;

    // 自身の当たり判定が何かと当たった場合に行う処理
    int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision) override;

    // プレイヤーが呼び出す関数で、プレイヤーと移動ベクトルを同期する関数
    bool SyncHoldMoveVec(const VECTOR2D &holdMoveVec);

    // こっちが呼び出す関数で、移動ベクトルをプレイヤーに同期させる関数
    int SetPlayerMoveVec(bool lightSide);

    // 現在プレイヤーに掴まれているかを確認する関数
    bool CheckHold() const;

private:
    // 2人で掴まなければいけない場合はtrue
    bool mbDoubleHold;
};