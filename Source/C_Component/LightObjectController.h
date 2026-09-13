#pragma once

#include "GimmickObjectController.h"

#include "../Y_Tool/VECTOR.h"
#include "../Z_Except/LightLineNode.h"

#include <vector>

// 全方向へのライトのやつ
class LightObjectController : public GimmickObjectController
{
public:
    // 
    LightObjectController();

    // 
    ~LightObjectController();

    virtual int Create() override;
    virtual int Initialize() override;
    virtual int Finalize() override;
    virtual int EarlyUpdate() override;
    virtual int Update() override;
    virtual int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision) override;
    virtual int LateUpdate() override;
    virtual int Draw() override;

    // このライトのレイの角度を設定する関数
    virtual void SetLineAngleBox(uint32_t lineCount);

    // このライトの光域を計算する関数
    bool CalculateLineEndPos(const VECTOR2D &startPos);

    // このライトのレイの終点座標の配列を取得する関数
    VECTOR2D *GetLineEndPosBox();

    // このライトのレイの本数を取得する関数
    uint32_t GetLightLineCount() const;

    // このライトの長さを取得する関数
    float GetLength() const;

    // このライトのレイの本数を設定する関数
    void SetLightLineCount(uint32_t count);

    // このライトの長さを設定する関数
    void SetLength(float length);

protected:
    // このライトのレイの角度を設定する関数
    void SetLineAngleBox_In(uint32_t lineCount, float maxAngle);


private:
    // ライトの距離。3000くらいあったらこのゲームでは無限くらいだと思うけど、無限って意味で0にするのはやめてね。
    float mfLength;

    // ライトの基準となる角度
    float mfBaseAngle;

    // 
    LightArea *mpNowLightArea;
};

// 基準の方向から一定角度へのライト
class DirectionLightController : public LightObjectController
{
public:
    // 
    DirectionLightController();

    // 
    ~DirectionLightController();

    int Create() override;
    int Initialize() override;
    int Finalize() override;
    int EarlyUpdate() override;
    int Update() override;
    int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision) override;
    int LateUpdate() override;
    int Draw() override;

    // このライトのレイの角度を設定する関数
    void SetLineAngleBox(uint32_t lineCount) override;

    // 
    void SetAngle(float radian);

private:
    // 角度はオブジェクトが持つ回転を基準に
    // 左方向に-mfAngle / 2
    // 右方向に+mfAngle / 2
    // とします。
    // 値はラジアン角で入力してください
    float mfAngle;
};
