#pragma once
#include "TestObjectBase.h"

class WallObject : public TestObjectBase
{
private:

public:
    WallObject();
    ~WallObject();

	void Init() override;
	void Finalize() override;

	void Update() override;

	void Draw() override;
};