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

// 描画
void StateFixedCamera::Draw(CameraData* cameraData)
{
}


/*--------------------*/
/*【移動視点ステート】*/
/*--------------------*/
StateMoveCamera::StateMoveCamera()
: IStateCamera({}, CAMERA_MODE::MOVE)
{
	mStateNumber = CAMERA_MODE::MOVE;
}

// この状態に入った時の処理
void StateMoveCamera::OnEnter(CameraData* cameraData, CAMERA_MODE preMode)
{
	CommonSetCamera(cameraData);

	cameraData->prePosition = VSub(cameraData->position, cameraData->moveDistance);

	Master::mpGameManager->GetCameraManager()->SetCameraData(*cameraData);
}

// この状態を出る時の処理
void StateMoveCamera::OnExit(CameraData* cameraData, CAMERA_MODE newMode)
{
}

// 初期化
void StateMoveCamera::Initilize(CameraData* cameraData)
{
}

// 更新
void StateMoveCamera::Update(CameraData* cameraData)
{
	if (!cameraData->processFlag)
	{
		return;
	}

	if (UtilCalc::VDiff(cameraData->prePosition, cameraData->targetPosition) >= UtilCalc::VDiff(cameraData->position, cameraData->targetPosition))
	{
		cameraData->prePosition = cameraData->position;
		cameraData->position = VAdd(cameraData->position, cameraData->moveDistance);
		
		if (UtilCalc::VDiff(cameraData->prePosition, cameraData->targetPosition) < UtilCalc::VDiff(cameraData->position, cameraData->targetPosition))
		{
			cameraData->processFlag = false;
			cameraData->position = VSub(cameraData->position, cameraData->moveDistance);
		}
	}

	Master::mpGameManager->GetCameraManager()->SetCameraData(*cameraData);
}

// 描画
void StateMoveCamera::Draw(CameraData* cameraData)
{
	// カメラ位置を反映する
	SetCameraPos(cameraData->position, VAdd(cameraData->position, cameraData->plusPosition));
}

/*----------------------------*/
/*【キャラクター視点ステート】*/
/*----------------------------*/
StateCharacterCamera::StateCharacterCamera()
: IStateCamera({}, CAMERA_MODE::CHARACTER)
{
	mStateNumber = CAMERA_MODE::CHARACTER;
}

// この状態に入った時の処理
void StateCharacterCamera::OnEnter(CameraData* cameraData, CAMERA_MODE preMode)
{
	CommonSetCamera(cameraData);
}

// この状態を出る時の処理
void StateCharacterCamera::OnExit(CameraData* cameraData, CAMERA_MODE newMode)
{
}

// 初期化
void StateCharacterCamera::Initilize(CameraData* cameraData)
{

}

// 更新
void StateCharacterCamera::Update(CameraData* cameraData)
{
}

// 描画
void StateCharacterCamera::Draw(CameraData* cameraData)
{
	// カメラ位置を反映する
	SetCameraPos(
		VAdd(VAdd(VScale(UtilCalc::VSignInversion(cameraData->targetCharacter->GetVec()), cameraData->cameraDistance), cameraData->targetCharacter->GetPos()), cameraData->plusPosition),
		VAdd(cameraData->targetCharacter->GetPos(), cameraData->plusPosition)
	);
}


/*--------------------------*/
/*【プレイヤー視点ステート】*/
/*--------------------------*/
StatePlayerCamera::StatePlayerCamera()
: IStateCamera({}, CAMERA_MODE::PLAYER)
{
	mStateNumber = CAMERA_MODE::PLAYER;
}

// この状態に入った時の処理
void StatePlayerCamera::OnEnter(CameraData* cameraData, CAMERA_MODE preMode)
{
	CommonSetCamera(cameraData);
}

// この状態を出る時の処理
void StatePlayerCamera::OnExit(CameraData* cameraData, CAMERA_MODE newMode)
{
}

// 初期化
void StatePlayerCamera::Initilize(CameraData* cameraData)
{
}

// 更新
void StatePlayerCamera::Update(CameraData* cameraData)
{
	if (!cameraData->processFlag)
	{
		return;
	}

	if (Master::mpStopManager->GetStopFlag(STOP_FLAG_TYPE::GAME_OBJECT))
	{
		// カメラの設置だけやる
		if (Master::mpStopManager->GetStopFlag(STOP_FLAG_TYPE::NODE))
		{
			cameraData->position = UtilCalc::VSphericalMovePos(cameraData->cameraDistance, UtilCalc::VRadChange(cameraData->angle));
			Master::mpGameManager->GetCameraManager()->SetCameraData(*cameraData);
		}

		return;
	}

	KeyState* keyState = Master::mpKeyState;

	// 左回転
	if (keyState->GetWordKey_Board(KEY_BOARD_WORD::ARROW_LEFT))
	{
		cameraData->angle.y += CAMERA_ONE_FRAME_AMOUNT;
		if (cameraData->angle.y >= UtilCalc::RadPi)
		{
			cameraData->angle.y -= UtilCalc::RadPiTwo;
		}

	}
	// 右回転
	if (keyState->GetWordKey_Board(KEY_BOARD_WORD::ARROW_RIGHT))
	{
		cameraData->angle.y -= CAMERA_ONE_FRAME_AMOUNT;
		if (cameraData->angle.y <= (-UtilCalc::RadPi))
		{
			cameraData->angle.y += UtilCalc::RadPiTwo;
		}
	}

	// 上回転
	if (keyState->GetWordKey_Board(KEY_BOARD_WORD::ARROW_UP))
	{
		cameraData->angle.x += CAMERA_ONE_FRAME_AMOUNT;
		if (cameraData->angle.x >= UP_MAX_ANGLE)
		{
			cameraData->angle.x = UP_MAX_ANGLE;
		}
	}
	// 下回転
	if (keyState->GetWordKey_Board(KEY_BOARD_WORD::ARROW_DOWN))
	{
		cameraData->angle.x -= CAMERA_ONE_FRAME_AMOUNT;
		if (cameraData->angle.x <= DOWN_MAX_ANGLE)
		{
			cameraData->angle.x = DOWN_MAX_ANGLE;
		}
	}

	cameraData->position = UtilCalc::VSphericalMovePos(cameraData->cameraDistance, UtilCalc::VRadChange(cameraData->angle));

	Master::mpGameManager->GetCameraManager()->SetCameraData(*cameraData);
}

// 描画
void StatePlayerCamera::Draw(CameraData* cameraData)
{
	// カメラ位置を反映する
	SetCameraPos(
		VAdd(VAdd(cameraData->position, cameraData->targetCharacter->GetPos()), cameraData->plusPosition),
		VAdd(cameraData->targetCharacter->GetPos(), cameraData->plusPosition)
	);
}