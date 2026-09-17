#pragma once

#include "PuzzleGimmickData.h"

#include "../Y_Tool/VECTOR.h"

// 
class GameObject2D;

// オブジェクトを回転させるクラス
class FreeMoveExecutor : public BaseGimmickExecutor
{
public:
    // 
    FreeMoveExecutor(PuzzleGimmickActiveParam param, GameObject2D *myObject);

    // 
    ~FreeMoveExecutor();

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
    void SetMoveVec(const VECTOR2D &vec);

private:
    // 
    VECTOR2D mvMoveVec;

    // 
    GameObject2D *mpMyObject;
};
