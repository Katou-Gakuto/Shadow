#pragma once

#include "PuzzleGimmickData.h"

// オブジェクトを回転させるクラス
class RotateExecutor : public BaseGimmickExecutor
{
public:
    // 
    RotateExecutor(PuzzleGimmickActiveParam param);

    // 
    ~RotateExecutor();

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


private:
    // 
    float mfNowRotate;
};
