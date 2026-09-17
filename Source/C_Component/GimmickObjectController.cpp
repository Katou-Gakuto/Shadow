#include "GimmickObjectController.h"

// 
GimmickObjectController::GimmickObjectController(GameObject *myObject) :
    BaseComponent(myObject, ComponentTagAndOrder::CTAO_GimmickController)
{
}

//
GimmickObjectController::~GimmickObjectController()
{
    // 
    for (auto &data : this->mlGimmickList)
    {
        delete data.mpTrigger;
        delete data.mpSumilater;
    }

    this->mlGimmickList.clear();
}

// 
int GimmickObjectController::Create()
{
    // 
    return 0;
}

// 
int GimmickObjectController::Initialize()
{
    // 
    return 0;
}

int GimmickObjectController::Finalize()
{
    // 
    return 0;
}

int GimmickObjectController::EarlyUpdate()
{
    // 
    return 0;
}

int GimmickObjectController::Update()
{
    // 
    for (uint32_t i = 0; i < this->mlGimmickList.size(); i++)
    {
        // 
        this->mlGimmickList[i].mbTriggerSignal = this->mlGimmickList[i].mpTrigger->GetSignal();

        // 
        this->mlGimmickList[i].mpSumilater->Update((this->mlGimmickList[i].mbTriggerSignal));
    }

    // 
    return 0;
}

int GimmickObjectController::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    // 
    return 0;
}

int GimmickObjectController::LateUpdate()
{
    // 
    return 0;
}

int GimmickObjectController::Draw()
{
    // 
    for (uint32_t i = 0; i < this->mlGimmickList.size(); i++)
    {
        // 
        this->mlGimmickList[i].mbTriggerSignal = this->mlGimmickList[i].mpTrigger->GetSignal();

        // 
        this->mlGimmickList[i].mpSumilater->Draw((this->mlGimmickList[i].mbTriggerSignal));
    }

    // 
    return 0;
}

// 
bool GimmickObjectController::AddGimmick(const PuzzleGimmickData &gimmick)
{
    // 
    if (gimmick.mpTrigger == nullptr ||
        gimmick.mpSumilater == nullptr)
    {
        // 
        return false;
    }

    // 
    this->mlGimmickList.push_back(gimmick);

    // 
    return true;
}
