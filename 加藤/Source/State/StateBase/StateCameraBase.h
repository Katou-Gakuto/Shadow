#pragma once
#include <vector>

#include "CameraData.h"

#include "StateBase.h"

enum class CAMERA_MODE;

class CameraManager;

/*------------------------*/
/*【カメラステートベース】*/
/*------------------------*/
class IStateCamera : public StateBase<CAMERA_MODE, CameraData>
{
protected:
	// カメラ1フレーム移動量
	const float CAMERA_ONE_FRAME_AMOUNT = 3.0f;

public:
	IStateCamera(std::vector<STATE_CHANGE_CRITERIA_DATA<CAMERA_MODE, CameraData>> stateChangeCriterias, CAMERA_MODE stateNumber)
	: StateBase(stateChangeCriterias, stateNumber)
	{
	}
	~IStateCamera() = default;

	/*更新*/
	virtual void Update(CameraData* cameraManager) = 0;

protected:
	/*カメラの共通設定をする*/
	void CommonSetCamera(CameraData *cameraData);
};