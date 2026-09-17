#include "RotateExecutor.h"

// 
RotateExecutor::RotateExecutor(PuzzleGimmickActiveParam param) :
    mfNowRotate(0.0f),
    BaseGimmickExecutor(param)
{
}

// 
RotateExecutor::~RotateExecutor()
{
}

// ギミックの内容を実行する関数
// ※GameObject::EarlyUpdate()のタイミングで呼ばれます
int RotateExecutor::EarlyUpdate(bool triggerSignal)
{
    // 
    return 0;
}

// ギミックの内容を実行する関数
// ※GameObject::Update()のタイミングで呼ばれます
int RotateExecutor::Update(bool triggerSignal)
{
    // 
    return 0;
}

// ギミックの内容を実行する関数
// ※GameObject::LateUpdate()のタイミングで呼ばれます
int RotateExecutor::LateUpdate(bool triggerSignal)
{
    // 
    return 0;
}

// ギミック内容を描画する関数
// ※既に実行段階である場合は引数がtrueになります。実行段階では描画しない、あるいはその逆の場合はこの引数を使ってください。
int RotateExecutor::Draw(bool triggerSignal)
{
    // 
    return 0;
}
