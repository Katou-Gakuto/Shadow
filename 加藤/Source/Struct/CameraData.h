#pragma once

#include "DxLib.h"

class CharacterBase;

enum class CAMERA_MODE
{
	NONE = -1,// ない

	FIXED = 0,	// 固定視点
	MOVE,	// 移動視点

	CHARACTER,	// キャラクター視点
	PLAYER,	// プレイヤー視点
};

struct CameraData
{
public:
	CameraData();

	VECTOR targetPosition;	// ターゲットポジション
	
	VECTOR position;		// カメラポジション
	VECTOR plusPosition;	// カメラプラスポジション
	VECTOR angle;	// カメラアングル

	VECTOR moveDistance;	// 移動量
	VECTOR prePosition;	// 前のポジション

	VECTOR twoDPosition;	// 2Dポジション

	CAMERA_MODE cameraMode;	// カメラモード

	int red;	// 赤
	int green;	// 緑
	int blue;	// 青
	int alpha;	// アルファ値

	int cameraID;	// カメラID

	bool processFlag = true;// 処理有効フラグ
};