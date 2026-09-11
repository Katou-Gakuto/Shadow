#pragma once

#include "BaseModel.h"
#include "../Y_Tool/VECTOR.h"

class GameObject3D;
class Model3D : BaseModel
{
    VECTOR3D mvPosition;


public:
    Model3D(GameObject3D *myObject, ScreenNumber drawScreen);
    ~Model3D() override;

    int Initialize() override;
    int Finalize() override;
    int Update() override;
    int Draw() override;
};