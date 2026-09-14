#include "Player.h"
#include "GameObject2D.h"
#include "Master.h"

Player::Player(GameObject* myObject)
: BaseComponent(myObject, ComponentTagAndOrder::CTAO_CharacterLife)
{
}

Player::~Player()
{
}

int Player::Create()
{
    Pos = OldPos;
    return 0;
}


int Player::Initialize()
{
    return 0;
}


int Player::Finalize()
{
    return 0;
}


int Player::EarlyUpdate()
{
    moveVec = VECTOR2D::GetZero();

	//十字ボタン
	if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::UP)/*(CONTROLLER_KEY_TYPE::UP, CONTROLLER_KEY_NUMBER::CONTROLLER_1)*/)
	{
        moveVec.SetY(-4);
	}
	if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::DOWN)/*(CONTROLLER_KEY_TYPE::DOWN, CONTROLLER_KEY_NUMBER::CONTROLLER_1)*/)
	{
        moveVec.SetY(4);
	}
	if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::LEFT)/*(CONTROLLER_KEY_TYPE::LEFT, CONTROLLER_KEY_NUMBER::CONTROLLER_1)*/)
	{
        moveVec.SetX(-4);
	}
	if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::RIGHT)/*(CONTROLLER_KEY_TYPE::RIGHT, CONTROLLER_KEY_NUMBER::CONTROLLER_1)*/)
	{
        moveVec.SetX(4);
	}
    return 0;
}


int Player::Update()
{
    GameObject2D* player = GetMyObject2D();
    player->SetMoveVec(moveVec);
    return 0;
}


int Player::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    //// 自身の攻撃の当たり判定が相手の体の当たり判定にあたっている場合は処理を行う
    //if (myCollision->GetCollisionTag() == CollisionTag::CollisionTag_Attack &&
    //    hitCollision->GetCollisionTag() == CollisionTag::CollisionTag_CharaBody &&
    //    myCollision->GetMyObject() != hitCollision->GetMyObject())
    //{

    //}
    return 0;
}


int Player::LateUpdate()
{
    return 0;
}


int Player::Draw()
{
    GameObject2D* player = GetMyObject2D();
    DrawBox(player->GetPosition().GetX(), player->GetPosition().GetY(), player->GetPosition().GetX() + 30, player->GetPosition().GetY() + 60, 0xFFFFFF, true);
    return 0;
}