#pragma once
#include "BitFlag.h"

// 終了フラグナンバー
enum class END_FLAG_NUMBER
{
	WITHIN_EXPECTATION_FLAG = 0,	// 期待通り
	START_SCENE_FLAG,	// 開始シーン
	RESOURCE_FLAG,	// リソースマネージャー
	LOADING_FLAG,	// ローディングマネージャー
	MAP_FLAG,	// マップマネージャー	
	HANDLE_FLAG,	// ハンドルコンテナ
	RESOURCE_BASE_FLAG,	// リソースベース
	BIT_FLAG,	// ビットフラグ
	FILE_FLAG,	// ファイルフラグ
	NO_DATA_FLAG,// データ無しフラグ
	STATE_NULL_FUNCTION_FLAG,	// ステート関数ヌルポインタフラグ
	ANIMATION_MISSING_FLAG,	// アニメーション不足フラグ

	MAX_COUNT	// 最大値計測用
};

static_assert(
    static_cast<int>(END_FLAG_NUMBER::MAX_COUNT) < 64,
    "エンドマネージャーのenum classが規定外にまで及んだ"
);

class EndManager
{
private:
	// 終了フラグ
	BIT_FLAG<unsigned long long> mullEndFlag;
	
	// 削除オブジェクト
	void* mpDeleteObject;

public:
	EndManager();
	~EndManager();

	/*初期化*/
	void Init();

	/// <summary>
	/// 終了フラグ
	/// </summary>
	/// <returns>どれかの終了フラグが有効なら「true」</returns>
	bool EndFlag();

	/// <summary>
	/// 終了フラグ設定
	/// </summary>
	/// <param name="flag">設定フラグ</param>
	/// <param name="number">設定する終了フラグのビット数</param>
	inline void SetEndFlag(bool flag, END_FLAG_NUMBER number) { mullEndFlag.SetFlag(flag, number); }

	/// <summary>
	/// 削除オブジェクト設定
	/// </summary>
	/// <param name="deleteObject">終了時に削除するオブジェクト</param>
	inline void SetDeleteObject(void* deleteObject) { mpDeleteObject = deleteObject; }

	/// <summary>
	/// ビットのエンドフラグ取得
	/// </summary>
	/// <returns>ビットフラグ</returns>
	inline BIT_FLAG<unsigned long long> GetBitEndflag() const { return mullEndFlag; }
};