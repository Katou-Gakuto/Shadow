#include "CameraData.h"

#include "UtilCalc.h"

CameraData::CameraData()
{
	targetPosition = UtilCalc::VZero();
	position = UtilCalc::VZero();
	plusPosition = UtilCalc::VZero();

	moveDistance = UtilCalc::VZero();
	prePosition = UtilCalc::VZero();

	twoDPosition = UtilCalc::VZero();

	cameraMode = CAMERA_MODE::NONE;

	red = 0;
	green = 0;
	blue = 0;
	alpha = 0;

	cameraID = -1;
}