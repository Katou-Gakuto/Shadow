#pragma once
#include "DxLib.h"

struct Vector2;

namespace UtilChange
{
	/// <summary>ベクトルをベクトル2に変換(xy)</summary>
	Vector2 ChangeVector_XY(VECTOR src);
	/// <summary>ベクトルをベクトル2に変換(xz)</summary>
	Vector2 ChangeVector_XZ(VECTOR src);
	/// <summary>ベクトルをベクトル2に変換(yz)</summary>
	Vector2 ChangeVector_YZ(VECTOR src);
}