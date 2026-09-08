#include "cameraData.h"

#include "Master.h"

#include "CameraManager.h"
#include "GameManager.h"
#include "KeyState.h"
#include "StateCameraBase.h"
#include "StateCamera.h"
#include "StopManager.h"
#include "UtilCalc.h"


/*--------------------*/
/*【固定視点ステート】*/
/*--------------------*/
StateFixedCamera::StateFixedCamera()
: IStateCamera({}, CAMERA_MODE::FIXED)
{
	mStateNumber = CAMERA_MODE::FIXED;
}

// この状態に入った時の処理
void StateFixedCamera::OnEnter(CameraData* cameraData, CAMERA_MODE preMode)
{
	CommonSetCamera(cameraData);
}

// この状態を出る時の処理
void StateFixedCamera::OnExit(CameraData* cameraData, CAMERA_MODE newMode)
{
}

// 初期化
void StateFixedCamera::Initilize(CameraData* cameraData)
{
}

// 更新
void StateFixedCamera::Update(CameraData* cameraData)
{
}