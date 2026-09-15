#pragma once
#include "Vector2.h"
#include "ShineManager.h"

class TestObjectBase
{
private:
	// ポジション
	Vector2 mv2Position;

	// サイズ
	Vector2 mv2Size;

	ShineManager* mpShineManager;
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
};