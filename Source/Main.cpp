#include "DxLib.h"


/*
* ゲームループ詳細
* gitに関して
* ブランチ説明詳細
* 
* 一欄目が
* 二欄目がひと段落
* 三欄目が大きい変更
* ここら辺苦手	
*/

/// <summary>
/// メイン
/// </summary>
/// <param name="hInstance">HINSTANCE</param>
/// <param name="hPrevInstance">HINSTANCE</param>
/// <param name="lpCmdLine">LPSTR</param>
/// <param name="nCmdShow">int</param>
/// <returns>int</returns>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// DXライブラリ前初期化

	// DXライブラリ初期化処理
	if (DxLib::DxLib_Init() == -1)
	{
		return -1;		// エラーが起きたら直ちに終了
	}

	// DXライブラリ後

	// DXライブラリ使用の終了処理
	DxLib::DxLib_End();

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

	// ソフトの終了
	return 0;
}
