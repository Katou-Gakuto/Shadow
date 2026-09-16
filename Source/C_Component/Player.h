#include "DxLib.h"
#include "KeyState.h"
#include "BaseComponent.h"

#include "VECTOR.h"

class Player : public BaseComponent
{
public:
    KeyState keystate;

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

private:

    VECTOR2D Pos;
    VECTOR2D OldPos;
    VECTOR2D moveVec;
    int PlayerNum; //P1‚©P2‚©
};