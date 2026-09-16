#pragma once

#include "PuzzleGimmickData.h"

#include <cstdint>
#include <vector>

#include "../Y_Tool/VECTOR.h"

// 自身の移動状態のON/OFFをどのように決めるのか
enum MoveGimmickOnMode
{
    // ONになっているフレームのみ動く
    MoveGimmickOnMode_HitFrame,

    // 一度でもONになったら最後まで動く
    MoveGimmickOnMode_OnePush,
};

// 自身の移動状態がOFFの際に、どのような挙動を取るのか
enum MoveGimmickOffMode
{
    // 自身の状態がOFFになった瞬間にその場で止まる
    MoveGimmickOffMode_Stop,

    // 自身の状態がOFFになった瞬間にその場から移動の逆再生を始める
    MoveGimmickOffMode_Return,
};

// キーフレームの設定
struct MoveGimmickKeyFrame
{
    // 何フレーム目のものか
    uint32_t mnFrameCount;

    // このフレームでの回転量はどのくらいか
    float mfRotate;

    // このフレームでの座標はどこか
    VECTOR2D mvPos;
};

// 0フレーム時点の情報も記録しておいてください
class MoveGimmickData
{
public:
    // コンストラクタ
    MoveGimmickData();

    // デストラクタ
    ~MoveGimmickData();

    // 初期化を行う関数
    int Initialize(MoveGimmickOnMode onMode, MoveGimmickOffMode offMode, bool loop, bool startOn);

    // キーフレームを登録する関数
    int Add(const MoveGimmickKeyFrame keyFrame);

    // 現在の状態を更新する関数
    void Update(bool nowOn);

    // フレームに合わせた座標を取得する関数
    VECTOR2D GetMovedPos(uint32_t frameCount) const;

    // フレームに合わせた回転量を取得する関数
    float GetRotatedParam(uint32_t frameCount) const;

    // 代入演算子(コピー)
    MoveGimmickData operator =(const MoveGimmickData &right);

private:
    // 登録されたキーフレームを記憶する動的配列
    // キーフレームからキーフレームまではLeapで補間
    std::vector<MoveGimmickKeyFrame> mlKeyFramesBox;

    // ONの状態でどのような移動を行うのかの設定
    MoveGimmickOnMode mnOnMode;

    // OFFの状態でどのような移動を行うのかの設定
    MoveGimmickOffMode mnOffMode;

    // ON時に最後のキーフレームまで到達した場合、移動の逆再生を行うかどうか
    bool mbOnLoop;

    // 現在ON状態なのか
    bool mdNowOn;

    // ON状態からスタートする場合はtrueを設定してください
    // (初期化した際に最後のKeyFrameを採用します。また、OnModeがLoopの場合は失敗になるかもしれないので要注意)
    bool mbOnStart;
};

// 
class MoveGimmickSumilater : public BaseGimmickSumilater
{
public:
    // コンストラクタ
    MoveGimmickSumilater();

    // デストラクタ
    ~MoveGimmickSumilater();

    // シュミレーションを行う関数
    int Sumilate() override;

    // 
    bool SetMoveData(const MoveGimmickData &data);

    // 
    float GetRotate() const;

    // 
    const VECTOR2D &GetPos() const;

    // 
    void SetFrameCount(uint32_t frameCount);

private:
    // 
    MoveGimmickData mdMoveData;

    // 
    uint32_t mnFrameCount;

    // 
    float mfRotate;

    // 
    VECTOR2D mvPos;
};