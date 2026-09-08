#include "CameraManager.h"
#include "FSM.h"
#include "FSMCamera.h"
#include "StateCameraBase.h"

/*------------------------*/
/*【カメラ有限状態マシン】*/
/*------------------------*/

FSMCamera::FSMCamera()
: FSMBase()
{
}

// // 実行中状態をセットする
// void FSMCamera::SetCurrentState(CameraManager* cameraManager, int& preThreeDFlag)
// {
// 	if (mCurrentState != CAMERA_MODE::NONE)
// 	{
// 		mmStateMap[mCurrentState]->OnExit(&cameraManager->GetCameraData());
// 	}

// 	mCurrentState = cameraManager->GetCameraData().cameraMode;
// 	mmStateMap[mCurrentState]->OnEnter(&cameraManager->GetCameraData());
// }

// 初期化
void FSMCamera::Initilize(CameraManager* cameraManager, int id)
{
	mmStateMap[cameraManager->GetCameraData(id).cameraMode]->Initilize(&cameraManager->GetCameraData());
}

// 更新
void FSMCamera::Update(CameraManager* cameraManager)
{
	mmStateMap[mCurrentState]->Update(&cameraManager->GetCameraData());
}