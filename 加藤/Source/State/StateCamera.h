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
};