#include "DxLib.h"


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
	// DXライブラリ初期化処理
	if (DxLib::DxLib_Init() == -1)
	{
		return -1;		// エラーが起きたら直ちに終了
	}

	// DXライブラリ使用の終了処理
	DxLib::DxLib_End();

	// ソフトの終了
	return 0;
}
