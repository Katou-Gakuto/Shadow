#pragma once
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <windows.h>

#ifdef _DEBUG

// デバッグ用
namespace DEBUG
{
	// デバッグ用マップデータ種類	(追加自由)
	enum DEBUG_MAP_TYPE
	{
		DEBUG_BASE = 0,
		DEBUG_EFFECT,
		DEBUG_UNDISCOVERED,
		DEBUG_GRAPH,
		DEBUG_3D_MODEL,
		DEBUG_SCENE_TITLE,
		DEBUG_GAME_MANAGER_WND_PROC,
		DEBUG_FADE,
		DEBUG_FAILURE_LOG,
		DEBUG_TIME,
	};

	// デバッグ用処理種類
	enum class DEBUG_PROCESS_TYPE
	{
		NONE = -1,
		FUNCTION_CALL,		// 関数呼び出し含む
		TIME,				// 時間描画
    	ALL_FILE_OUTPUT,	// 全デバッグファイルに出力
	};

	// デバッグ用追加ファイル別データ
	struct DEBUG_FILE_DATA
	{
		std::vector<DEBUG_PROCESS_TYPE> debugType;
		
		std::vector<std::string> plusFileName;

		bool outputFileFlag = true;// TODO: デバッグ描画フラグ
	};

	// デバッグ用SaveText関数内データ
	struct DEBUG_SAVE_TEXT_FUNCTION_DATA
	{
		std::string timeString;
		std::string logString;
		int debugMapType;
	};

	static bool DebugOutputFileFlag;

	static std::string LogFileString;
	static std::vector<DEBUG_PROCESS_TYPE> LogFileProcess;

	static std::map<int, DEBUG::DEBUG_FILE_DATA> PlusLogFileData;

	static HANDLE DebugProcessHandle;

	/// <summary>デバッグ初期化</summary>
	void DebugInitialization(bool debugOutputFlag);

	/// <summary>デバッグ出力先を追加する(上書き)</summary>
	void DebugCreateLogFileName(DEBUG_MAP_TYPE debugMapType = DEBUG_MAP_TYPE::DEBUG_BASE, std::vector<std::string> plusFileName = {});

	/// <summary>デバッグ出力情報追加</summary>
	void DebugLogAddData(DEBUG_PROCESS_TYPE debugProcessType, DEBUG_MAP_TYPE debugMapType = DEBUG_MAP_TYPE::DEBUG_BASE);
	
	/// <summary>デバッグ出力情報削除</summary>
	void DebugLogSubData(DEBUG_PROCESS_TYPE debugProcessType, DEBUG_MAP_TYPE debugMapType = DEBUG_MAP_TYPE::DEBUG_BASE);

// TODO: 可変長引数にする
	/// <summary>文字列をファイルに追加する</summary>
	void SaveText(std::string logString, DEBUG_MAP_TYPE debugMapType = DEBUG_MAP_TYPE::DEBUG_BASE);

	/// <summary>デバッグ種類別の処理</summary>
	void ProcessByDebugType(std::ofstream *file, DEBUG_PROCESS_TYPE debugProcessType, DEBUG_SAVE_TEXT_FUNCTION_DATA debugSaveTextFunctionData);

	/// <summary>関数名を取得</summary>
	/// <returns>関数(ファイル名.行数)/関数(ファイル名.行数)</returns>
	std::string FunctionCallHistoryAcquisition(int deleteNumber = 1);

	/// <summary>時間文字列取得</summary>
	std::string TimeToString();
}

#endif


/*------------------------------------------------------------【見本】------------------------------------------------------------*/
// 初期は【LogDebug/debug_0.txt】にデバッグ情報出力
// 
// /*-------------------------------------------------------------------------------------------*/
// /*【    デバッグ用にテキストを外部に出力する    　　　　　　　　　　　　　　　　　　　　　 】*/
// /*【    「SaveText」でファイルに出力していく     　　　　　　　　　　　　　　　　　　　　　】*/
// /*【    共通で基本ファイルに出力で追加で別のファイルに保存できる 　　　　　　　　　　　　　】*/
// /*【    以下は見本(初期設定は初期化時に呼ぶ　テキスト出力は出力したい文字列を適時入力)     】*/
// /*-------------------------------------------------------------------------------------------*/
// #ifdef _DEBUG
// #include "DebugLogs/DebugLog.h"
// #endif

// int main()
// {
// //【初期設定】
// #ifdef _DEBUG
//     // デバッグ初期化
//     DEBUG::DebugInitialization(true/*デバッグファイルを生成するフラグ*/);
	
//     // デバッグテキストの出力先を新しいファイルにする
//     {
//         // 基本出力ファイルを新しいファイルにする
//         DEBUG::DebugCreateLogFileName();

//         // 基本ファイル出力時追加出力設定
//         DEBUG::DebugLogAddData(DEBUG::DEBUG_PROCESS_TYPE::FUNCTION_CALL);
//         DEBUG::DebugLogAddData(DEBUG::DEBUG_PROCESS_TYPE::TIME);
 
//         // 追加出力ファイル設定
//         {
//             // 出力先ファイル情報追加
//             DEBUG::DebugCreateLogFileName(DEBUG::DEBUG_MAP_TYPE::DEBUG_SCENE_TITLE/*【タグ】*/, "_title"/*【追加ファイル名】*/);
//             // 追加ファイル出力時追加出力設定
//             DEBUG::DebugLogAddData(DEBUG::DEBUG_PROCESS_TYPE::ALL_FILE_OUTPUT/*【追加出力設定】*/, DEBUG::DEBUG_MAP_TYPE::DEBUG_SCENE_TITLE/*【タグ】*/);
            
//         }

//         // 追加出力ファイル設定
//         {
//             DEBUG::DebugCreateLogFileName(DEBUG::DEBUG_MAP_TYPE::DEBUG_EFFECT, "_effect");
//             DEBUG::DebugLogAddData(DEBUG::DEBUG_PROCESS_TYPE::FUNCTION_CALL, DEBUG::DEBUG_MAP_TYPE::DEBUG_EFFECT);
//             DEBUG::DebugLogAddData(DEBUG::DEBUG_PROCESS_TYPE::TIME, DEBUG::DEBUG_MAP_TYPE::DEBUG_EFFECT);
//         }
//     }
// #endif

 
 
// //【テキスト出力】 
// #ifdef _DEBUG
//     // 基本ファイルにのみ出力
//     DEBUG::SaveText("個々の文字が保存される\n");
//     // 基本ファイルと_effect付きファイルに出力
//     DEBUG::SaveText("追加のファイルに保存できる\n", DEBUG::DEBUG_MAP_TYPE::DEBUG_EFFECT);
// #endif
	
//     return 0;
// }