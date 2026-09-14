#pragma once
#include "BaseScene.h"

class Player;

class TestScene : public BaseScene
{
private:
    Player* mpPlayer;

public:

    TestScene();
    ~TestScene();

    int Create() override;
    int Initialize() override;
    int Finalize() override;
    int Update() override;
    int Draw() override;
};