#include "GameObject.h"
#include "GameObjectManager.h"
#include "../B_BehaviorTree/BehaviorTree.h"
#include "../C_Component/BaseComponentList.h"
#include "../T_Model/BaseModelList.h"
#include "../S_Collision/BaseCollisionList.h"
#include "../Z_Except/Master.h"

void GameObject::OnModelCreateFlag()
{
    this->mcBitFlag |= GOBF::GOBF_ModelCreate;
}

void GameObject::OnCollisionListCreateFlag()
{
    this->mcBitFlag |= GOBF::GOBF_CollisionListCreate;
}

void GameObject::OnBehaviorCreateFlag()
{
    this->mcBitFlag |= GOBF::GOBF_BehaviorCreate;
}

void GameObject::OnComponentCreateFlag()
{
    this->mcBitFlag |= GOBF::GOBF_ComponentCreate;
}

void GameObject::SetDeleteBit(bool flag)
{
    if (flag)
    {
        this->mcBitFlag |= GOBF::GOBF_DeleteFlag;
    }
    else
    {
        this->mcBitFlag &= ~(1 << GOBF::GOBF_DeleteFlag);
    }
}

void GameObject::SetActiveBit(bool flag)
{
    if (flag)
    {
        this->mcBitFlag |= GOBF::GOBF_ActiveFlag;
    }
    else
    {
        this->mcBitFlag &= ~(1 << GOBF::GOBF_ActiveFlag);
    }
}

void GameObject::SetStopBit(bool flag)
{
    if (flag)
    {
        this->mcBitFlag |= GOBF::GOBF_StopFlag;
    }
    else
    {
        this->mcBitFlag &= ~(1 << GOBF::GOBF_StopFlag);
    }
}

void GameObject::SetDrawBit(bool flag)
{
    if (flag)
    {
        this->mcBitFlag |= GOBF::GOBF_DrawFlag;
    }
    else
    {
        this->mcBitFlag &= ~(1 << GOBF::GOBF_DrawFlag);
    }
}

GameObject::GameObject(GameObjectTag tag) :
    mnObjectTag(tag),
    mbCreateFlag(),
    mcBitFlag(0),
    mdInitBehavior(GetBehaviorAccessDataZero()),
    mpModelList(nullptr),
    mpBaseCollisionList(nullptr),
    mpBehaviorTree(nullptr),
    mpBaseComponentList(nullptr),
    mpPrev(nullptr),
    mpNext(nullptr)
{
    SetDeleteBit(false);
    SetActiveBit(true);
    SetStopBit(false);
    SetDrawBit(true);
}

GameObject::~GameObject()
{
    if (this->mpModelList != nullptr)
    {
        delete this->mpModelList;
        this->mpModelList = nullptr;
    }
    if (this->mpBaseCollisionList != nullptr)
    {
        delete this->mpBaseCollisionList;
        this->mpBaseCollisionList = nullptr;
    }
    if (this->mpBehaviorTree != nullptr)
    {
        delete this->mpBehaviorTree;
        this->mpBehaviorTree = nullptr;
    }
    if (this->mpBaseComponentList != nullptr)
    {
        delete this->mpBaseComponentList;
        this->mpBaseComponentList = nullptr;
    }
}

int GameObject::Create()
{
    return 0;
}

int GameObject::Initialize()
{
    int temp = 0;
    if (this->mbCreateFlag.GetFlag())
    {
        temp = this->Create();
        this->mbCreateFlag.CreateComplate();
    }
    if (temp != 0)
    {
        return temp;
    }
    if (this->mpModelList != nullptr)
    {
        temp = this->mpModelList->Initialize();
    }
    if (temp != 0)
    {
        return temp;
    }
    if (this->mpBaseCollisionList != nullptr)
    {
        temp = this->mpBaseCollisionList->Initialize();
    }
    if (temp != 0)
    {
        return temp;
    }
    if (this->mpBehaviorTree != nullptr)
    {
        temp = this->mpBehaviorTree->Initialize(this->mdInitBehavior);
    }
    if (temp != 0)
    {
        return temp;
    }
    if (this->mpBaseComponentList != nullptr)
    {
        temp = this->mpBaseComponentList->Initialize();
    }
    return temp;
}

int GameObject::Finalize()
{
    int temp = 0;
    if (this->mpModelList != nullptr)
    {
        temp = this->mpModelList->Finalize();
    }
    if (temp != 0)
    {
        return temp;
    }
    if (this->mpBaseCollisionList != nullptr)
    {
        temp = this->mpBaseCollisionList->Finalize();
    }
    if (temp != 0)
    {
        return temp;
    }
    if (this->mpBehaviorTree != nullptr)
    {
        temp = this->mpBehaviorTree->Finalize();
    }
    if (temp != 0)
    {
        return temp;
    }
    if (this->mpBaseComponentList != nullptr)
    {
        temp = this->mpBaseComponentList->Finalize();
    }
    return temp;
}

