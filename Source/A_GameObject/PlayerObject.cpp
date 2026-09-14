#include "PlayerObject.h"
#include "Player.h"

PlayerObject::PlayerObject(int playerNumber)
: GameObject2D(GameObjectTag::GOT_Player)
, mnPlayerNumber(playerNumber)
{
}

PlayerObject::~PlayerObject()
{
}

int PlayerObject::Create()
{
	this->CreateObjectComponentList();
	this->CreateObjectCollisionList();
	this->AddComponent(new Player(this, mnPlayerNumber));

	return 0;
}