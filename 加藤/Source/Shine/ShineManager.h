#pragma once
#include <vector>

#include "Vector2.h"

class ShineObject;
class TestObjectBase;

struct SHINE_TRIANGLE
{
	Vector2 Vertex1;
	Vector2 Vertex2;
	Vector2 Vertex3;
};

class ShineManager
{
private:
	std::vector<TestObjectBase*> mpObjects;

	std::vector<SHINE_TRIANGLE> mstSheineTriangles;

	ShineObject* mpShineObject;
public:
	ShineManager();
	~ShineManager();

	void Init();

	void Finalize();

	void Update();

	void Draw();
private:
	// Œõ—Ìˆæ‚Ìì¬
	void CreateLightArea();
};