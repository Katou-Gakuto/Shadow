#include "Master.h"

#include "DrawManager.h"
#include "FadeManager.h"
#include "FSMScene.h"
#include "GameManager.h"
#include "ObjectManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "UtilFactorys.h"
#include "UtilChange.h"

#ifdef _DEBUG
#include "DebugLogs/DebugLog.h"
#endif

SceneManager::SceneManager()
: mpFSMScene(nullptr)
, meNextScene(SCENE::START)
, mePreScene(SCENE::START)
{

}
SceneManager::~SceneManager()
{
	delete mpFSMScene;
}

// 初期化
void SceneManager::Initilize()
{
	mpFSMScene = UtilFactorys::FSMSceneFactory(this);
}

// 更新
void SceneManager::Update()
{
	mpFSMScene->Update(this);
}

// 次のシーンへ移動する
void SceneManager::NextScene()
{
	Master::mpTimeManager->SetNewSceneTimeFlag(true);

	mpFSMScene->GetStateMap()[UtilChange::SceneState(mePreScene)]->OnExit(this, UtilChange::SceneState(meNextScene));
	Master::mpGameManager->GetObjectManager()->DeleteSetScene(mePreScene);

	// 描画情報削除
	Master::mpDrawManager->DeleteDrawData();

	mpFSMScene->NextScene(this);

	// オブジェクト最終更新
	Master::mpGameManager->GetObjectManager()->ObjectSceneLastInitilize();


#ifdef _DEBUG
	DEBUG::SaveText("\nNEXT SCENE : " + UtilChange::SceneTypeToString(GetNowScene()) + "\n\n", DEBUG::DEBUG_MAP_TYPE::DEBUG_SCENE_TITLE);
#endif
}

// 次のシーンを設定
void SceneManager::SetNextScene(SCENE nextScene)
{
	Master::mpFadeManager->FadeOut();
	meNextScene = nextScene;
}