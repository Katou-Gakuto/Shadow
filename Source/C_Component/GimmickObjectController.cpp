#include "GimmickObjectController.h"

// 
GimmickObjectController::GimmickObjectController(GameObject *myObject) :
    BaseComponent(myObject, ComponentTagAndOrder::CTAO_GimmickController)
{
}

//
GimmickObjectController::~GimmickObjectController()
{

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
        bool checkerResult = this->mlGimmickList[i].mpChecker->CheckSumilate();

        // 
        if (this->mlGimmickList[i].mpSumilater->CheckSumilate(checkerResult))
        {
            // 
            this->mlGimmickList[i].mpSumilater->Sumilate();
        }

        // 
        checkerResult = this->mlGimmickList[i].mpChecker->CheckExecute();

        // ここでシミュレートの内容を実行に移す処理
        if (this->mlGimmickList[i].mpSumilater->CheckExecute(checkerResult))
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