int GameObject::CollisionActivate()
{
    if (this->mpBaseCollisionList != nullptr)
    {
        this->mpBaseCollisionList->ListCollisionActivate();
    }
    return 0;
}

int GameObject::CollisionDeactivate()
{
    if (this->mpBaseCollisionList != nullptr)
    {
        this->mpBaseCollisionList->ListCollisionDeactivate();
    }
    return 0;
}

// 通常更新の前にしなければいけない何か
int GameObject::EarlyUpdate()
{
    if (this->GetActiveFlag() == false ||
        this->GetStopFlag() == true)
    {
        return 0;
    }

    int temp = 0;
    if (this->mpBaseComponentList != nullptr)
    {
        temp = this->mpBaseComponentList->EarlyUpdate();
    }
    if (temp != 0)
    {
        return temp;
    }
    if (this->mpBehaviorTree != nullptr)
    {
        temp = this->mpBehaviorTree->EarlyUpdate();
    }
    return temp;
}

// 通常更新
int GameObject::Update()
{
    if (this->GetActiveFlag() == false ||
        this->GetStopFlag() == true)
    {
        return 0;
    }

    int temp = 0;
    if (this->mpBaseComponentList != nullptr)
    {
        temp = this->mpBaseComponentList->Update();
    }
    if (temp != 0)
    {
        return temp;
    }
    if (this->mpBehaviorTree != nullptr)
    {
        temp = this->mpBehaviorTree->Update();
    }
    return temp;
}

// 当たり判定計算でオブジェクトと当たった場合の処理
int GameObject::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    if (this->GetActiveFlag() == false ||
        this->GetStopFlag() == true)
    {
        return 0;
    }

    int temp = 0;
    if (this->mpBaseComponentList != nullptr)
    {
        temp = this->mpBaseComponentList->HitOnCollision(myCollision, hitCollision);
    }
    if (temp != 0)
    {
        return temp;
    }
    if (this->mpBehaviorTree != nullptr)
    {
        temp = this->mpBehaviorTree->HitOnCollision(myCollision, hitCollision);
    }
    return temp;
}

// 当たり判定計算後の更新
int GameObject::LateUpdate()
{
    if (this->GetActiveFlag() == false ||
        this->GetStopFlag() == true)
    {
        return 0;
    }

    int temp = 0;
    if (this->mpBaseComponentList != nullptr)
    {
        temp = this->mpBaseComponentList->LateUpdate();
    }
    if (temp != 0)
    {
        return temp;
    }
    if (this->mpBehaviorTree != nullptr)
    {
        temp = this->mpBehaviorTree->LateUpdate();
    }
    return temp;
}

// 描画関連の関数を実行する関数
int GameObject::ObjectDraw()
{
    int temp = 0;
    temp = this->Draw();
    if (temp != 0)
    {
        return temp;
    }
    return this->UniqueDraw();
}

int GameObject::Draw()
{
    if (this->GetActiveFlag() == false ||
        this->GetDrawFlag() == false)
    {
        return 0;
    }

    int temp = 0;
    if (this->mpModelList != nullptr)
    {
        this->mpModelList->Draw();
    }
    if (this->mpBaseComponentList != nullptr)
    {
        this->mpBaseComponentList->Draw();
    }
    return temp;
}

int GameObject::UniqueDraw()
{
    return 0;
}

void GameObject::OnDeleteFlag()
{
    this->SetDeleteBit(true);
}

void GameObject::SetActiveFlag(bool active)
{
    this->SetActiveBit(active);
}

void GameObject::SetStopFlag(bool stop)
{
    this->SetStopBit(stop);
}

void GameObject::SetDrawFlag(bool draw)
{
    this->SetDrawBit(draw);
}

int GameObject::DeleteToFlagMember()
{
    int temp = 0;
    if (this->mpBaseCollisionList != nullptr)
    {
        temp = this->mpBaseCollisionList->DeleteToFlag();
    }
    if (temp != 0)
    {
        return temp;
    }
    if (this->mpBaseComponentList != nullptr)
    {
        temp = this->mpBaseComponentList->DeleteToFlag();
    }
    return temp;
}

int GameObject::UpdateModel()
{
    int temp = 0;
    if (this->mpModelList != nullptr && this->mpBehaviorTree != nullptr)
    {
        temp = this->mpBehaviorTree->UpdateModel(this->mpModelList);
    }
    return temp;
}

