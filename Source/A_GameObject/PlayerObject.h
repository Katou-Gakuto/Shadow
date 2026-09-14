#pragma once
#include "GameObject2D.h"

class PlayerObject : public GameObject2D
{
public:
	PlayerObject();
	~PlayerObject();

	int Create() override;
};