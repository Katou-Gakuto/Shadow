#include "SoundVisualizerController.h"
#include "../A_GameObject/SoundVisualizer.h"

SoundVisualizer *SoundVisualizerController::GetMyVisual()
{
    return static_cast<SoundVisualizer *>(this->GetMyObject());
}

SoundVisualizerController::SoundVisualizerController(GameObject *myObject) :
    BaseComponent(myObject, CTAO_SoundVisualizerController)
{

}

SoundVisualizerController::~SoundVisualizerController()
{

}

int SoundVisualizerController::Update()
{
    int temp = 0;
    auto obj = this->GetMyVisual();
    if (obj != nullptr)
    {
        temp = obj->ObjectUpdate();
    }
    return temp;
}

int SoundVisualizerController::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    int temp = 0;
    auto obj = this->GetMyVisual();
    if (obj != nullptr)
    {
        temp = obj->ObjectHitOnCollision(myCollision, hitCollision);
    }
    return temp;
}

int SoundVisualizerController::LateUpdate()
{
    int temp = 0;
    auto obj = this->GetMyVisual();
    if (obj != nullptr)
    {
        temp = obj->ObjectLateUpdate();
    }
    return temp;
}
