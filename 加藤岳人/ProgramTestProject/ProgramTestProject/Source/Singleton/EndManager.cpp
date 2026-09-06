#include <windows.h>

#include "BitFlag.h"

#include "DxLib.h"

#include "EndManager.h"

#ifdef _DEBUG
#include "DebugLogs/DebugLog.h"
#endif

EndManager::EndManager()
: mullEndFlag(BIT_FLAG<unsigned long long>())
, mpDeleteObject(nullptr)
{
}
EndManager::~EndManager()
{
	if (mpDeleteObject != nullptr)
	{
		delete mpDeleteObject;
	}
}

// 初期化
void EndManager::Init()
{
}

// 終了フラグ
bool EndManager::EndFlag()
{
#ifdef _DEBUG
	// ビットフラグ設定
	if (BIT_FAILURE_FLAG)
	{
		SetEndFlag(true, END_FLAG_NUMBER::BIT_FLAG);
	}
#endif


	bool endFlag = false;

	endFlag |= (ProcessMessage() != 0);

	endFlag |= ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0);

	if (mullEndFlag.Bool())
	{
#ifdef _DEBUG
		// ブレークポイント弾き用
		if ((mullEndFlag & (1llu << (int)END_FLAG_NUMBER::WITHIN_EXPECTATION_FLAG)) != 0)
		{
			return true;
		}
#endif

		endFlag = true;
		ErrorLogFmtAdd("ERROR %llu", mullEndFlag);

		
#ifdef _DEBUG
		DEBUG::SaveText("\n強制終了 : " + std::to_string(mullEndFlag) + "\n\n");
#endif
	}

	return endFlag;
}