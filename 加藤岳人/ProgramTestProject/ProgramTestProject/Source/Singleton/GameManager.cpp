#include "DxLib.h"

#include "Master.h"

#include "CameraManager.h"
#include "DotWeenManager.h"
#include "DrawManager.h"
#include "ImguiManager.h"
#include "KeyState.h"
#include "ResourceManager.h"
#include "StopManager.h"
#include "ThreadManager.h"
#include "TimeManager.h"

#ifdef _DEBUG
#include "DebugLogs/DebugLog.h"
#endif

LRESULT WINAPI GameManagerWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// コンストラクタ
GameManager::GameManager()
: mpAttackManager(nullptr)
, mpCameraManager(nullptr)
, mpCollisionManager(nullptr)
, mpDotWeenManager(nullptr)
, mpMapManager(nullptr)
, mpObjectManager(nullptr)
, mpSceneManager(nullptr)
, mpTargetManager(nullptr)
, mnUINumber(0)
, mbUninitializedFlag(true)
{
}
// デストラクタ
GameManager::~GameManager()
{
}

// DxLib_Init前初期化
void GameManager::DxLib_PreInit()
{

#ifndef _DEBUG
	//SetUseDirect3DVersion(DX_DIRECT3D_9EX);

	// log.txtを生成しない
	SetOutApplicationLogValidFlag(FALSE);
#endif

	// DirectX11を使用するようにする
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	SetEnableXAudioFlag(TRUE);

	// ウインドウモードで起動
	ChangeWindowMode(true);

    SetHookWinProc(GameManagerWndProc);

    // キーステート初期化(ウィンドウメッセージを渡してる)
    Master::mpKeyState->Initialize();


#ifndef _DEBUG
	// 画面サイズ
	SetGraphMode(1280, 960, 32);
#endif
}

// 初期化
void GameManager::Initilize()
{
    Master::mpDataManager->Initilize();

    Master::mpDrawManager->Initilize();

    mpCameraManager = new CameraManager();

    mpDotWeenManager = new DotWeenManager();
    mpDotWeenManager->Initilize();

    Master::mpResourceManager->Initilize();

    Master::mpFadeManager->Initilize();
    Master::mpStopManager->Initilize();

    mpSceneManager = new SceneManager();
	mpSceneManager->Initilize();

    mpObjectManager = new ObjectManager();
    mpObjectManager->Initilize();

    mpAttackManager = new AttackManager();
    mpCollisionManager = new CollisionManager();
    mpMapManager = new MapManager();
    mpMapManager->Initilize();
    mpTargetManager = new TargetManager();

    SetDrawScreen(DX_SCREEN_BACK);

    mbUninitializedFlag = false;
}

// 終了処理
void GameManager::Finalize()
{
    mpDotWeenManager->Finalize();
    mpObjectManager->Finalize();
    
    mpMapManager->Finalize();

    delete mpCameraManager;
    delete mpDotWeenManager;
    delete mpObjectManager;
    delete mpSceneManager;
    delete mpAttackManager;
    delete mpCollisionManager;
    delete mpMapManager;
    delete mpTargetManager;
}

// 更新
void GameManager::Update()
{
    Master::mpKeyState->Update();

    mpCameraManager->Update();

    mpObjectManager->ObjectUpdate();

    mpDotWeenManager->Update();

    mpCollisionManager->CollisionProcess();

    mpObjectManager->ObjectLastUpdate();

	mpSceneManager->Update();

    Master::mpResourceManager->Update();
}

// 必要であれば削除する
void GameManager::DeleteAllIfNeeded()
{
    mpObjectManager->DeleteAllIfNeeded();

    Master::mpThreadManager->CleanupReadyTasks();
}

