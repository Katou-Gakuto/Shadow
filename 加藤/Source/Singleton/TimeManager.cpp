#include <windows.h>

#ifdef _DEBUG
#include <map>

#include "DebugLogs/DebugLog.h"
#endif

#include "TimeData.h"

#include "DxLib.h"

#include "TimeManager.h"

// コンストラクタ
TimeManager::TimeManager(int oneFrameTime)
: munLowestOneFrameSeconds(oneFrameTime)
, mstStartTime(TIME_DATA())
, mstPreviousTime(TIME_DATA())
, mstGameStopTime(TIME_DATA())
, mfTimeScale(1.0f)
, mfDeltaTime(0.0f)
, mstElapsedTime(TIME_DATA())
, mstFrameCount(FRAME_DATA())
, mstGameElapsedTime(TIME_DATA())
, mstGameFrameCount(FRAME_DATA())
, mstSceneElapsedFrame(FRAME_DATA())
, mstBackgroundTime(TIME_DATA())
, mstUnprocessedBackgroundTime(TIME_DATA())
, mulBackgroundStartTime(0ul)
, mbGameStopFlag(false)
, mbNewSceneTimeFlag(false)
{
#ifdef _DEBUG
    mmForPastFrameSecondsSave.clear();
#endif
}
// デストラクタ
TimeManager::~TimeManager()
{
}
// 初期化
void TimeManager::Initilize()
{
    timeBeginPeriod(1); // タイマーの分解量の設定を1msにする (1ミリ秒/1000秒)にする
    mstStartTime.Time = timeGetTime();
    mstElapsedTime = mstStartTime;
}

// 更新
bool TimeManager::GetNextUpdateFlag()
{
    unsigned long nowTime = timeGetTime();

    // バックグラウンド処理
    if (mstUnprocessedBackgroundTime.Full != 0)
    {
        // バックグラウンド時間を経過時間に加算する
        mstElapsedTime += mstUnprocessedBackgroundTime;
        
        // バックグラウンド時間をゲーム停止時間に加算する
        mstGameStopTime += mstUnprocessedBackgroundTime;

        // 処理したためリセット
        mstUnprocessedBackgroundTime = TIME_DATA();
    }

    // 経過時間差分
    unsigned long timeDiff = nowTime - mstElapsedTime.Time;    

    // 時間経過処理
    if (timeDiff >= munLowestOneFrameSeconds)
    {
        mstPreviousTime = mstElapsedTime;
#ifdef _DEBUG
        mstElapsedTime += timeDiff;
        mfDeltaTime = static_cast<float>(timeDiff) * 0.001f * mfTimeScale;
        //DEBUG::SaveText("[" + std::to_string(timeDiff) + "時間差][" + std::to_string(mfDeltaTime) + "秒][" + std::to_string(mfTimeScale) + "タイムスケール]\n", DEBUG::DEBUG_MAP_TYPE::DEBUG_TIME);
        //// 試験運用家でデバッグ時
        //mstElapsedTime += munLowestOneFrameSeconds;
        //mfDeltaTime = static_cast<float>(munLowestOneFrameSeconds) * 0.001f * mfTimeScale;
#else
        mstElapsedTime += timeDiff;
        mfDeltaTime = static_cast<float>(timeDiff) * 0.001f * mfTimeScale;
#endif

        mstFrameCount.Frame++;

        // 時間停止時処理
        if (mbGameStopFlag) {
            mstGameStopTime += timeDiff;

            // シーン時間フラグ無効化
            mbNewSceneTimeFlag = false;
        }
        else if (mbNewSceneTimeFlag) {
            mstGameStopTime += ((timeDiff) - munLowestOneFrameSeconds);
            mbNewSceneTimeFlag = false;
        }
        else
        {
            mstGameFrameCount.Frame++;
#ifdef _DEBUG
            mstGameElapsedTime += timeDiff;
            //mstGameElapsedTime += munLowestOneFrameSeconds;
#else
            mstGameElapsedTime += timeDiff;
            //mstGameElapsedTime += static_cast<unsigned long>(timeDiff * mfTimeScale);
#endif
        }

#ifdef _DEBUG
        if (mmForPastFrameSecondsSave.find(timeDiff) == mmForPastFrameSecondsSave.end())
        {
            mmForPastFrameSecondsSave[timeDiff] = 0u;
        }
        mmForPastFrameSecondsSave[timeDiff]++;
#endif

        return true;
    }

    return false;
}

// バックグラウンドに出た時の処理
void TimeManager::OnEnterBackground()
{
    mulBackgroundStartTime = timeGetTime();
}

// バックグラウンドから戻った処理
void TimeManager::OnReturnForeground()
{
    unsigned long backgroundTime = timeGetTime() - mulBackgroundStartTime;
    mstUnprocessedBackgroundTime += backgroundTime;
    mstBackgroundTime += backgroundTime;
}