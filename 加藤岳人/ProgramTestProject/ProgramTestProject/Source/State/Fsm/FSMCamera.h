#pragma once

#include "FSM.h"
#include "StateCameraBase.h"

/*----------*/
/*【カメラ有限状態マシン】*/
/*----------*/
class FSMCamera : public FSMBase<IStateCamera, CAMERA_MODE, CameraData>
{
public:
	FSMCamera();

	// /*実行中状態をセットする*/
	// void SetCurrentState(CameraManager* cameraManager, int& preThreeDFlag);

	/*初期化*/
	void Initilize(CameraManager* cameraManager, int id);

	/*更新*/
	void Update(CameraManager* cameraManager);

	/*描画*/
	void Draw(CameraManager* cameraManager);
};