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
        delete data.mpChecker;
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
        this->mlGimmickList[i].mbSumilating = this->mlGimmickList[i].mpChecker->GetSignalSumilate();

        // 
        if (this->mlGimmickList[i].mpSumilater->CheckSumilate(this->mlGimmickList[i].mbSumilating))
        {
            // 
            this->mlGimmickList[i].mpSumilater->Sumilate();
        }

        // 
        this->mlGimmickList[i].mbExecuting = this->mlGimmickList[i].mpChecker->GetSignalExecute();

        // ここでシミュレートの内容を実行に移す処理
        if (this->mlGimmickList[i].mpSumilater->CheckExecute(this->mlGimmickList[i].mbExecuting))
        {
            // 
            this->mlGimmickList[i].mpSumilater->Execute();
        }
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
        // ここでシミュレートの内容を実行に移す処理
        if (this->mlGimmickList[i].mpSumilater->Draw(this->mlGimmickList[i].mbSumilating, this->mlGimmickList[i].mbExecuting) != 0)
        {
            // 
            return -1;
        }
    }

    // 
    return 0;
}

// 
bool GimmickObjectController::AddGimmick(const PuzzleGimmickData &gimmick)
{
    // 
    if (gimmick.mpChecker == nullptr ||
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
