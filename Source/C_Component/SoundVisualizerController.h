#pragma once

#include "BaseComponent.h"

class SoundVisualizer;
class SoundVisualizerController : public BaseComponent
{
private:
    SoundVisualizer *GetMyVisual();

public:
    SoundVisualizerController(GameObject *myObject);
    ~SoundVisualizerController() override;

    int Update() override;
    int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision) override;
    int LateUpdate() override;
};