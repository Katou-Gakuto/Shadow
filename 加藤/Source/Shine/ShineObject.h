#pragma once
#include "Vector2.h"

#include "TestObjectBase.h"

class ShineObject : public TestObjectBase
{
private:

public:
    ShineObject();
    ~ShineObject();

	void Init() override;
	void Finalize() override;

	void Update() override;

	void Draw() override;
};