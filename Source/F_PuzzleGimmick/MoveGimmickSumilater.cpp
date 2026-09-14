#include "MoveGimmickSumilater.h"


// コンストラクタ
MoveGimmickData::MoveGimmickData()
{
}

// デストラクタ
MoveGimmickData::~MoveGimmickData()
{
}

// 初期化を行う関数
int MoveGimmickData::Initialize(MoveGimmickOnMode onMode, MoveGimmickOffMode offMode, bool loop, bool startOn)
{
	return 0;
}


// キーフレームを登録する関数
int MoveGimmickData::Add(const MoveGimmickKeyFrame keyFrame) 
{
	return 0;
}

// 現在の状態を更新する関数
void MoveGimmickData::Update(bool nowOn)
{
}

// フレームに合わせた座標を取得する関数
VECTOR2D MoveGimmickData::GetMovedPos(uint32_t frameCount)
{
}


// フレームに合わせた回転量を取得する関数
float MoveGimmickData::GetRotatedParam(uint32_t frameCount)
{
}