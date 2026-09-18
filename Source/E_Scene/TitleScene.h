#pragma once

#include "BaseScene.h"

// 
enum TitleSelectMode
{
    TitleSelectMode_ModeSelect = 0,     // モード選択
    TitleSelectMode_StageSelect,        // ステージ選択に移る
    TitleSelectMode_Option,             // 設定(音量とか)に移る
    TitleSelectMode_Exit,               // ゲーム終了

    TitleSelectMode_Max,
};

class TitleScene : public BaseScene
{
public:
    // 
    TitleScene();

    // 
    ~TitleScene();

    // 
    int Create() override;

    // 
    int Initialize() override;

    // 
    int Finalize() override;

    // 
    int Update() override;

    // 
    int Draw() override;

private:
    // 「ボタンを押してください！」のフェーズか
    bool mbLogoPhase;

    // 現在のモード
    TitleSelectMode mnNowMode;

    // カーソルで選択しているモード
    TitleSelectMode mnCurrentMode;
};