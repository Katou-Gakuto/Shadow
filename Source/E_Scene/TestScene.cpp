#include "TestScene.h"
#include "PlayerObject.h"

#include "GameObject2D.h"
#include "GameObjectManager.h"

#include "BaseCollision2DManager.h"

TestScene::TestScene()
: BaseScene(SceneTag::ST_Test)
{
}

TestScene::~TestScene()
{
}

int TestScene::Create()
{
	this->mpGameObjectManager = new GameObjectManager();
	this->mpBaseCollision2DManager = new BaseCollision2DManager();

	GameObject2D* currentObject = nullptr;

	currentObject = new PlayerObject();
	this->mpGameObjectManager->Add(currentObject);

	this->mpGameObjectManager->Create();
	return 0;
}

int TestScene::Initialize()
{
	this->mpGameObjectManager->Initialize();
	return 0;
}

int TestScene::Finalize()
{
	this->mpGameObjectManager->Finalize();
	return 0;
}

int TestScene::Update()
{
	// ここでMoveVecを0にしておく
	this->mpGameObjectManager->ResetMoveVec();

	// 先にやりたい処理
	this->mpGameObjectManager->EarlyUpdate();

	// 通常処理
	this->mpGameObjectManager->Update();

	// 当たり判定の計算 + スライド移動 + 当たり判定のイベント通知
	this->mpBaseCollision2DManager->CheckHitAllMove();

	// ゲームオブジェクトの座標を更新
	this->mpGameObjectManager->SetPositionToMoveVec();

	// 当たり判定の座標を更新
	this->mpGameObjectManager->SetCollisionPosToCollisionMoveVec();

	// 座標更新後の処理
	this->mpGameObjectManager->LateUpdate();

	// モデルの更新処理
	this->mpGameObjectManager->UpdateModel();

	// 成功を返す
	return 0;
}

int TestScene::Draw()
{
	this->mpGameObjectManager->Draw();
	return 0;
}
