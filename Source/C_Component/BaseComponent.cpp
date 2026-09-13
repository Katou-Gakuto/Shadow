#include "BaseComponent.h"
#include "BaseComponentList.h"

#include "../A_GameObject/GameObject.h"
#include "../A_GameObject/GameObject2D.h"
#include "../A_GameObject/GameObject3D.h"

BaseComponent::BaseComponent(GameObject *myObject, ComponentTagAndOrder tagOrder) :
    mnTagOrder(tagOrder),
    mbDeleteFlag(false),
    mbActiveFlag(true),
    mbCreateFlag(false),
    mbInitializedFlag(false),
    mpUp(myObject),
    mpPrev(nullptr),
    mpNext(nullptr)
{

}

BaseComponent::~BaseComponent()
{

}

int BaseComponent::Create()
{
    return  0;
}

int BaseComponent::Initialize()
{
    return 0;
}

int BaseComponent::Finalize()
{
    this->mbInitializedFlag = false;
    return 0;
}

int BaseComponent::EarlyUpdate()
{
    return 0;
}

int BaseComponent::Update()
{
    return 0;
}

int BaseComponent::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    return 0;
}

int BaseComponent::LateUpdate()
{
    return 0;
}

int BaseComponent::Draw()
{
    return 0;
}

int BaseComponent::BaseInitialize()
{
    int temp = 0;
    if (this->mbCreateFlag == false)
    {
        temp = this->Create();
        this->mbCreateFlag = true;
    }
    if (temp == 0 && this->mbInitializedFlag == false)
    {
        temp = this->Initialize();
        this->mbInitializedFlag = true;
    }
    return temp;
}

void BaseComponent::OnDeleteFlag()
{
    this->mbDeleteFlag = true;
}

void BaseComponent::SetPrevNext(BaseComponent *prev, BaseComponent *next)
{
    this->mpPrev = prev;
    this->mpNext = next;
}

ComponentTagAndOrder BaseComponent::GetTagAndOrder() const
{
    return this->mnTagOrder;
}

bool BaseComponent::GetDeleteFlag() const
{
    return this->mbDeleteFlag;
}

bool BaseComponent::GetActiveFlag() const
{
    return this->mbActiveFlag;
}

bool BaseComponent::GetInitializedFlag() const
{
    return this->mbInitializedFlag;
}

GameObject *BaseComponent::GetMyObject() const
{
    return this->mpUp;
}

GameObject2D *BaseComponent::GetMyObject2D()
{
    return static_cast<GameObject2D *>(this->mpUp);
}

GameObject3D *BaseComponent::GetMyObject3D()
{
    return static_cast<GameObject3D *>(this->mpUp);
}

BaseComponent *BaseComponent::GetPrev() const
{
    return this->mpPrev;
}

BaseComponent *BaseComponent::GetNext() const
{
    return this->mpNext;
}
