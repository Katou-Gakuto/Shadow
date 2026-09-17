#pragma once
#include <vector>

#include "Vector2.h"

struct LINE_POS
{
	Vector2 linePos1;
	Vector2 linePos2;
};

// マップオブジェクト情報
struct MAP_OBJECT_DATA
{
public:
	std::vector<LINE_POS> LinePoss;

	bool LitFlag;
};

// 視界を遮る情報
struct BLOCK_POS_DATA
{
public:
	Vector2_Int BlockPos;
	int ArrayIndex;
};

// 視界
struct SHINE_DIRECTION
{
	Vector2 shineDirection1;
	Vector2 shineDirection2;
};