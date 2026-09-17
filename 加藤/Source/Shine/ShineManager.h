#pragma once
#include <stack>
#include <queue>
#include <vector>

#include "Vector2.h"

#include "MapObjectData.h"

class ShineObject;
class TestObjectBase;

struct SHINE_TRIANGLE
{
	Vector2_Int Vertex1;
	Vector2_Int Vertex2;
	Vector2_Int Vertex3;
};

enum SHINE_DRAW_MODE
{
	OBJECT_SHINE_DRAW_MODE = 0,
	TRIANGLE_SHINE_DRAW_MODE,
	GRID_SHINE_DRAW_MODE,
	SHINE_DRAW_MODE_MAX
};

enum class SHINE_GRID_TYPE
{
	NOT_SHINE_GRID = 0,				// 光領域外
	SHINE_GRID,						// 光領域内
	SHINE_AND_OBJECT_GRID,			// 光領域で尚且つオブジェクトがある
};

class ShineManager
{
private:
	static constexpr int MAP_SIZE_X = 1280;
	static constexpr int MAP_SIZE_Y = 960;

	static constexpr int MAP_ARRAY_SIZE_X = 10;
	static constexpr int MAP_ARRAY_SIZE_Y = 10;

	const int ONE_GRID_SIZE_X = MAP_SIZE_X / MAP_ARRAY_SIZE_X;
	const int ONE_GRID_SIZE_Y = MAP_SIZE_Y / MAP_ARRAY_SIZE_Y;

	std::vector<TestObjectBase*> mpObjects;

	std::vector<SHINE_TRIANGLE> mstSheineTriangles;

	// グリッド状にしたマップの当たり判定
	MAP_OBJECT_DATA mstMapObjectGridData[MAP_ARRAY_SIZE_Y][MAP_ARRAY_SIZE_X];

	ShineObject* mpShineObject;

	int mnDrawMode;
public:
	ShineManager();
	~ShineManager();

	void Init();

	void Finalize();

	void Update();

	void Draw();
private:
	// 光領域の作成
	void CreateLightArea();

	// グリッドの探索
    void CheckLightGrid(std::queue<Vector2_Int>& nextCheckShinePos);

    // グリッドの状態を判定
    SHINE_GRID_TYPE JudgeGrid(const Vector2_Int& gridPos, const std::vector<SHINE_DIRECTION>& shineDirections, int shineDirectionsIndex);

    // オブジェクトがあるグリッド内部の処理
    void InsideLightGridProcess(std::stack<BLOCK_POS_DATA>& blockPoss, std::queue<Vector2_Int>& nextCheckShinePos, std::vector<SHINE_DIRECTION>& shineDirections);

    // マップ外判定
    bool IsOutsideLightStage(const Vector2_Int& gridPos);

	// 光領域を左端から右端へ走査するグリッドを取得
	std::vector<Vector2_Int> GetLightGridPositions(const std::queue<Vector2_Int>& nowCheckShinePos);
};