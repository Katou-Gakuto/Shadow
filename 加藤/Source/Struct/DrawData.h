#pragma once
#include <vector>

#include "BitFlag.h"
#include "ResourceData.h"

enum class DRAW_MANAGER_DRAW_TYPE
{
    GRAPH = 0,
};

struct DRAW_DATA
{
    int drawID; // 描画ID

    bool drawFlag = true;

    DRAW_MANAGER_DRAW_TYPE drawManagerDrawType; // 描画マネージャー描画種類

    DRAW_GRAPH_DATA drawGraphData;  // 画像描画情報

    DRAW_CONFIG_DATA drawConfigData;    // 描画設定情報
};


// インデックス情報
struct IndexData
{
    unsigned short v1; // 頂点1
    unsigned short v2; // 頂点2
    unsigned short v3; // 頂点3
};

// 頂点情報
struct IndexedData
{
    std::vector<VERTEX3D> vertex;// 頂点情報
    std::vector<IndexData> index; // インデックス情報
    int textureHandle; // テクスチャハンドル
    BIT_FLAG<unsigned char> textureType; // テクスチャ種類
    int transFlag;    // 透過フラグ
};