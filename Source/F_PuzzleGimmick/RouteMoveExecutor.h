#pragma once

#include "PuzzleGimmickData.h"

#include <cstdint>
#include <vector>

#include "../Y_Tool/VECTOR.h"

// 自身の移動状態がOFFの際に、どのような挙動を取るのか
enum RouteOffMode
{
    // 自身の状態がOFFになった瞬間にその場で止まる
    RouteOffMode_Stop,

    // 自身の状態がOFFになった瞬間にその場から移動の逆再生を始める
    RouteOffMode_Return,
};

// キーフレームの設定
struct RouteKeyFrame
{
    // 何フレーム目のものか
    uint32_t mnFrameCount;

    // このフレームでの座標はどこか
    VECTOR2D mvPos;
};

// 0フレーム時点の情報も記録しておいてください
class RouteData
{
public:
    // コンストラクタ
    RouteData();

    // デストラクタ
    ~RouteData();

    // 初期化を行う関数
    int Initialize(RouteOffMode offMode, bool loop, bool startOn);

    // キーフレームを登録する関数
    int Add(const RouteKeyFrame keyFrame);

    // 現在の状態を更新する関数
    void Update(bool nowOn);

    // フレームに合わせた座標を取得する関数
    VECTOR2D GetMovedPos(uint32_t frameCount) const;

    // フレームに合わせた回転量を取得する関数
    float GetRotatedParam(uint32_t frameCount) const;

    // 代入演算子(コピー)
    RouteData operator =(const RouteData &right);

private:
    // 登録されたキーフレームを記憶する動的配列
    // キーフレームからキーフレームまではLeapで補間
    std::vector<RouteKeyFrame> mlKeyFramesBox;

    // OFFの状態でどのような移動を行うのかの設定
    RouteOffMode mnOffMode;

    // ON時に最後のキーフレームまで到達した場合、移動の逆再生を行うかどうか
    bool mbOnLoop;

    // 現在ON状態なのか
    bool mdNowOn;
};

// 
class RouteMoveExecutor : public BaseGimmickExecutor
{
public:
    // コンストラクタ
    RouteMoveExecutor(PuzzleGimmickActiveParam param);

    // デストラクタ
    ~RouteMoveExecutor() override;

    // ギミックの内容を実行する関数
    // ※GameObject::EarlyUpdate()のタイミングで呼ばれます
    int EarlyUpdate(bool triggerSignal) override;

    // ギミックの内容を実行する関数
    // ※GameObject::Update()のタイミングで呼ばれます
    int Update(bool triggerSignal) override;

    // ギミックの内容を実行する関数
    // ※GameObject::LateUpdate()のタイミングで呼ばれます
    int LateUpdate(bool triggerSignal) override;

    // ギミック内容を描画する関数
    // ※既に実行段階である場合は引数がtrueになります。実行段階では描画しない、あるいはその逆の場合はこの引数を使ってください。
    int Draw(bool triggerSignal) override;

    // 
    bool SetMoveData(const RouteData &data);

    // 
    float GetRotate() const;

    // 
    const VECTOR2D &GetPos() const;

    // 
    void SetFrameCount(uint32_t frameCount);

private:
    // 
    RouteData mdMoveData;

    // 
    uint32_t mnFrameCount;

    // 
    float mfRotate;

    // 
    VECTOR2D mvPos;
};