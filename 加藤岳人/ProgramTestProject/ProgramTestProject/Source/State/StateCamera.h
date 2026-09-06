#pragma once
#include "CameraData.h"

#include "StateCameraBase.h"

/*--------------------*/
/*【固定視点ステート】*/
/*--------------------*/
class StateFixedCamera : public IStateCamera
{
public:
	// FIXME: コンストラクタでステート変更条件を渡せます
	StateFixedCamera();

	/*この状態に入った時の処理*/
	void OnEnter(CameraData* cameraData, CAMERA_MODE preMode) override;
	/*この状態を出る時の処理*/
	void OnExit(CameraData* cameraData, CAMERA_MODE newMode) override;

	/*初期化*/
	void Initilize(CameraData* cameraData) override;

	/*更新*/
	void Update(CameraData* cameraData) override;

	/*描画*/
	void Draw(CameraData* cameraData) override;
};

/*--------------------*/
/*【移動視点ステート】*/
/*--------------------*/
class StateMoveCamera : public IStateCamera
{
public:
	// FIXME: コンストラクタでステート変更条件を渡せます
	StateMoveCamera();

	/*この状態に入った時の処理*/
	void OnEnter(CameraData* cameraData, CAMERA_MODE preMode) override;
	/*この状態を出る時の処理*/
	void OnExit(CameraData* cameraData, CAMERA_MODE newMode) override;

	/*初期化*/
	void Initilize(CameraData* cameraData) override;

	/*更新*/
	void Update(CameraData* cameraData) override;

	/*描画*/
	void Draw(CameraData* cameraData) override;
};

/*----------------------------*/
/*【キャラクター視点ステート】*/
/*----------------------------*/
class StateCharacterCamera : public IStateCamera
{
public:
	// FIXME: コンストラクタでステート変更条件を渡せます
	StateCharacterCamera();

	/*この状態に入った時の処理*/
	void OnEnter(CameraData* cameraData, CAMERA_MODE preMode) override;
	/*この状態を出る時の処理*/
	void OnExit(CameraData* cameraData, CAMERA_MODE newMode) override;

	/*初期化*/
	void Initilize(CameraData* cameraData) override;

	/*更新*/
	void Update(CameraData* cameraData) override;

	/*描画*/
	void Draw(CameraData* cameraData) override;
};

/*--------------------------*/
/*【プレイヤー視点ステート】*/
/*--------------------------*/
class StatePlayerCamera : public IStateCamera
{
private:
	// 上方向最大アングル
	const float UP_MAX_ANGLE = 70.0f;
	// 下方向最大アングル
	const float DOWN_MAX_ANGLE = 0.0f;

public:
	// FIXME: コンストラクタでステート変更条件を渡せます
	StatePlayerCamera();

	/*この状態に入った時の処理*/
	void OnEnter(CameraData* cameraData, CAMERA_MODE preMode) override;
	/*この状態を出る時の処理*/
	void OnExit(CameraData* cameraData, CAMERA_MODE newMode) override;

	/*初期化*/
	void Initilize(CameraData* cameraData) override;

	/*更新*/
	void Update(CameraData* cameraData) override;

	/*描画*/
	void Draw(CameraData* cameraData) override;
};