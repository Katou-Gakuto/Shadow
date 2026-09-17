#include "FreeMoveExecutor.h"

#include "../A_GameObject/GameObject2D.h"

#include "../S_Collision/BaseCollisionList.h"

// 
FreeMoveExecutor::FreeMoveExecutor(PuzzleGimmickActiveParam param, GameObject2D *myObject) :
    mvMoveVec(),
    mpMyObject(myObject),
    BaseGimmickExecutor(param)
{
}

// 
FreeMoveExecutor::~FreeMoveExecutor()
{
}

// ギミックの内容を実行する関数
// ※GameObject::EarlyUpdate()のタイミングで呼ばれます
int FreeMoveExecutor::EarlyUpdate(bool triggerSignal)
{
    // 
    return 0;
}

// ギミックの内容を実行する関数
// ※GameObject::Update()のタイミングで呼ばれます
int FreeMoveExecutor::Update(bool triggerSignal)
{
    // 
    if (this->mpMyObject == nullptr)
    {
        // 
        return -1;
    }

    // 
    BaseCollisionList *list = this->mpMyObject->GetBaseCollisionList();
    if (list == nullptr)
    {
        // 
        return -1;
    }
    
    // 
    this->mpMyObject->SetMoveVec(this->mvMoveVec);

    // 
    list->SetCollisionMoveVec(CollisionDimension::CollisionDimension_2D, &this->mvMoveVec);

    // 
    return 0;
}

// ギミックの内容を実行する関数
// ※GameObject::LateUpdate()のタイミングで呼ばれます
int FreeMoveExecutor::LateUpdate(bool triggerSignal)
{
    // 
    if (this->GetActiveParam().GetSignalNot())
    {
        triggerSignal = !triggerSignal;
    }

    // 
    if (!triggerSignal)
    {
        // 
        return 0;
    }

    // 
    return 0;
}

// ギミック内容を描画する関数
// ※既に実行段階である場合は引数がtrueになります。実行段階では描画しない、あるいはその逆の場合はこの引数を使ってください。
int FreeMoveExecutor::Draw(bool triggerSignal)
{
    // 
    return 0;
}

// 
void FreeMoveExecutor::SetMoveVec(const VECTOR2D &vec)
{
    // 
    this->mvMoveVec = vec;
}
