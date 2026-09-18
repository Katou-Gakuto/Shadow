#include "DxLib.h"
#include "KeyState.h"
#include "BaseComponent.h"

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

    // 
    int HoldObject(BaseCollision *hitCollision);

    // 
    int HoldMove();

    // 
    bool CheckHoldObject(const HoldObjectController *hold) const;

	VECTOR2D GetPos() const { return Pos; }

private:

    VECTOR2D Pos;
    VECTOR2D OldPos;
    VECTOR2D moveVec;
    int PlayerNum; //P1かP2か

	int PlayerGraphHandle; // プレイヤーのグラフィックハンドル

    
    HoldObjectController *mpHold; // 自身がつかんでいるオブジェクト

    // 
    bool CheckHoldNow() const;
};