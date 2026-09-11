#pragma once

#include <cstdint>
#include <vector>

#include "../Y_Tool/VECTOR.h"

// 自身の状態のON/OFFをどのように決めるのか
enum MapObjectOnMode
{
    // 自身の状態がONになっているフレームのみ動く
    MapObjectOnType_HitFrame,

    // 自身の状態が一度でもONになったら最後まで動く
    MapObjectOnType_OnePush,
};

// 自身の状態がOFFの際に、どのような挙動を取るのか
enum MapObjectOffMode
{
    // 自身の状態がOFFになった瞬間にその場で止まる
    MapObjectOffType_Stop,

    // 自身の状態がOFFになった瞬間にその場から移動の逆再生を始める
    MapObjectOffType_Return,
};

// キーフレームの設定
struct MoveDataKeyFrame
{
    // 何フレーム目のものか
    uint32_t mnFrameCount;

    // このフレームでの回転量はどのくらいか
    float mfRotate;

    // このフレームでの座標はどこか
    VECTOR2D mvPos;
};

// 0フレーム時点の情報も記録しておいてください
class MoveData
{
public:
    // コンストラクタ
    MoveData();

    // デストラクタ
    ~MoveData();

    // 初期化を行う関数
    int Initialize(MapObjectOnMode onMode, MapObjectOffMode offMode, bool loop, bool startOn);

    // キーフレームを登録する関数
    int Add(const MoveDataKeyFrame keyFrame);

    // 現在の状態を更新する関数
    void Update(bool nowOn);

    // フレームに合わせた座標を取得する関数
    VECTOR2D GetMovedPos(uint32_t frameCount) const;

    // フレームに合わせた回転量を取得する関数
    float GetRotatedParam(uint32_t frameCount) const;

    // 代入演算子(コピー)
    MoveData operator =(const MoveData &right);

private:
    // 登録されたキーフレームを記憶する動的配列
    // キーフレームからキーフレームまではLeapで補間
    std::vector<MoveDataKeyFrame> mlKeyFramesBox;

    // ONの状態でどのような移動を行うのかの設定
    MapObjectOnMode mnOnMode;

    // OFFの状態でどのような移動を行うのかの設定
    MapObjectOffMode mnOffMode;

    // ON時に最後のキーフレームまで到達した場合、移動の逆再生を行うかどうか
    bool mbOnLoop;

    // 現在ON状態なのか
    bool mdNowOn;

    // ON状態からスタートする場合はtrueを設定してください
    // (初期化した際に最後のKeyFrameを採用します。また、OnModeがLoopの場合は失敗になるかもしれないので要注意)
    bool mbOnStart;
};
