#pragma once

#include <cstdint>
#include <vector>

#include "../Y_Tool/VECTOR.h"

// 
class LightArea;

// 
class LightAreaManager
{
public:
    // 
    LightAreaManager();

    // 
    ~LightAreaManager();

    // 
    int AddArea(LightArea *newArea);

    // 
    int DeleteArea(int areaIndex);

    // 
    LightArea *SearchArea(int areaIndex);

    // どの光域に入っているかを確認する関数
    // 返り値のintは0で処理の成功の可否、引数のoutは自身が入っているAreaIndexを返す関数
    int CheckInLightArea(const VECTOR2D &pos, std::vector<int> &out);

private:
    // 
    std::vector<LightArea *> mlNode;

    // 
    uint16_t mnDeactivateStartIndex;
};
