#pragma once
#include "MapObjectData.h"
#include "Vector2.h"

#include "TestObjectBase.h"


class ShineObject : public TestObjectBase
{
private:
	// Œõ‚Ì•ûŒü
	SHINE_DIRECTION mstShineDirection;

	// ƒfƒOƒŠ[
	float mfAngle;

	float mfVisionAngle;

public:
    ShineObject();
    ~ShineObject();

	void Init() override;
	void Finalize() override;

	void Update() override;

	void Draw() override;

	SHINE_DIRECTION GetLineDirection() { return mstShineDirection; }
};