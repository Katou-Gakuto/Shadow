#pragma once

#include "BaseModel.h"
#include "../Y_Tool/VECTOR.h"
#include <map>
#include <string>

class GameObject3D;
class PartsBox;
class PartsAnimator;
class ModelPartsBox : BaseModel
{
    std::map<std::string, PartsBox *> mlPartsList;
    std::string msRootPartsName;
    PartsAnimator *mpAnimator;

public:
    ModelPartsBox(std::string rootPartsName, GameObject3D *up, ScreenNumber drawScreen);
    ~ModelPartsBox() override;

    int Initialize() override;
    int Finalize() override;
    int Update() override;
    int Draw() override;

    int AddPartsBox(std::string partsName, PartsBox *partsBox);
    PartsBox *GetPartsBox(std::string partsName);
    
    void SetAnimator(PartsAnimator *animator);

    std::string GetRootPartsName() const;
    PartsAnimator *GetAnimator() const;
};