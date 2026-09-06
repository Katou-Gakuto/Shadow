#pragma once
#include "BitFlag.h"

enum class LOADING_NUMBER
{
	FADE = 0,
	SCENE,
};

class LoadingManager
{
private:
	// ローディングフラグ
	BIT_FLAG<unsigned long long> mullLoadingFlag;
	
public:
	LoadingManager();
	~LoadingManager();

	// ローディング
	void Loading();

	/*--------*/
	/*【設定】*/
	/*--------*/
	
	/// <summary>
	/// ローディングフラグ設定
	/// </summary>
	/// <param name="number">フラグのビット数</param>
	inline void SetLoadingFlag(LOADING_NUMBER number) { mullLoadingFlag.EnableFlag(number); }

	/*--------*/
	/*【取得】*/
	/*--------*/

	/// <summary>
	/// ローディングフラグ取得
	/// </summary>
	/// <returns>0なら「false」 0以外なら「true」</returns>
	inline bool GetLoadingFlag() const { return mullLoadingFlag.Bool(); }
};