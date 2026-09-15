#pragma once

#include "PuzzleGimmickData.h"

#include "../G_LightArea/LightArea.h"

#include "../Y_Tool/VECTOR.h"

// LightのON/OFFを行うシミュレーター
class LightGimmickSumilater : public BaseGimmickSumilator
{
public:
    // コンストラクタ
    LightGimmickSumilater();

    // デストラクタ
    ~LightGimmickSumilater() override;

    // シミュレーションを行うかをCheckerの結果も踏まえたうえで判断する関数
    bool CheckSumilate(bool checkerSignal) const override;

    // シミュレーションを行う関数
    int Sumilate() override;

    // 実行に移すかをCheckerの結果も踏まえたうえで判断する関数
    int CheckExecute(bool checkerSignal) const override;

    // シミュレートの結果を実行を行う関数
    int Execute() override;

    // シミュレーション内容を描画する関数
    // ※既に実行段階である場合は引数がtrueになります。実行段階では描画しない、あるいはその逆の場合はこの引数を使ってください。
    int Draw(bool sumilating, bool executing) override;

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

    // このライトの視点を設定する関数
    void SetStartPos(const VECTOR2D &pos);

    // このライトの全体の角度を設定する関数
    void SetLightAngle(float radian);

protected:
    // このライトのレイの角度を設定する関数
    void SetLineAngleBox_In(uint32_t lineCount, float maxAngle);

    // この光域を取得する関数
    LightArea *GetMyLightArea() const;

private:
    // 今ライトが点いているか
    bool mbLightOn;

    // ライトの距離。3000くらいあったらこのゲームでは無限くらいだと思うけど、無限って意味で0にするのはやめてね。
    float mfLength;

    // ライトの基準となる角度
    float mfBaseAngle;

    // 基準から左右に広がるライトの合計角度
    float mfLightAngle;

    // 
    VECTOR2D mvStartPos;

    // 
    int mnAreaIndex;
};