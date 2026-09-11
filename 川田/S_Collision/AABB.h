#pragma once

#include "../Y_Tool/VECTOR.h"

// カリング処理に使用する値
struct AABB2D
{
    VECTOR2D mvMinPos;
    VECTOR2D mvMaxPos;
    VECTOR2D mvMoveVec;
};

// カリング処理に使用する値
struct AABB3D
{
    VECTOR3D mvMinPos;
    VECTOR3D mvMaxPos;
    VECTOR3D mvMoveVec;
};
