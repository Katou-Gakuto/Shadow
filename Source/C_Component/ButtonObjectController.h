#pragma once

#include "BaseComponent.h"

#include <cstdint>

#include "../Y_Tool/VECTOR.h"

// 
class ButtonObjectController : public BaseComponent
{
public:
    // 
    ButtonObjectController(GameObject *myObject, const VECTOR2D &localLeftUp, const VECTOR2D &localRightDown);

    // 
    ~ButtonObjectController() override;

    int Create() override;
    int Initialize() override;
    int Finalize() override;
    int EarlyUpdate() override;
    int Update() override;
    int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision) override;
    int LateUpdate() override;
    int Draw() override;

    // 現在このボタンはONの状態なのか
    bool CheckButtonSignal() const;

    // 現在このボタンの上にプレイヤーがいるのか
    bool GetLightSide() const;

    // 現在このボタンの上にプレイヤーがいるのか
    bool CheckOnPlayer() const;

private:
    // 現在スイッチがONになっているか
    bool mbNowOn;

    // この値がtrueの時は毎フレーム押し込み判定が必要なのかを確認する
    bool mbHitButton;

    // 前回のフレームでもボタンを踏んでいるか
    bool mbPushLastFrame;

    // 現在スイッチは光域に位置しているのか
    bool mbLightSide;

    // ここが0以外の値になっていたらこのフレーム分だけONになり、後は自動でOFFになるという設定
    uint32_t mnOnFrameCount;

    // 現在ONになってから何フレーム経ったかを記憶する変数
    uint32_t mnNowFrameCount;

    // 
    VECTOR2D mvLeftUp;

    // 
    VECTOR2D mvRightDown;

    // 
    void CheckLightSide();

};
