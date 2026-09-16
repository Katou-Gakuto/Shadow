#include "LightAreaManager.h"

#include "LightArea.h"

// 
LightAreaManager::LightAreaManager() :
    mlNode(),
    mnDeactivateStartIndex(0)
{
}

// 
LightAreaManager::~LightAreaManager()
{
    // 
    for (auto node : this->mlNode)
    {
        // 
        if (node == nullptr)
        {
            continue;
        }

        // 
        delete node;

        // 
        node = nullptr;
    }

    // 
    this->mlNode.clear();

    // 
    this->mnDeactivateStartIndex = 0;
}

// 
int LightAreaManager::AddArea(LightArea *newArea)
{
    // 
    if (newArea == nullptr)
    {
        // 
        return -1;
    }

    // 
    int addIndex = this->mnDeactivateStartIndex;

    // 
    this->mlNode[addIndex] = newArea;

    // 
    this->mnDeactivateStartIndex++;

    // 
    return addIndex;
}

// 
int LightAreaManager::DeleteArea(int areaIndex)
{
    // 
    if (this->mlNode[areaIndex] == nullptr)
    {
        // 
        return 0;
    }

    // 
    delete this->mlNode[areaIndex];

    // 
    this->mlNode[areaIndex] = nullptr;

    // 
    for (uint16_t i = this->mnDeactivateStartIndex - 1; areaIndex < i; i--)
    {
        // 
        this->mlNode[i - 1] = this->mlNode[i];
    }

    // 
    this->mnDeactivateStartIndex--;

    // 
    return 0;
}

// 
LightArea *LightAreaManager::SearchArea(int areaIndex)
{
    // 
    if (areaIndex < 0 ||
        this->mnDeactivateStartIndex <= areaIndex)
    {
        // 
        return nullptr;
    }

    // 
    return this->mlNode[areaIndex];
}

// どの光域に入っているかを確認する関数
// 返り値のintは0で処理の成功の可否、引数のoutは自身が入っているAreaIndexを返す関数
int LightAreaManager::CheckInLightArea(const VECTOR2D &pos, std::vector<int> &out)
{
    // 
    for (uint16_t i = 0; i < this->mnDeactivateStartIndex; i++)
    {
        // 
        if (this->mlNode[i]->CheckInArea(pos))
        {
            // 
            out.push_back(i);
        }
    }

    // 
    return 0;
}
