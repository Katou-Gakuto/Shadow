#pragma once

#include "BaseScene.h"

class Player;
class LightAreaManager;

class PuzzleScene : public BaseScene
{
public:
    PuzzleScene();
    ~PuzzleScene();

    int Create() override;
    int Initialize() override;
    int Finalize() override;
    int Update() override;
    int Draw() override;
};