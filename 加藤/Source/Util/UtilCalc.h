#pragma once
#include <iostream>
#include <limits>
#include "DxLib.h"

namespace UtilCalc
{
	/*int最大値*/
	constexpr int IntMax = (std::numeric_limits<int>::max)();

	/*ゼロ*/
	inline VECTOR VZero() { return VGet(0.0f, 0.0f, 0.0f); }
	/*イチ*/
	inline VECTOR VOne() { return VGet(1.0f, 1.0f, 1.0f); }
	/*xのみ1*/
	inline VECTOR VXOne() { return VGet(1.0f, 0.0f, 0.0f); }
	/*三分の一*/
	constexpr float One_Third = 0.3333333f;
	/*3,14*/
	constexpr float Pi = DX_PI_F;
	/*3,14 * 2.0f*/
	constexpr float PiTwo = DX_PI_F * 2.0f;

	/*フロート誤差*/
	constexpr float FloatError = 0.00000001f;

	/*ラジアン変換*/
	constexpr float DegToRad = 0.0174532924f;
	/*デグリー変換*/
	constexpr float RadToDeg = 57.29578f;
	/*デグリー半径*/
	constexpr float RadPi = 180.0f;
	/*デグリー直径*/
	constexpr float RadPiTwo = 360.0f;

	/*キャラクター1.0f割合サイズ*/
	static float CharacterOneSize = 180.0f;

	/// <summary>float同士がの誤差範囲内なら「true」を返す</summary>
	bool FloatEqual(float src, float dst, float error = FloatError);

	/*符号反転*/
	inline const VECTOR VSignInversion(VECTOR src) { return VGet(src.x, src.y, src.z); }

	/*ラジアン変換(PI / 180.0f)*/
	inline const VECTOR VRadChange(VECTOR src) { return VScale(src, DegToRad/*PI / 180.0f*/); }
	/*デグリー変換(180.0f / PI)*/
	inline const VECTOR VDegChange(VECTOR src) { return VScale(src, RadToDeg/*180.0f / PI*/); }

    /*方向を3.14から-3.14になるように調整する(ラジアン)*/
	float NotExceedAngle(float angle);

    /*方向を3.14から-3.14になるように調整する(3方向)(ラジアン)*/
	VECTOR VNotExceedAngle(VECTOR angle, bool radFlag = false);

	/*球面移動した場所(ラジアン)*/
	VECTOR VSphericalMovePos(float size, VECTOR angle);

	// TODO: 未完 Yのみ完成
	/// <summary>ベクトルをアングルに変換する</summary>
	VECTOR VVecToAngle(VECTOR vec);

	// TODO: 未完
	/// <summary>アングルをベクトルに変換する</summary>
	VECTOR VAngleToVec(VECTOR angle);

	/// <summary>移動量をアングルに反映した値を返す(ラジアン)</summary>
	VECTOR VMoveVecToAngle(VECTOR moveVec, VECTOR angle, float speed = 0.2f);

	/// <summary>VECTORの平均を出す(正の数)</summary>
	float PutVectorAverage(VECTOR vector);

	/// <summary>VECTORの掛け算</summary>
	VECTOR VMultiply(VECTOR src, VECTOR dst);

	/// <summary>差を出す</summary>
	float VDiff(VECTOR src, VECTOR dst);

	/// <summary>アングル同士の差を返す</summary>
	float AngleDiff(float srcAngle, float dstAngle);

	/*--------------*/
	/*【当たり判定】*/
	/*--------------*/

	/*球同士の当たり判定*/
	bool SphereCollision(VECTOR srcPos, float srcRadius, VECTOR dstPos, float dstRadius);

	///*四角同士の当たり判定*/
	//bool BoxCollision(VECTOR srcTopLeft, VECTOR srcBottomRight, VECTOR dstTopLeft, VECTOR dstBottomRight);
}