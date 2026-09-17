#include "Player.h"
#include "GameObject2D.h"
#include "Master.h"

#include "../E_Scene/BaseScene.h"
#include "../E_Scene/BaseSceneManager.h"

#include "../S_Collision/BaseCollision.h"
#include "../S_Collision/Circle2D.h"

Player::Player(GameObject* myObject, int playerNumber)
: BaseComponent(myObject, ComponentTagAndOrder::CTAO_PlayerController)
, PlayerNum(playerNumber)
{
}

Player::~Player()
{
}

int Player::Create()
{
    // 
    GameObject2D *obj = this->GetMyObject2D();
    if (obj == nullptr)
    {
        // 
        return -1;
    }
    
    // 
    Circle2D *body = new Circle2D(
        VECTOR2D::GetZero(),
        0.0f,
        obj,
        CollisionTag::CollisionTag_CharaBody,
        CollisionNorm::CollisionNorm_Out,
        false,
        true,
        1.0f);

    // 
    obj->AddCollision(body, 0);

    // 
    return 0;
}


int Player::Initialize()
{
    Pos = OldPos;
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
	if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::UP, PlayerNum - 1)/*(CONTROLLER_KEY_TYPE::UP, CONTROLLER_KEY_NUMBER::CONTROLLER_1)*/)
	{
        moveVec.SetY(-4);
	}
	if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::DOWN, PlayerNum - 1)/*(CONTROLLER_KEY_TYPE::DOWN, CONTROLLER_KEY_NUMBER::CONTROLLER_1)*/)
	{
        moveVec.SetY(4);
	}
	if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::LEFT, PlayerNum - 1)/*(CONTROLLER_KEY_TYPE::LEFT, CONTROLLER_KEY_NUMBER::CONTROLLER_1)*/)
	{
        moveVec.SetX(-4);
	}
	if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::RIGHT, PlayerNum - 1)/*(CONTROLLER_KEY_TYPE::RIGHT, CONTROLLER_KEY_NUMBER::CONTROLLER_1)*/)
	{
        moveVec.SetX(4);
	}

	//ボタン
	if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::A, PlayerNum - 1)/*(CONTROLLER_KEY_TYPE::UP, CONTROLLER_KEY_NUMBER::CONTROLLER_1)*/)
	{
		// この中にAを押したときの処理を追加する
	}
	if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::B, PlayerNum - 1)/*(CONTROLLER_KEY_TYPE::DOWN, CONTROLLER_KEY_NUMBER::CONTROLLER_1)*/)
	{
		// この中にBを押したときの処理を追加する
	}
	if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::X, PlayerNum - 1)/*(CONTROLLER_KEY_TYPE::LEFT, CONTROLLER_KEY_NUMBER::CONTROLLER_1)*/)
	{
		// この中にXを押したときの処理を追加する
	}
	if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::Y, PlayerNum - 1)/*(CONTROLLER_KEY_TYPE::RIGHT, CONTROLLER_KEY_NUMBER::CONTROLLER_1)*/)
	{
		// この中にYを押したときの処理を追加する
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
    // 自身の攻撃の当たり判定が相手の体の当たり判定にあたっている場合は処理を行う
    if (myCollision->GetMyObject() != hitCollision->GetMyObject() && 
        myCollision->GetCollisionTag() == CollisionTag::CollisionTag_CharaBody)
    {
        // 当たったオブジェクトが普通のオブジェクト、ライト、鏡のいずれかの場合は処理を行う
        if (hitCollision->GetCollisionTag() == CollisionTag::CollisionTag_Wall ||
            hitCollision->GetCollisionTag() == CollisionTag::CollisionTag_LightBody ||
            hitCollision->GetCollisionTag() == CollisionTag::CollisionTag_Mirror)
        {
            // 
            HoldObject(hitCollision);
        }
    }

    // 
    return 0;
}


int Player::LateUpdate()
{
    return 0;
}


int Player::Draw()
{
    GameObject2D *player = GetMyObject2D();
	if (PlayerNum == 1)// プレイヤー1の場合は赤色で描画
        DrawBox(player->GetPosition().GetX(), player->GetPosition().GetY(), player->GetPosition().GetX() + 30, player->GetPosition().GetY() + 60, 0xFF0000, true);
	else// プレイヤー2の場合は青色で描画
        DrawBox(player->GetPosition().GetX(), player->GetPosition().GetY(), player->GetPosition().GetX() + 30, player->GetPosition().GetY() + 60, 0x0000FF, true);
    return 0;
}

// 
int Player::HoldObject(BaseCollision *hitCollision)
{
    // @Debug
    // アクションボタン
    // 現在はTEST_1だけど、後々は違うボタンに
    if (!Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::TEST_1, PlayerNum - 1))
    {
        // 何もしない
        return 0;
    }

    // 当たったオブジェクトが普通のオブジェクト、ライト、鏡のいずれでもないなら処理を行う
    if ((hitCollision->GetCollisionTag() == CollisionTag::CollisionTag_Wall ||
        hitCollision->GetCollisionTag() == CollisionTag::CollisionTag_LightBody ||
        hitCollision->GetCollisionTag() == CollisionTag::CollisionTag_Mirror) == false)
    {
        // 何もしない
        return 0;
    }

    // 既にオブジェクトをつかんでいる場合は処理を行う
    if (this->mpHold != nullptr)
    {
        // 掴みっぱなしなので
        return 0;
    }

    // 
    BaseScene *nowScene = Master::mpBaseSceneManager->SearchSceneNow();

    // 
    nowScene->GetBaseCollision2DManager();

    // 
    hitCollision;
}

// 
int Player::HoldMove()
{
    // 


    // 
    return 0;
}

// 
bool Player::CheckHoldObject(const HoldObjectController *hold) const
{
    // 
    return this->mpHold == hold;
}

// 
bool Player::CheckHoldNow() const
{
    // 
    return this->mpHold != nullptr;
}