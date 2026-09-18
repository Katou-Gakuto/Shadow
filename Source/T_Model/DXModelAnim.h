#pragma once
#include "BaseModel.h"

class DXAnimModel : public BaseModel
{
	int Initialize();
	int Update();
	int Draw();
	int Finalize();

	VECTOR2D uvLeftUp;
	VECTOR2D uvRightDown;

};