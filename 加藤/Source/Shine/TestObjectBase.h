#pragma once
#include "VECTOR.h"
#include "ShineManager.h"

class TestObjectBase
{
private:
	// ポジション
	VECTOR2D mv2Position;

	// サイズ
	VECTOR2D mv2Size;

	ShineManager* mpShineManager;
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
};