#pragma once
#include "BitFlag.h"

// 終了フラグナンバー
enum class END_FLAG_NUMBER
{
	WITHIN_EXPECTATION_FLAG = 0,	// 期待通り
	BIT_FLAG,	// ビットフラグ

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