#include "BaseModel.h"

#include "../A_GameObject/GameObject.h"
#include "BaseModelList.h"

#include "../V_Display/System.h"
#include "../Z_Except/Master.h"

void BaseModel::SetDrawBit(bool flag)
{
    if (flag)
    {

    }
    else
    {

    }
}

BaseModel::BaseModel(GameObject *up, ModelType modelType, ScreenNumber drawScreen) :
    mnModelType(modelType),
    mnDrawScreen(drawScreen),
    mcBitFlag(0),
    mnModelNumber(0),
    mpUp(up),
    mpPrev(nullptr),
    mpNext(nullptr)
{

}

BaseModel::~BaseModel()
{

}

int BaseModel::Initialize()
{
    return 0;
}

int BaseModel::Finalize()
{
    return 0;
}

int BaseModel::Update()
{
    return 0;
}

int BaseModel::Draw()
{
    return 0;
}

int BaseModel::BaseDraw()
{
    int temp = 0;
    if (Master::mpSystem->ChangeDrawScreen(this->mnDrawScreen) == 0)
    {
        temp = this->Draw();
    }
    return temp;
}

void BaseModel::SetDrawFlag(bool flag)
{
    this->SetDrawBit(flag);
}

void BaseModel::SetModelNumber(unsigned long number)
{
    this->mnModelNumber = number;
}

ModelType BaseModel::GetModeltype() const
{
    return this->mnModelType;
}

bool BaseModel::GetDrawFlag() const
{
    return ((this->mcBitFlag | BMBF::BMBF_DrawFlag) != 0);
}

unsigned long BaseModel::GetModelNumber()const
{
    return this->mnModelNumber;
}

GameObject *BaseModel::GetUp() const
{
    return this->mpUp;
}

void BaseModel::SetPrevNext(BaseModel *prev, BaseModel *next)
{
    this->mpPrev = prev;
    this->mpNext = next;
}

BaseModel *BaseModel::GetPrev() const
{
    return this->mpPrev;
}

BaseModel *BaseModel::GetNext() const
{
    return this->mpNext;
}
