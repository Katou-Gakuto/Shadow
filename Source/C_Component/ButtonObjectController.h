#pragma once

#include "BaseComponent.h"

#include <cstdint>

// 
class ButtonObjectController : public BaseComponent
{
public:
    // 


    // 
    void Push();

    // 
    void Hit();

    // 現在このボタンはONの状態なのか
    bool GetFlag() const;

    // 現在このボタンの上にプレイヤーがいるのか
    bool GetOnPlayer() const;

private:
    // この値がtrueの時は毎フレーム押し込み判定が続いているかを確認する
    bool mbHitButton;

    // 現在スイッチがONになっているか
    bool mdNowOn;

    // 現在スイッチは光域に位置しているのか
    bool mdLightSide;

    // 現在このスイッチの上にプレイヤーが乗っているのか
    bool mbOnPlayer;

    // ここが0以外の値になっていたらこのフレーム分だけONになり、後は自動でOFFになるという設定
    uint32_t mnOnFrameCount;

    // 現在ONになってから何フレーム経ったかを記憶する変数
    uint32_t mnNowFrameCount;

    // 
    void CheckOnPlayer();
};
