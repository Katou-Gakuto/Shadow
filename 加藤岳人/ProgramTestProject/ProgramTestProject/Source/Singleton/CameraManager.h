#pragma once
#include <map>

#include "CameraData.h"

class FSMCamera;

class CameraManager
{
private:
	// カメラ情報達
	std::map<int, CameraData> mmCameraDatas;

	// 現在のカメラID
	int mnNowID;
	// 最大カメラID
	int mnMaxID;

	// カメラが設定されていないフラグ
	bool mbNotSetCameraFlag;

	// カメラ有限状態マシン
	FSMCamera* mpFsm;
public:
	CameraManager();
	~CameraManager();

	/*データ初期化*/
	void DataInit();

	/*更新*/
	void Update();
	/*描画*/
	void Draw();

	/*新しいカメラを作成*/
	int NewCamera(CameraData cameraData);

	/*カメラ削除*/
	void DeleteCameraData(int cameraID);

	/*カメラ設定*/
	void SetCameraMode(int cameraID);

	/*カメラデータ取得*/
	inline CameraData& GetCameraData(int id = -1)
	{
		if (id != -1)
		{
			return mmCameraDatas[id];
		}
		if (mnNowID != -1)
		{
			return mmCameraDatas[mnNowID];
		}
		CameraData nullCameraData;
		return nullCameraData;
	}

	/*カメラデータ設定*/
	inline void SetCameraData(const CameraData& cameraData)
	{
		if (mnNowID != -1)
		{
			mmCameraDatas[mnNowID] = cameraData;
		}
	}

	/*カメラが設定されていないフラグ取得*/
	inline bool GetNotSetCameraFlag() const { return mbNotSetCameraFlag; }

	/*カメラの全データ取得*/
	inline std::map<int, CameraData> GetAllCameraData() { return mmCameraDatas; }
};