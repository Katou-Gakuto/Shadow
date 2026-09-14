#pragma comment(lib, "winmm.lib")

#include "DxLib.h"

#include "E_Scene/BaseSceneManager.h"
#include "Z_Except/Master.h"
#include "EndManager.h"
#include "KeyState.h"
#include "TimeManager.h"

#ifdef _DEBUG
#include "DebugLogs/DebugLog.h"
#endif

/*
【フォルダの規則】

「どのフォルダにファイルを入れようか……」
という悩みがあると思います。

そういう場合は『どのような役割を持っているか』でフォルダの入れる先を決めてください。
なお、ヘッダファイルとソースファイルでフォルダ分けはしない予定です。

あとフォルダ名の頭についているアルファベットはソートの順番を指定したいだけなので気にしないでください。

【】



*/

/// <summary>
/// メイン
/// </summary>
/// <param name="hInstance">HINSTANCE</param>
/// <param name="hPrevInstance">HINSTANCE</param>
/// <param name="lpCmdLine">LPSTR</param>
/// <param name="nCmdShow">int</param>
/// <returns>int</returns>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG

	// デバッグ初期化
	DEBUG::DebugInitialization(/*/true/*/false/**/);

	// デバッグテキストの出力先を新しいファイルにする
	{
		DEBUG::DebugCreateLogFileName();
		DEBUG::DebugLogAddData(DEBUG::DEBUG_PROCESS_TYPE::FUNCTION_CALL);
		DEBUG::DebugLogAddData(DEBUG::DEBUG_PROCESS_TYPE::TIME);

		DEBUG::DebugCreateLogFileName(DEBUG::DEBUG_MAP_TYPE::DEBUG_SCENE_TITLE, { "_sceneTitle" });
		DEBUG::DebugLogAddData(DEBUG::DEBUG_PROCESS_TYPE::ALL_FILE_OUTPUT, DEBUG::DEBUG_MAP_TYPE::DEBUG_SCENE_TITLE);

		/**/
		DEBUG::DebugCreateLogFileName(DEBUG::DEBUG_MAP_TYPE::DEBUG_TIME, { "_Time" });
		DEBUG::DebugLogAddData(DEBUG::DEBUG_PROCESS_TYPE::TIME, DEBUG::DEBUG_MAP_TYPE::DEBUG_TIME);//*/

		/**/
		DEBUG::DebugCreateLogFileName(DEBUG::DEBUG_MAP_TYPE::DEBUG_ARRIVAL, { "_Arrival" });
		DEBUG::DebugLogAddData(DEBUG::DEBUG_PROCESS_TYPE::FUNCTION_CALL, DEBUG::DEBUG_MAP_TYPE::DEBUG_ARRIVAL);//*/
	}
#endif
	// DXライブラリ前初期化
	{
		// ウインドウモードで起動
		ChangeWindowMode(true);

		// 画面サイズ
		SetGraphMode(1280, 960, 32);
	}
	
	// DXライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;		// エラーが起きたら直ちに終了
	}

#ifndef _DEBUG
	//SetUseDirect3DVersion(DX_DIRECT3D_9EX);

	// log.txtを生成しない
	SetOutApplicationLogValidFlag(FALSE);
#endif

	//// ウインドウモードで起動
	//ChangeWindowMode(true);

	//// 画面サイズ
	//SetGraphMode(1280, 960, 32);

	// DXライブラリ後
	Master::Initialize();

	/*
	* ゲームループ
	* [
	* 更新判定
	*     [
	*     ]
	* 
	* 更新
	*     [
	*		キーの更新
	*		オブジェクトの更新
	*     ]
	* 
	* 描画
	*     [
	*		オブジェクトの描画
	*     ]
	* ]
	*/

	while (Master::mpEndManager->EndFlag() == false)
	{
		// ↓約17ms経ってるか、とかメッセージの処理とか諸々が終わったよってところから
		if (Master::mpTimeManager->GetNextUpdateFlag())
		{
			// コントローラーのキーの状態を更新
			Master::mpKeyState->Update();

			// 
			Master::mpBaseSceneManager->Update();

			ClearDrawScreen();

			// 
			Master::mpBaseSceneManager->Draw();

			ScreenFlip();
		}
	}

	Master::Finalize();

	// DXライブラリ使用の終了処理
	DxLib::DxLib_End();
	// ソフトの終了
	return 0;
}
