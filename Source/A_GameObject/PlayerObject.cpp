#include "PlayerObject.h"
#include "Player.h"

PlayerObject::PlayerObject()
: GameObject2D(GameObjectTag::GOT_Player)
{
}

PlayerObject::~PlayerObject()
{
}

int PlayerObject::Create()
{
	this->CreateObjectComponentList();
	this->CreateObjectCollisionList();
	this->AddComponent(new Player(this));

	return 0;
}