#include "DxLib.h"

#include "E_Scene/BaseSceneManager.h"
#include "Z_Except/Master.h"

/*
【フォルダの規則】

「どのフォルダにファイルを入れようか……」
という悩みがあると思います。

そういう場合は『どのような役割を持っているか』でフォルダの入れる先を決めてください。
なお、ヘッダファイルとソースファイルでフォルダ分けはしない予定です。

あとフォルダ名の頭についているアルファベットはソートの順番を指定したいだけなので気にしないでください。

【】



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

	// ↓約17ms経ってるか、とかメッセージの処理とか諸々が終わったよってところから
	if (true)
	{
		// コントローラー二つ分の入力を取得する

		// 
		Master::mpBaseSceneManager->Update();
		
		// 
		Master::mpBaseSceneManager->Draw();
	}

	// DXライブラリ使用の終了処理
	DxLib::DxLib_End();

	// ソフトの終了
	return 0;
}
