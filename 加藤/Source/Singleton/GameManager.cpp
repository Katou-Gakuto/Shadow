#include "DxLib.h"

#include "Master.h"

#include "CameraManager.h"
#include "DotWeenManager.h"
#include "DrawManager.h"
#include "GameManager.h"
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
: mpCameraManager(nullptr)
, mpDotWeenManager(nullptr)
, mnUINumber(0)
, mbUninitializedFlag(true)
, testX(0.5f)
, testY(0.5f)
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


    // 画面サイズ
    SetGraphMode(1280, 960, 32);
#ifndef _DEBUG
	// 画面サイズ
	SetGraphMode(1280, 960, 32);
#endif
}
#include "ResourceGraph.h"
#include <math.h>
int testHandle = -1;
// 初期化
void GameManager::Initilize()
{
    Master::mpTimeManager->Initilize();

    Master::mpDrawManager->Initilize();

    mpCameraManager = new CameraManager();

    mpDotWeenManager = new DotWeenManager();
    mpDotWeenManager->Initilize();

    Master::mpResourceManager->Initilize();
    testHandle = Master::mpResourceManager->GetGraphResource()->GetResourceHandle("Red.png");

    Master::mpStopManager->Initilize();

    SetDrawScreen(DX_SCREEN_BACK);

    mbUninitializedFlag = false;
}

// 終了処理
void GameManager::Finalize()
{
    mpDotWeenManager->Finalize();
    
    delete mpCameraManager;
    delete mpDotWeenManager;
}

// 更新
void GameManager::Update()
{
    Master::mpKeyState->Update();

    mpCameraManager->Update();

    mpDotWeenManager->Update();

    Master::mpResourceManager->Update();


    Master::mpImguiManager->AddDrawImgui(IMGUI_FLOAT_DATA::GetImguiData(
        { &testX, &testY },
        0.03f,
        0.03f,
        0.03f,
        0.0f,
        10.0f,
        "TEST_SIZE_BOX_",
        "%f",
        0,
        IMGUI_TYPE::SLIDER2
    )
    );
    Master::mpImguiManager->AddDrawImgui(IMGUI_FLOAT_DATA::GetImguiData(
        { &testX, &testY },
        0.03f,
        0.03f,
        0.03f,
        0.0f,
        10.0f,
        "TEST_SIZE_BOX_",
        "%f",
        0,
        IMGUI_TYPE::DRAG2
    )
    );
    Master::mpImguiManager->AddDrawImgui(IMGUI_FLOAT_DATA::GetImguiData(
        { &testX, &testY },
        0.03f,
        0.03f,
        0.03f,
        0.0f,
        10.0f,
        "TEST_SIZE_BOX_",
        "%f",
        0,
        IMGUI_TYPE::INPUT2
    )
    );
}

// 必要であれば削除する
void GameManager::DeleteAllIfNeeded()
{
    Master::mpThreadManager->CleanupReadyTasks();
}

