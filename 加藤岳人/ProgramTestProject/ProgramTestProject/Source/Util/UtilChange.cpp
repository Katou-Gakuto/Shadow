#include "Vector2.h"

#include "DxLib.h"

#include "UtilChange.h"

/// <summary>ベクトルをベクトル2に変換(xy)</summary>
Vector2 UtilChange::ChangeVector_XY(VECTOR src)
{
	return Vector2(src.x, src.y);
}


/// <summary>ベクトルをベクトル2に変換(xz)</summary>
Vector2 UtilChange::ChangeVector_XZ(VECTOR src)
{
	return Vector2(src.x, src.z);
}

/// <summary>ベクトルをベクトル2に変換(yz)</summary>
Vector2 UtilChange::ChangeVector_YZ(VECTOR src)
{
	return Vector2(src.y, src.z);
}