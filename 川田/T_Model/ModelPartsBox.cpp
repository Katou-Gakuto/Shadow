#include "ModelPartsBox.h"

#include "PartsBox.h"
#include "PartsAnimator.h"
#include "../A_GameObject/GameObject3D.h"

ModelPartsBox::ModelPartsBox(std::string rootPartsName, GameObject3D *up, ScreenNumber drawScreen) :
    mlPartsList(),
    msRootPartsName(rootPartsName),
    mpAnimator(nullptr),
    BaseModel(up, ModelType::ModelType_ModelPartsBox, drawScreen)
{

}

ModelPartsBox::~ModelPartsBox()
{

}

int ModelPartsBox::Initialize()
{
    return 0;
}

int ModelPartsBox::Finalize()
{
    return 0;
}

int ModelPartsBox::Update()
{
    if (this->mpAnimator != nullptr)
    {
        this->mpAnimator->Update();
    }
    for (auto currentParts : this->mlPartsList)
    {
        currentParts.second->Transform();
    }

    return 0;
}

int ModelPartsBox::Draw()
{
    for (auto currentParts : this->mlPartsList)
    {
        currentParts.second->Draw();
    }

    return 0;
}

int ModelPartsBox::AddPartsBox(std::string partsName, PartsBox *partsBox)
{
    this->mlPartsList[partsName] = partsBox;
    return 0;
}

PartsBox *ModelPartsBox::GetPartsBox(std::string partsName)
{
    PartsBox *temp = nullptr;

    // 指定した要素が存在するか、イテレーターを使って確かめている
    auto it = this->mlPartsList.find(partsName);

    // 指定した要素が存在しているなら、tempの中身をその要素にする
    if (it != this->mlPartsList.end())
    {
        temp = this->mlPartsList[partsName];
    }
    return temp;
}

void ModelPartsBox::SetAnimator(PartsAnimator *animator)
{
    this->mpAnimator = animator;
}

std::string ModelPartsBox::GetRootPartsName() const
{
    return this->msRootPartsName;
}

PartsAnimator *ModelPartsBox::GetAnimator() const
{
    return this->mpAnimator;
}