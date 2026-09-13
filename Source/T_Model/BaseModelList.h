#pragma once

class BaseModel;
class BaseModelList
{
private:
    BaseModel *mpFirstModel;

    int DeleteAll();

public:
    BaseModelList();
    ~BaseModelList();

    int Initialize();
    int Finalize();
    int Draw();

    int Add(BaseModel *model, unsigned long listNumber);

    BaseModel *SearchModelNumber(unsigned long listNumber);
};