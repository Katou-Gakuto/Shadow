#include "CameraData.h"

#include "DxLib.h"

#include "Master.h"

#include "CameraManager.h"
#include "ResourceManager.h"
#include "ResourceSound.h"
#include "StateCameraBase.h"

/*------------------------*/
/*【カメラステートベース】*/
/*------------------------*/
// カメラの共通設定をする
void IStateCamera::CommonSetCamera(CameraData *cameraData)
{
	// 背景の色を設定する
	SetBackgroundColor(cameraData->red, cameraData->green, cameraData->blue, cameraData->alpha);

	// Zバファに書き込む準備
	SetUseZBufferFlag(FALSE);
	SetWriteZBufferFlag(FALSE);
}