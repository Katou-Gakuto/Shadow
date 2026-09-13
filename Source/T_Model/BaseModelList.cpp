#include "BaseModelList.h"

#include "BaseModel.h"

int BaseModelList::DeleteAll()
{
    if (this->mpFirstModel == nullptr)
    {
        return 0;
    }
    BaseModel *current = this->mpFirstModel;
    BaseModel *next = nullptr;
    while (current != nullptr)
    {
        next = current->GetNext();
        if (next != nullptr)
        {
            next->SetPrevNext(nullptr, next->GetNext());
        }
        current->SetPrevNext(nullptr, nullptr);
        delete current;
        current = next;
    }
    this->mpFirstModel = nullptr;
    return 0;
}

BaseModelList::BaseModelList() :
    mpFirstModel(nullptr)
{

}

BaseModelList:: ~BaseModelList()
{
    this->DeleteAll();
}

int BaseModelList::Initialize()
{
    int temp = 0;
    BaseModel *current = this->mpFirstModel;
    while (current != nullptr)
    {
        if (current->GetDrawFlag())
        {
            temp = current->Initialize();
        }
        if (temp == 0)
        {
            current = current->GetNext();
        }
        else
        {
            break;
        }
    }
    return temp;
}

int BaseModelList::Finalize()
{
    int temp = 0;
    BaseModel *current = this->mpFirstModel;
    while (current != nullptr)
    {
        if (current->GetDrawFlag())
        {
            temp = current->Finalize();
        }
        if (temp == 0)
        {
            current = current->GetNext();
        }
        else
        {
            break;
        }
    }
    return temp;
}

int BaseModelList::Draw()
{
    int temp = 0;
    BaseModel *current = this->mpFirstModel;
    while (current != nullptr)
    {
        if (current->GetDrawFlag())
        {
            temp = current->BaseDraw();
        }
        if (temp == 0)
        {
            current = current->GetNext();
        }
        else
        {
            break;
        }
    }
    return temp;
}

int BaseModelList::Add(BaseModel *model, unsigned long listNumber)
{
    if (model == nullptr)
    {
        return -1;
    }
    if (this->mpFirstModel == nullptr)
    {
        this->mpFirstModel = model;
        return 0;
    }
    int temp = 0;
    BaseModel *current = this->mpFirstModel;
    while (current->GetNext() != nullptr)
    {
        if (current->GetModelNumber() == listNumber)
        {
            temp = -1;
            break;
        }
        current = current->GetNext();
    }
    if (temp != 0)
    {
        return temp;
    }
    current->SetPrevNext(current->GetPrev(), model);
    model->SetPrevNext(current, nullptr);
    model->SetModelNumber(listNumber);
    return temp;
}

BaseModel *BaseModelList::SearchModelNumber(unsigned long listNumber)
{
    BaseModel *current = this->mpFirstModel;
    while (current != nullptr)
    {
        if (current->GetModelNumber() == listNumber)
        {
            break;
        }
        current = current->GetNext();
    }
    return current;
}
