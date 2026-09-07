#include <map>

#include "CameraManager.h"
#include "FSMCamera.h"

CameraManager::CameraManager()
{
	DataInit();

	mpFsm = new FSMCamera();
}

CameraManager::~CameraManager()
{
	mmCameraDatas.clear();

	delete mpFsm;
}

// データ初期化
void CameraManager::DataInit()
{
	mmCameraDatas.clear();
	mnNowID = -1;
	mnMaxID = 0;

	mbNotSetCameraFlag = true;
}

// 更新
void CameraManager::Update()
{
	if (mnNowID != -1)
	{
		mpFsm->Update(this);
	}
}

// 描画
void CameraManager::Draw()
{
	if (mnNowID != -1)
	{
		mpFsm->Draw(this);
	}
}

// 新しいカメラを作成
int CameraManager::NewCamera(CameraData cameraData)
{
	cameraData.cameraID = mnMaxID;
	++mnMaxID;
	mmCameraDatas[cameraData.cameraID] = cameraData;
	mpFsm->Initilize(this, cameraData.cameraID);
	return cameraData.cameraID;// カメラのIDを返す
}

// カメラ削除
void CameraManager::DeleteCameraData(int cameraID)
{
	std::map<int, CameraData>::iterator deleteData = mmCameraDatas.find(cameraID);
	if (deleteData != mmCameraDatas.end())
	{
		mmCameraDatas.erase(deleteData);
	}

	// 現在の視点を消したなら適当な視点を見る
	if (mnNowID == cameraID)
	{
		if (mmCameraDatas.size() > 0)
		{
			mbNotSetCameraFlag = true;

			mnNowID = (*mmCameraDatas.begin()).first;
		}
		else
		{
			DataInit();
		}
	}
}

// カメラモード設定
void CameraManager::SetCameraMode(int cameraID)
{
	if (mnNowID != cameraID)
	{
		mbNotSetCameraFlag = false;

		int preID = mnNowID;
		mnNowID = cameraID;
		mpFsm->SetCurrentState(mmCameraDatas[mnNowID].cameraMode, &mmCameraDatas[mnNowID], &mmCameraDatas[preID]);
	}
}