int GameObject::CreateObjectModelList()
{
    if (this->GetModelCreateFlag())
    {
        return 1;
    }
    this->mpModelList = new BaseModelList();
    this->OnModelCreateFlag();
    return 0;
}

int GameObject::CreateObjectCollisionList()
{
    if (this->GetCollisionListCreateFlag())
    {
        return 1;
    }
    this->mpBaseCollisionList = new BaseCollisionList();
    this->OnCollisionListCreateFlag();
    return 0;
}

int GameObject::CreateObjectBehavior(const BehaviorAccessData &initAccessData)
{
    if (this->GetBehaviorCreateFlag())
    {
        return 1;
    }
    this->mpBehaviorTree = new BehaviorTree();
    this->mdInitBehavior = initAccessData;
    this->OnBehaviorCreateFlag();
    return 0;
}

int GameObject::CreateObjectComponentList()
{
    if (this->GetComponentCreateFlag())
    {
        return 1;
    }
    this->mpBaseComponentList = new BaseComponentList();
    this->OnComponentCreateFlag();
    return 0;
}

void GameObject::SetInitBehavior(const BehaviorAccessData &accessData)
{
    this->mdInitBehavior = accessData;
}

GameObjectTag GameObject::GetObjectTag() const
{
    return this->mnObjectTag;
}

bool GameObject::GetModelCreateFlag() const
{
    return ((this->mcBitFlag & GOBF::GOBF_ModelCreate) != 0);
}

bool GameObject::GetCollisionListCreateFlag() const
{
    return ((this->mcBitFlag & GOBF::GOBF_CollisionListCreate) != 0);
}

bool GameObject::GetBehaviorCreateFlag() const
{
    return ((this->mcBitFlag & GOBF::GOBF_BehaviorCreate) != 0);
}

bool GameObject::GetComponentCreateFlag() const
{
    return ((this->mcBitFlag & GOBF::GOBF_ComponentCreate) != 0);
}

bool GameObject::GetDeleteFlag() const
{
    return ((this->mcBitFlag & GOBF::GOBF_DeleteFlag) != 0);
}

bool GameObject::GetActiveFlag() const
{
    return ((this->mcBitFlag & GOBF::GOBF_ActiveFlag) != 0);
}

bool GameObject::GetStopFlag() const
{
    return ((this->mcBitFlag & GOBF::GOBF_StopFlag) != 0);
}

bool GameObject::GetDrawFlag() const
{
    return ((this->mcBitFlag & GOBF::GOBF_DrawFlag) != 0);
}

BaseModelList *GameObject::GetModelList()const
{
    return this->mpModelList;
}

BaseCollisionList *GameObject::GetBaseCollisionList()const
{
    return this->mpBaseCollisionList;
}

BehaviorTree *GameObject::GetBehaviorTree()const
{
    return this->mpBehaviorTree;
}

BaseComponentList *GameObject::GetBaseComponentList()const
{
    return this->mpBaseComponentList;
}

int GameObject::AddModel(BaseModel *model, unsigned long number)
{
    if (model == nullptr ||
        this->mpModelList == nullptr)
    {
        return -1;
    }
    return this->mpModelList->Add(model, number);
}

int GameObject::AddCollision(BaseCollision *collision, unsigned long number)
{
    if (collision == nullptr ||
        this->mpBaseCollisionList == nullptr)
    {
        return -1;
    }
    return this->mpBaseCollisionList->Add(collision, number);
}

int GameObject::AddBehaviorNode(BehaviorNode *node, unsigned long number)
{
    if (node == nullptr ||
        this->mpBehaviorTree == nullptr)
    {
        return -1;
    }
    return this->mpBehaviorTree->AddNode(node, number);
}

int GameObject::AddMonoBehavior(MonoBehavior *mono, const BehaviorAccessData &accessData)
{
    if (mono == nullptr ||
        this->mpBehaviorTree == nullptr)
    {
        return -1;
    }
    return this->mpBehaviorTree->AddMono(mono, accessData);
}

int GameObject::AddComponent(BaseComponent *component)
{
    if (component == nullptr ||
        this->mpBaseComponentList == nullptr)
    {
        return -1;
    }
    return this->mpBaseComponentList->Add(component);
}

void GameObject::SetPrev(GameObject *prev)
{
    this->mpPrev = prev;
}

void GameObject::SetNext(GameObject *next)
{
    this->mpNext = next;
}

GameObject *GameObject::GetPrev() const
{
    return this->mpPrev;
}

GameObject *GameObject::GetNext() const
{
    return this->mpNext;
}
