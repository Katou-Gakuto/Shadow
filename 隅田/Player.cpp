#include "Player.h"

Player::Create()
{
    Pos = OldPos;
}


Player::Initialize()
{
}


Player::Finalize()
{
}


Player::EarlyUpdate(KeyState keystate)
{
    OldPos = Pos;


	//十字ボタン
	if (keystate.GetKey_Controller(CONTROLLER_KEY_TYPE::UP, CONTROLLER_KEY_NUMBER::CONTROLLER_1))
	{
        moveVec.y = -4;
	}
	if (keystate.GetKey_Controller(CONTROLLER_KEY_TYPE::DOWN, CONTROLLER_KEY_NUMBER::CONTROLLER_1))
	{
        moveVec.y = 4;
	}
	if (keystate.GetKey_Controller(CONTROLLER_KEY_TYPE::LEFT, CONTROLLER_KEY_NUMBER::CONTROLLER_1))
	{
        moveVec.x = -4;
	}
	if (keystate.GetKey_Controller(CONTROLLER_KEY_TYPE::RIGHT, CONTROLLER_KEY_NUMBER::CONTROLLER_1))
	{
        moveVec.x = 4;
	}
}


Player::Update()
{
}


Player::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    // 自身の攻撃の当たり判定が相手の体の当たり判定にあたっている場合は処理を行う
    if (myCollision->GetCollisionTag() == CollisionTag::CollisionTag_Attack &&
        hitCollision->GetCollisionTag() == CollisionTag::CollisionTag_CharaBody &&
        myCollision->GetMyObject() != hitCollision->GetMyObject())
    {

    }
    return 0;
}


Player::LateUpdate()
{
    if (moveVec.x != 0)
        Pos.x += moveVec.x;
    if (moveVec.y != 0)
        Pos.y += moveVec.y;
}


Player::Draw()
{
    DrawBox(Pos.x; Pos.y, Pos.x + 30, Pos.y + 60, 0xFFFFFF, true);
}