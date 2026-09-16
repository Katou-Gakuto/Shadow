#pragma once
#include "Vector2.h"
#include "ShineManager.h"

class TestObjectBase
{
protected:
	// ポジション
	Vector2 mv2Position;

	// サイズ
	Vector2 mv2Size;

	ShineManager* mpShineManager;
public:
	TestObjectBase()
	: mv2Position(Vector2(1.0f, 1.0f))
	, mv2Size(Vector2(10.0f, 10.0f))
	{
	}

	virtual void Init() = 0;
	virtual void Finalize() = 0;
	
	virtual void Update() = 0;

	virtual void Draw() = 0;

    Vector2 GetPosition() const { return mv2Position; }
	void SetPosition(const Vector2 position) { mv2Position = position; }
    Vector2 GetSize() const { return mv2Size; }
	void SetSize(const Vector2 size) { mv2Size = size; }
};