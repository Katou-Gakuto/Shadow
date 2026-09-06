#pragma once
#include <string>

#include "ResourceData.h"
#include "UtilCalc.h"


class ResourceGraph;
class ResourceDivGraph;
class ResourceMovie;
class ResourceSound;
class ResourceBackSound;

class ResourceManager
{
	/*--------*/
	/*【共通】*/
	/*--------*/

public:
	// リソースファイルの名前
	static std::string msResourceFile;
	// ディスプレイサイズ
	static DisplaySize mstDisplaySize;

public:
	ResourceManager();
	~ResourceManager();

	/// <summary>初期化</summary>
	void Initilize();
	/// <summary>終了</summary>
	void Finalize();

	/// <summary>更新</summary>
	void Update();

	/// <summary>開始描画</summary>
	void StartDraw();
	/// <summary>中間描画</summary>
	void MiddleDraw();
	/// <summary>最終描画</summary>
	void LastDraw();

	/// <summary>描画データ解放</summary>
	void DrawDataRelease();

	/*--------*/
	/*【画像】*/
	/*--------*/
private:
	ResourceGraph* mpGraphResource;
	ResourceDivGraph* mpDivGraphResource;


public:
	/// <summary>画像リソース取得</summary>
	ResourceGraph* GetGraphResource() { return mpGraphResource; }
	/// <summary>分割画像リソース取得</summary>
	ResourceDivGraph* GetDivGraphResource() { return mpDivGraphResource; }

	/*--------*/
	/*【動画】*/
	/*--------*/
private:
	ResourceMovie* mpMovieResource;

public:
	/// <summary>動画リソース取得</summary>
	ResourceMovie* GetMovieResource() { return mpMovieResource; }

	/*------------*/
	/*【サウンド】*/
	/*------------*/
private:
	ResourceSound* mpSoundResource;
	ResourceBackSound* mpBackSoundResource;

public:
	/// <summary>サウンドリソース取得</summary>
	ResourceSound* GetSoundResource() { return mpSoundResource; }
	/// <summary>バックサウンドリソース取得</summary>
	ResourceBackSound* GetBackSoundResource() { return mpBackSoundResource; }
};
