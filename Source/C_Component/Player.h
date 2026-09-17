#include "DxLib.h"
#include "KeyState.h"
#include "BaseComponent.h"

#include "../S_Collision/CollisionHandle.h"

#include "VECTOR.h"

class HoldObjectController;

class Player : public BaseComponent
{
public:

    Player(GameObject* myObject, int playerNumber);
    ~Player();

    int Create() override;
    int Initialize() override;
    int Finalize() override;
    int EarlyUpdate() override;
    int Update() override;
    int HitOnCollision(BaseCollision* myCollision, BaseCollision* hitCollision) override;
    int LateUpdate() override;
    int Draw() override;

    // HitOnCollision()が呼ばれたら行う関数で、オブジェクトが掴める際に掴む関数
    int Hold(BaseCollision *hitCollision);

    // オブジェクトをつかんでいる状態でHitOnCollision()が呼ばれたら行う関数
    int HoldMove();

    // 
    bool CheckHoldObject(const HoldObjectController *hold) const;

    // 
    bool SyncPlayerMoveVec(const VECTOR2D &vec);

private:

    VECTOR2D Pos;
    VECTOR2D OldPos;
    VECTOR2D moveVec;
    int PlayerNum; //P1かP2か

    // 自身がつかんでいるオブジェクト
    HoldObjectController *mpHold;

    // このプレイヤーの当たり判定
    CollisionHandle mdBodyCollision;

    // 
    bool mbHoldFlag;

    // 
    bool CheckHoldNow() const;
};