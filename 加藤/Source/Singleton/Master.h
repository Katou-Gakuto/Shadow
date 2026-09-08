#pragma once

class DrawManager;
class EndManager;
class GameManager;
class ImguiManager;
class KeyState;
class LoadingManager;
class ResourceManager;
class StopManager;
class ThreadManager;
class TimeManager;

// 変える
class SoundManager;

class Master
{
public:
	static DrawManager* mpDrawManager;	// 描画マネージャー
	static EndManager* mpEndManager;	// エンドマネージャー
	static GameManager* mpGameManager;	// ゲームマネージャー
	static ImguiManager* mpImguiManager;	// Imguiマネージャー
	static KeyState* mpKeyState;	// キーステート
	static LoadingManager* mpLoadingManager;	// ローディングマネージャー
	static ResourceManager* mpResourceManager;	// リソースマネージャー
	static StopManager* mpStopManager;	// ストップマネージャー
	static ThreadManager* mpThreadManager;	// スレッドマネージャー
	static TimeManager* mpTimeManager;	// タイムマネージャー

	/*マスター所持物全削除処理*/
	static void AllDelete();
};