// 描画
void GameManager::Draw()
{
    ClearDrawScreen();

    Master::mpResourceManager->StartDraw();
 /*   mpObjectManager->ObjectDraw();
    mpMapManager->Draw();*/
    Master::mpResourceManager->MiddleDraw();

    //mpObjectManager->ObjectDraw();

    //mpMapManager->Draw();

    Master::mpResourceManager->LastDraw();

    // TODO: リソースの描画処理こちらに移す
    Master::mpDrawManager->Draw();

    //Master::mpFadeManager->Draw();

    Master::mpResourceManager->DrawDataRelease();

#ifdef _DEBUG
    Master::mpImguiManager->Draw();
#endif

    if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::MOVE))
    {
        if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::UP))
        {
            testY -= 0.03f;
        }
        if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::DOWN))
        {
            testY += 0.03f;
        }
        if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::LEFT))
        {
            testX -= 0.03f;
        }
        if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::RIGHT))
        {
            testX += 0.03f;
        }
    }

    if (Master::mpKeyState->GetShadowGameKey(KEY_SHADOW_GAME_TYPE::TEST_1))
    {
        DOT_WEEN_DATA test;
        test.DotWeenElapsedTime = 0.0f;
        test.DotWeenTotalTime = 1.00f;
        test.DotWeenType = DOT_WEEN_TYPE::OUT_BOUNCE;
        test.EndData = 0.5f;
        test.GameTimeFlag = true;


        test.ChangeData = &testX;
        test.StartData = testX;
        Master::mpGameManager->GetDotWeenManager()->SetDotWeen(test);

        test.ChangeData = &testY;
        test.StartData = testY;
        Master::mpGameManager->GetDotWeenManager()->SetDotWeen(test);
    }

    DisplaySize test = ResourceManager::mstDisplaySize;
    //DrawGraph(test.Left_RatioWidth(testX - 0.05f), test.Up_RatioHeight(testY - 0.05f), testHandle, TRUE);
    DrawBox(test.Left_RatioWidth(testX - 0.05f), test.Up_RatioHeight(testY - 0.05f), test.Left_RatioWidth(testX + 0.05f), test.Up_RatioHeight(testY + 0.05f), GetColor(255, 0, 0), TRUE);

    /*VERTEX2D points[] =
    {
        { 320 + (int)(100 * cos(0 * DX_PI / 180)), 240 + (int)(100 * sin(0 * DX_PI / 180)) },
        { 320 + (int)(100 * cos(40 * DX_PI / 180)), 240 + (int)(100 * sin(40 * DX_PI / 180)) },
        { 320 + (int)(100 * cos(120 * DX_PI / 180)), 240 + (int)(100 * sin(120 * DX_PI / 180)) },
        { 320 + (int)(100 * cos(200 * DX_PI / 180)), 240 + (int)(100 * sin(200 * DX_PI / 180)) },
        { 320 + (int)(100 * cos(300 * DX_PI / 180)), 240 + (int)(100 * sin(300 * DX_PI / 180)) }
    };

    int testNumber = DrawPolygon2D(points, 5, testHandle, TRUE);*/

    VERTEX2D vertex[30];

    const float cx = 320.0f;
    const float cy = 240.0f;

    const float outerR = 100.0f;
    const float innerR = 50.0f;

    double angle[5] = {
        0.0,
        40.0,
        120.0,
        200.0,
        300.0
    };

    VECTOR outer[5];
    VECTOR inner[5];

    for (int i = 0; i < 5; i++)
    {
        double rad = angle[i] * 3.14159265358979323846 / 180.0;

        outer[i] = VGet(
            cx + outerR * (float)cos(rad),
            cy + outerR * (float)sin(rad),
            0.0f
        );

        inner[i] = VGet(
            cx + innerR * (float)cos(rad),
            cy + innerR * (float)sin(rad),
            0.0f
        );
    }

    COLOR_U8 color = GetColorU8(255, 255, 255, 255 * 1.0f);

    int v = 0;

    for (int i = 0; i < 5; i++)
    {
        int next = (i + 1) % 5;

        // 外側 → 外側 → 内側
        vertex[v++].pos = outer[i];
        vertex[v++].pos = outer[next];
        vertex[v++].pos = inner[next];

        // 外側 → 内側 → 内側
        vertex[v++].pos = outer[i];
        vertex[v++].pos = inner[next];
        vertex[v++].pos = inner[i];
    }

    for (int i = 0; i < 30; i++)
    {
        vertex[i].rhw = 1.0f;
        vertex[i].dif = color;
        vertex[i].u = 0.0f;
        vertex[i].v = 0.0f;
    }

    //DrawPolygon2D(
    //    vertex,
    //    10,       // 三角形10枚
    //    testHandle,
    //    TRUE
    //); 

    //VERTEX2D vertex[15];

    //const float cx = 320.0f;
    //const float cy = 240.0f;
    //float radius[5] = {
    //100.0f,
    //100.0f,
    // -30.0f,   // ← ここを内側にへこませる
    //100.0f,
    //100.0f
    //};

    //double angle[5] = {
    //    0.0,
    //    40.0,
    //    120.0,
    //    200.0,
    //    300.0
    //};

    //// 外周の頂点
    //VECTOR p[5];
    //for (int i = 0; i < 5; i++)
    //{
    //    double rad = angle[i] * 3.14159265358979323846 / 180.0;

    //    p[i] = VGet(
    //        cx + radius[i] * (float)cos(rad),
    //        cy + radius[i] * (float)sin(rad),
    //        0.0f
    //    );
    //}

    //// 中心
    //VECTOR center = VGet(cx, cy, 0.0f);

    //COLOR_U8 color = GetColorU8(255, 0, 0, 255);

    //// 5個の三角形に分割
    //for (int i = 0; i < 5; i++)
    //{
    //    int next = (i + 1) % 5;

    //    vertex[i * 3 + 0].pos = center;
    //    vertex[i * 3 + 1].pos = p[i];
    //    vertex[i * 3 + 2].pos = p[next];

    //    vertex[i * 3 + 0].rhw = 1.0f;
    //    vertex[i * 3 + 1].rhw = 1.0f;
    //    vertex[i * 3 + 2].rhw = 1.0f;

    //    vertex[i * 3 + 0].dif = color;
    //    vertex[i * 3 + 1].dif = color;
    //    vertex[i * 3 + 2].dif = color;

    //    vertex[i * 3 + 0].u = 0.0f;
    //    vertex[i * 3 + 0].v = 0.0f;
    //    vertex[i * 3 + 1].u = 0.0f;
    //    vertex[i * 3 + 1].v = 0.0f;
    //    vertex[i * 3 + 2].u = 0.0f;
    //    vertex[i * 3 + 2].v = 0.0f;
    //}

    //// ★描画呼び出しは1回
    //DrawPolygon2D(
    //    vertex,
    //    5,
    //    testHandle,
    //    TRUE
    //);

    ScreenFlip();
}
/*----------------------------------*/
/*【ウィンドウプロシージャ使用関数】*/
/*----------------------------------*/

// 別アプリ移動
void GameManager::OnDeactivate()
{
    // 時間
    Master::mpTimeManager->OnEnterBackground();
}

// 別アプリからこのアプリに移動
void GameManager::OnActivate()
{
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
    Master::mpImguiManager->ImguiWndProcProcess(hWnd, msg, wParam, lParam);
    Master::mpKeyState->KeyStateWndProcProcess(hWnd, msg, wParam, lParam);
#ifdef _DEBUG
    DEBUG::SaveText("GAME MAnager WndProc\n", DEBUG::DEBUG_MAP_TYPE::DEBUG_GAME_MANAGER_WND_PROC);
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