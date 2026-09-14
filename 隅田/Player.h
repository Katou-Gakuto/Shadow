#include "DXLib.h
#include "KeyState.h"
#include "BaseComponent.h"



class Player : public BaseComponent
{
public:
    KeyState keystate;

    int Create() override;
    int Initialize() override;
    int Finalize() override;
    int EarlyUpdate(KeyState keystate) override;
    int Update() override;
    int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision) override;
    int LateUpdate() override;
    int Draw() override;

private:

    struct PlayerData
    {
        VECTOR3D Pos;
        VECTOR3D OldPos;
        VECTOR3D moveVec;
        int PlayerNum //P1‚©P2‚©
    };

    PlayerData PDate;
}