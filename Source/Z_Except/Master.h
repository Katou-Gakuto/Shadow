#pragma once

// シーンマネージャー
class BaseSceneManager;
// 終了マネージャー
class EndManager;
// 時間マネージャー
class TimeManager;
// キー情報
class KeyState;

// 
class Master
{
public:
    // シーンのManager
    static BaseSceneManager *mpBaseSceneManager;

    // 終了の管理
    static EndManager* mpEndManager;
    // 時間の管理
    static TimeManager* mpTimeManager;

    // キーの情報管理
    static KeyState* mpKeyState;

    // そのほかManager

    // Masterの各メンバをnewする関数
    static int Initialize();

    // Masterの各メンバをdeleteする関数
    static int Finalize();
};