#include "BitFlag.h"

#include "DxLib.h"

#include "Master.h"

#include "EndManager.h"
#include "LoadingManager.h"

LoadingManager::LoadingManager()
: mullLoadingFlag(BIT_FLAG<unsigned long long>())
{
}
LoadingManager::~LoadingManager()
{
}

//ローディング
void LoadingManager::Loading()
{

	// シーン
	if (mullLoadingFlag.GetFlag_BitShift(LOADING_NUMBER::SCENE) && !Master::mpEndManager->GetBitEndflag().Bool())
	{
		// TODO: エフェクトがバグる感じがある(他にミスがないかを確認しろ)
		//SetUseASyncLoadFlag(TRUE);

		//Master::mpGameManager->GetSceneManager()->NextScene();

		while ((GetASyncLoadNum() != 0) && !Master::mpEndManager->EndFlag())
		{
			// TODO: シーン読み込み中描画
		}
		if (GetASyncLoadNum() != 0)
		{
			Master::mpEndManager->SetEndFlag(true, END_FLAG_NUMBER::LOADING_FLAG);
		}

		SetUseASyncLoadFlag(FALSE);

		//Master::mpFadeManager->FadeIn();
		mullLoadingFlag.DisableFlag(LOADING_NUMBER::SCENE);
	}
}