#pragma once

#include "BaseScene.h"
#include "../D_UI/QuadrangleButton2D.h"

class Model2D;

class TitleScene : public BaseScene
{
private:
    int mnCorsorNum;
    QuadrangleButton2D mdButtonGameStart;
    QuadrangleButton2D mdButtonNetwork;
    QuadrangleButton2D mdButtonQuit;

public:
    TitleScene();
    ~TitleScene() override;

    int Create() override;
    int Initialize() override;
    int Finalize() override;
    int Update() override;
    int Draw() override;
};