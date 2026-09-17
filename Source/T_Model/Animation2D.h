#pragma once

#include "Pose2D.h"

enum Animation2DTag : unsigned short
{
    Animation2DTag_Default = 0,
    
    Animation2DTag_Stop_Left,
    Animation2DTag_Stop_Right,
    Animation2DTag_Stop_Up,
    Animation2DTag_Stop_Down,
    Animation2DTag_Walk_Left,
    Animation2DTag_Walk_Right,
    Animation2DTag_Walk_Up,
    Animation2DTag_Walk_Down,
    
    Animation2DTag_Battle,

    Animation2DTag_Max,
};

// Animation2D、またの名をPoseList
class Animation2D
{
private:
    Pose2D *mpFirstPose;
    Animation2DTag mnAnimation2DTag;
    unsigned char mcCurrentPose; // 現在のポーズ番号
    unsigned char mcPoseMax; // ポーズの最大値
	unsigned char mcChangeCount; // ポーズを次に進めるまでのカウントダウン(0で次へ)
    unsigned char mcChangeInterval; // mcChangeCountの更新の際の数値
	bool mbLoopFlag; // ループするアニメーションかどうか
	bool mbEndTimingNow; // 非ループのアニメーションが最後のフレームに入ったかどうか

	Animation2D *mpPrev; // 前のポーズ
	Animation2D *mpNext; // 次のポーズ

public:
    Animation2D();
    Animation2D(Animation2DTag tag, unsigned char poseChangeInterval, bool loopFlag);
    ~Animation2D();

    int UpdatePoseChangeCount(); // ポーズを次に進めるカウントダウンの更新処理を行う

    int Initialize();
    int Finalize();
    int Update();
    int Draw();

    int Add(Pose2D *target);                    // ポーズを追加する
    int DeleteTarget(Pose2D *target);           // 特定のポーズを削除する
    int DeleteAll();                            // 全てのポーズを削除する
    int ConnectTarget(Pose2D *prev, Pose2D *target, Pose2D *next);// ポーズを追加する際に、前後のポーズと接続する
    int IsolateTarget(Pose2D *target);          // 特定のポーズを孤立させる
    Pose2D *SearchPose2D(unsigned char index);  // ポーズを探す
    Pose2D *GetCurrentPose2D() { return this->SearchPose2D(this->mcCurrentPose); }// ポーズを探す

	Animation2DTag GetTag() const { return this->mnAnimation2DTag; } // アニメーションのタグを取得する
	bool GetLoopFlag() const { return this->mbLoopFlag; } // ループするアニメーションかどうかを取得する
	bool CheckEndTimingNow() const { return this->mbEndTimingNow; } // 非ループのアニメーションが最後のフレームに入ったかどうかを取得する
	void SetPoseChangeInterval(char interval) { this->mcChangeInterval = interval; } // ポーズを次に進めるまでのカウントダウンの更新の際の数値を設定する
	void SetPoseChangeCount(char count) { this->mcChangeCount = count; } // ポーズを次に進めるまでのカウントダウンを設定する
	void SetLoopFlag(bool flag) { this->mbLoopFlag = flag; } // ポーズがループするかを設定する

    void SetPrev(Animation2D *prev) { this->mpPrev = prev; }
    void SetNext(Animation2D *next) { this->mpNext = next; }
    Animation2D *GetPrev() { return this->mpPrev; }
    Animation2D *GetNext() { return this->mpNext; }
};