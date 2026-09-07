#pragma once

class DataManager;
class DrawManager;
class EndManager;
class FadeManager;
class GameManager;
class ImguiManager;
class KeyState;
class LoadingManager;
class ResourceManager;
class StageDataManager;
class StopManager;
class TelopManager;
class ThreadManager;
class TimeManager;

// 変える
class SoundManager;

class Master
{
public:
	static DataManager* mpDataManager;	// データマネージャー
	static DrawManager* mpDrawManager;	// 描画マネージャー
	static EndManager* mpEndManager;	// エンドマネージャー
	static FadeManager* mpFadeManager;	// フェードマネージャー
	static GameManager* mpGameManager;	// ゲームマネージャー
	static ImguiManager* mpImguiManager;	// Imguiマネージャー
	static KeyState* mpKeyState;	// キーステート
	static LoadingManager* mpLoadingManager;	// ローディングマネージャー
	static ResourceManager* mpResourceManager;	// リソースマネージャー
	static StageDataManager* mpStageDataManager;	// ステージデータマネージャー
	static StopManager* mpStopManager;	// ストップマネージャー
	static TelopManager* mpTelopManager;	// テロップマネージャー
	static ThreadManager* mpThreadManager;	// スレッドマネージャー
	static TimeManager* mpTimeManager;	// タイムマネージャー

	static SoundManager* mpSoundManager;	// タイムマネージャー

	/*マスター所持物全削除処理*/
	static void AllDelete();
};