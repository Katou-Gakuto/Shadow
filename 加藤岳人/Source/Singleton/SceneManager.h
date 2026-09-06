#pragma once

#include "SceneEnum.h"

#include "FSMScene.h"
#include "StateScene.h"

class SceneManager
{
private:
	// 有限状態マシン
	FSMScene* mpFSMScene;
	// 次のシーン
	SCENE meNextScene;

	// 前のシーン
	SCENE mePreScene;

public:
	SceneManager();
	~SceneManager();

	/*初期化*/
	void Initilize();
	/*更新*/
	void Update();
	/*次のシーンへ移動する*/
	void NextScene();

	/*--------------------------------------------------
	* 【設定】
	*/
	/*次のシーンを設定*/
	void SetNextScene(SCENE nextScene);

	/*前のシーン設定*/
	inline void SetPreScene(SCENE preScene) { mePreScene = preScene; }
	
	/*--------------------------------------------------
	* 【取得】
	*/
	/// <summary>今のシーン取得</summary>
	inline SCENE GetNowScene() const { return mpFSMScene->GetCurrentState(); }
	/// <summary>次のシーン取得</summary>
	inline SCENE GetNextScene() const { return meNextScene; }
	/// <summary>前のシーン取得</summary>
	inline SCENE GetPreScene() const { return mePreScene; }

	/*有限状態マシン取得*/
	inline FSMScene* GetFSMScene() const { return mpFSMScene; }

	/*シーンカメラID取得*/
	inline int GetSceneCameraID() const { return mpFSMScene->GetSceneCameraID(); }
};