// 描画
void GameManager::Draw()
{
    ClearDrawScreen();

    mpCameraManager->Draw();

    Master::mpResourceManager->StartDraw();
    mpObjectManager->ObjectDraw();
    mpMapManager->Draw();
    Master::mpResourceManager->MiddleDraw();

    mpObjectManager->ObjectDraw();

    mpMapManager->Draw();

    Master::mpResourceManager->LastDraw();

    // TODO: リソースの描画処理こちらに移す
    Master::mpDrawManager->Draw();

    Master::mpFadeManager->Draw();

    Master::mpResourceManager->DrawDataRelease();

#ifdef _DEBUG
    Master::mpImguiManager->Draw();
#endif

    ScreenFlip();
}
/*----------------------------------*/
/*【ウィンドウプロシージャ使用関数】*/
/*----------------------------------*/

// 別アプリ移動
void GameManager::OnDeactivate()
{
    // エフェクト
    Master::mpResourceManager->GetEffectResource()->StopAllEfect();

    // 時間
    Master::mpTimeManager->OnEnterBackground();
}

// 別アプリからこのアプリに移動
void GameManager::OnActivate()
{
    // エフェクト
    Master::mpResourceManager->GetEffectResource()->PlayAllEfect();

    // 時間
    Master::mpTimeManager->OnReturnForeground();
}

/*----------*/
/*【UI処理】*/
/*----------*/

// UIナンバー増加
int GameManager::IncreaseUINumber()
{
    mnUINumber += 1;
    if (mnUINumber == 1) {
        // 時間を止める
        Master::mpTimeManager->SetGameStopFlag(true);
    }
    return mnUINumber;
}

// UIナンバー減少
void GameManager::DecreaseUINumber()
{
    mnUINumber -= 1;
    if (mnUINumber <= 0) {
        mnUINumber = 0;

        // 時間を動かす
        Master::mpTimeManager->SetGameStopFlag(false);
    }
}

// ウィンドウプロシージャの定義
LRESULT WINAPI GameManagerWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
    //DEBUG::SaveText("GAME MAnager WndProc\n", DEBUG::DEBUG_MAP_TYPE::DEBUG_GAME_MANAGER_WND_PROC);
#endif
    if (Master::mpGameManager->GetUninitializedFlag())
    {
        return 0;
    }

    switch (msg)
    {
    case WM_ACTIVATEAPP:
    {
        if (wParam == FALSE)
        {
            // =========================
            // 別アプリへ切り替わった
            // Alt+Tab など
            // =========================
            Master::mpGameManager->OnDeactivate();
#ifdef _DEBUG
            //DEBUG::SaveText("別アプリへ切り替わった\n", DEBUG::DEBUG_MAP_TYPE::DEBUG_GAME_MANAGER_WND_PROC);
#endif
        }
        else
        {
            // =========================
            // アプリへ戻ってきた
            // =========================
            Master::mpGameManager->OnActivate();
#ifdef _DEBUG
            //DEBUG::SaveText("アプリへ戻ってきた\n", DEBUG::DEBUG_MAP_TYPE::DEBUG_GAME_MANAGER_WND_PROC);
#endif
        }
    }
    break;

    case WM_ACTIVATE:
    {
        if (LOWORD(wParam) == WA_INACTIVE)
        {
            // 非アクティブ
#ifdef _DEBUG
            //DEBUG::SaveText("非アクティブ\n", DEBUG::DEBUG_MAP_TYPE::DEBUG_GAME_MANAGER_WND_PROC);
#endif
        }
        else
        {
            // アクティブ
#ifdef _DEBUG
            //::SaveText("アクティブ\n", DEBUG::DEBUG_MAP_TYPE::DEBUG_GAME_MANAGER_WND_PROC);
#endif
        }
    }
    break;

    case WM_KILLFOCUS:
    {
        // フォーカス失った
#ifdef _DEBUG
            //DEBUG::SaveText("フォーカス失った\n", DEBUG::DEBUG_MAP_TYPE::DEBUG_GAME_MANAGER_WND_PROC);
#endif
    }
    break;

    case WM_SETFOCUS:
    {
        // フォーカス取得
#ifdef _DEBUG
            //DEBUG::SaveText("フォーカス取得\n", DEBUG::DEBUG_MAP_TYPE::DEBUG_GAME_MANAGER_WND_PROC);
#endif
    }
    break;

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }

    return 0;
}