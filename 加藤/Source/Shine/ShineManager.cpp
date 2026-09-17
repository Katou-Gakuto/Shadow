#include <stack>
#include <queue>
#include <vector>

#include "Vector2.h"

#include "DxLib.h"

#include "Master.h"

#include "ShineManager.h"
#include "ShineObject.h"
#include "TestObjectBase.h"
#include "WallObject.h"
#include "ImguiManager.h"

ShineManager::ShineManager()
: mpObjects()
, mstSheineTriangles()
, mpShineObject(nullptr)
, mnDrawMode(2)
{
}

ShineManager::~ShineManager()
{
}

void ShineManager::Init()
{
    mpObjects.clear();

    // ここで生成
    WallObject* wallObject = new WallObject();
    wallObject->SetPosition(Vector2(1280 * 0.7f, 960 * 0.7f));
    wallObject->SetSize(Vector2(1280 * 0.05f, 960 * 0.05f));
    mpObjects.push_back(wallObject);
    wallObject = new WallObject();
    wallObject->SetPosition(Vector2(1280 * 0.5f, 960 * 0.5f));
    wallObject->SetSize(Vector2(1280 * 0.05f, 960 * 0.05f));
    mpObjects.push_back(wallObject);

    mpShineObject = new ShineObject();
    mpShineObject->Init();

    for (int y = 0; y < MAP_ARRAY_SIZE_Y; ++y)
    {
        for (int x = 0; x < MAP_ARRAY_SIZE_X; ++x)
        {
            mstMapObjectGridData[y][x].LinePoss.clear();
            mstMapObjectGridData[y][x].LitFlag = false;
        }
    }

    // 生成したオブジェクト初期化
    for (auto& object : mpObjects)
    {
        object->Init();
    }
}

void ShineManager::Finalize()
{
    for (auto& object : mpObjects)
    {
        object->Finalize();
    }

    mpObjects.clear();

    mpShineObject->Finalize();
    delete mpShineObject;
}

void ShineManager::Update()
{
    for (auto& object : mpObjects)
    {
        object->Update();
    }
    mpShineObject->Update();

    // 光領域作成
    CreateLightArea();

    Master::mpImguiManager->AddDrawImgui(IMGUI_INT_DATA::GetImguiData(
        {&mnDrawMode},
        0.1f, 
        0.1f, 
        0.1f, 
        0, 
        SHINE_DRAW_MODE::SHINE_DRAW_MODE_MAX - 1, 
        "_DRAW_MODE", 
        "%d", 
        0x10,
        IMGUI_TYPE::DRAG1));
}

void ShineManager::Draw()
{
    switch (mnDrawMode)
    {
    case SHINE_DRAW_MODE::OBJECT_SHINE_DRAW_MODE:
        for (auto& object : mpObjects)
        {
            object->Draw();
        }
        break;
    
    case SHINE_DRAW_MODE::TRIANGLE_SHINE_DRAW_MODE:
        for (const SHINE_TRIANGLE& shineTriangle : mstSheineTriangles)
        {
            // 光領域の描画
            DrawTriangle(
                shineTriangle.Vertex1.x, shineTriangle.Vertex1.y,
                shineTriangle.Vertex2.x, shineTriangle.Vertex2.y,
                shineTriangle.Vertex3.x, shineTriangle.Vertex3.y,
                GetColor(255, 255, 255), FALSE
            );
        }
        break;
    
    case SHINE_DRAW_MODE::GRID_SHINE_DRAW_MODE:
        for (int y = 0; y < MAP_ARRAY_SIZE_Y; ++y)
        {
            for (int x = 0; x < MAP_ARRAY_SIZE_X; ++x)
            {
                if (mstMapObjectGridData[y][x].LitFlag)
                {
                    DrawBox(ONE_GRID_SIZE_X * x,       ONE_GRID_SIZE_Y * y,
                            ONE_GRID_SIZE_X * (x + 1), ONE_GRID_SIZE_Y * (y + 1),
                            GetColor(255, 255, 255),
                            FALSE);
                }
            }
        }
        break;
    }

    mpShineObject->Draw();
}
// 光領域の作成
void ShineManager::CreateLightArea()
{
    mstSheineTriangles.clear();

    // 光領域をリセット
    for (int y = 0; y < MAP_ARRAY_SIZE_Y; ++y)
    {
        for (int x = 0; x < MAP_ARRAY_SIZE_X; ++x)
        {
            mstMapObjectGridData[y][x].LitFlag = false;
        }
    }

    // 光源の位置
    Vector2 shinePos = mpShineObject->GetPosition();

    // 光源が存在するグリッド
    Vector2_Int shineGridPos(
        static_cast<int>(shinePos.x / ONE_GRID_SIZE_X),
        static_cast<int>(shinePos.y / ONE_GRID_SIZE_Y)
    );

    // マップ外なら処理しない
    if (IsOutsideLightStage(shineGridPos))
    {
        return;
    }

    // 調査するグリッド
    std::queue<Vector2_Int> nextCheckShinePos;

    // 光源のグリッドから開始
    nextCheckShinePos.push(shineGridPos);

    // 光源のグリッドは必ず光領域に含める
    mstMapObjectGridData[shineGridPos.y][shineGridPos.x].LitFlag = true;

    // グリッドの探索
    CheckLightGrid(nextCheckShinePos);
}


// グリッドの探索
void ShineManager::CheckLightGrid(std::queue<Vector2_Int>& nextCheckShinePos)
{
    // 8方向
    const Vector2_Int checkDirections[8] =
    {
        Vector2_Int( 1,  0),
        Vector2_Int(-1,  0),
        Vector2_Int( 0,  1),
        Vector2_Int( 0, -1),

        Vector2_Int( 1,  1),
        Vector2_Int(-1,  1),
        Vector2_Int( 1, -1),
        Vector2_Int(-1, -1)
    };

    std::vector<SHINE_DIRECTION> shineDirections;
    shineDirections.push_back(mpShineObject->GetLineDirection());

    while (!nextCheckShinePos.empty())
    {
        // 今回調べるグリッドを取り出す
        std::queue<Vector2_Int> nowCheckShinePos;
        nowCheckShinePos.swap(nextCheckShinePos);

        // 今回の探索中に見つかった障害物
        std::stack<BLOCK_POS_DATA> blockPoss;

        int shineDirectionsIndex = 0;

        //  隣接グリッドの探索
        while (nowCheckShinePos.size())
        {
            Vector2_Int nowPos = nowCheckShinePos.front();
            nowCheckShinePos.pop();
            
            for (const Vector2_Int& checkDirection : checkDirections)
            {
                Vector2_Int nextPos = nowPos + checkDirection;

                // マップ外なら除外
                if (IsOutsideLightStage(nextPos))
                {
                    // 光領域を削る
                    // 三角頂点入力
                    continue;
                }

                // すでに光が届いているなら除外
                if (mstMapObjectGridData[nextPos.y][nextPos.x].LitFlag)
                {
                    continue;
                }

                // グリッドの光判定
                SHINE_GRID_TYPE lightGridResult;
                while ((lightGridResult = JudgeGrid(nextPos, shineDirections, shineDirectionsIndex)) == SHINE_GRID_TYPE::OTHER_SHINE_GRID)
                {
                    shineDirectionsIndex++;
                    if (shineDirections.size() >= shineDirectionsIndex)
                    {
                        break;
                    }
                }

                // 光範囲外なら除外
                switch (lightGridResult)
                {
                case SHINE_GRID_TYPE::NOT_SHINE_GRID:
                {
                    continue;
                }
                case SHINE_GRID_TYPE::SHINE_GRID:
                {
                    // 光範囲内として登録
                    mstMapObjectGridData[nextPos.y][nextPos.x].LitFlag = true;

                    // 次に調べるグリッドへ追加
                    nextCheckShinePos.push(nextPos);
                    break;
                }
                case SHINE_GRID_TYPE::SHINE_AND_OBJECT_GRID:
                {
                    // ここでグリッド内調査を申請
                    BLOCK_POS_DATA blockPos;
                    blockPos.BlockPos = nextPos;
                    //blockPos.ArrayIndex = ; // 分かつ可能性のある光領域の添え字
                    blockPoss.push(blockPos);
                    break;
                }
                case SHINE_GRID_TYPE::OTHER_SHINE_AND_OBJECT_GRID:
                {
                    // ここでグリッド内調査を申請
                    BLOCK_POS_DATA blockPos;
                    blockPos.BlockPos = nextPos;
                    //blockPos.ArrayIndex = ; // 分かつ可能性のある光領域の添え字
                    blockPoss.push(blockPos);
                    //blockPos.ArrayIndex = ; // 分かつ可能性のある一つ前の光領域の添え字
                    //blockPoss.push(blockPos);
                    break;
                }
                case SHINE_GRID_TYPE::OTHER_SHINE_GRID:
                {
                    break;
                }
                }
            }
        }

        // オブジェクトがあるグリッド内部の処理
        InsideLightGridProcess(blockPoss, nextCheckShinePos, shineDirections);
    }
}

// グリッドが光範囲内か判定
SHINE_GRID_TYPE ShineManager::JudgeGrid(const Vector2_Int& gridPos, const std::vector<SHINE_DIRECTION>& shineDirections, int shineDirectionsIndex)
{
    // 光源の位置
    Vector2 shinePos = mpShineObject->GetPosition();

    // 光の方向
    SHINE_DIRECTION shineDirection = shineDirections[shineDirectionsIndex];

    Vector2 direction1 = shineDirection.shineDirection1;

    Vector2 direction2 = shineDirection.shineDirection2;

    // 2本の方向ベクトルの外積
    const float directionCross =
        direction1.x * direction2.y -
        direction1.y * direction2.x;

    // グリッドの四隅の座標
    const float left =
        gridPos.x * ONE_GRID_SIZE_X;

    const float right =
        left + ONE_GRID_SIZE_X;

    const float top =
        gridPos.y * ONE_GRID_SIZE_Y;

    const float bottom =
        top + ONE_GRID_SIZE_Y;

    const Vector2 gridCorners[4] =
    {
        Vector2(left,  top),
        Vector2(right, top),
        Vector2(left,  bottom),
        Vector2(right, bottom)
    };

    // 四隅のいずれかが光範囲内か判定
    for (const Vector2& corner : gridCorners)
    {
        // 光源からグリッドの角への方向
        Vector2 toCorner =
            corner - shinePos;

        // 光源と角が同じ位置なら光範囲内
        if (toCorner.x == 0.0f &&
            toCorner.y == 0.0f)
        {
            if (mstMapObjectGridData[gridPos.y][gridPos.x].LinePoss.size() > 0)
            {
                return SHINE_GRID_TYPE::SHINE_AND_OBJECT_GRID;
            }
            return SHINE_GRID_TYPE::SHINE_GRID;
        }

        // 1本目の方向との外積
        const float cross1 =
            direction1.x * toCorner.y -
            direction1.y * toCorner.x;

        // 2本目の方向との外積
        const float cross2 =
            direction2.x * toCorner.y -
            direction2.y * toCorner.x;

        bool isLightArea = false;

        // 2本の方向ベクトルの間にあるか判定
        if (directionCross < 0.0f)
        {
            // 時計回り
            isLightArea =
                cross1 <= 0.0f &&
                cross2 >= 0.0f;
        }
        else
        {
            // 反時計回り
            isLightArea =
                cross1 >= 0.0f &&
                cross2 <= 0.0f;
        }

        // 光範囲内
        if (isLightArea)
        {
            if (mstMapObjectGridData[gridPos.y][gridPos.x].LinePoss.size() > 0)
            {
                return SHINE_GRID_TYPE::SHINE_AND_OBJECT_GRID;
            }
            return SHINE_GRID_TYPE::SHINE_GRID;
        }
    }

    // 光範囲外
    return SHINE_GRID_TYPE::NOT_SHINE_GRID;
}

// グリッド内部の判定
void ShineManager::InsideLightGridProcess(std::stack<BLOCK_POS_DATA>& blockPoss, std::queue<Vector2_Int>& nextCheckShinePos, std::vector<SHINE_DIRECTION>& shineDirections)
{
    // 遮っている場所探す
    while (blockPoss.size())
    {
        BLOCK_POS_DATA blockPos = blockPoss.top();
        blockPoss.pop();

        // 光を遮る位置にオブジェクトがあるなら三角頂点入力
    }
}

// マップ外判定
bool ShineManager::IsOutsideLightStage(const Vector2_Int& gridPos)
{
    if (gridPos.x < 0 || gridPos.x >= MAP_ARRAY_SIZE_X ||
        gridPos.y < 0 || gridPos.y >= MAP_ARRAY_SIZE_Y)
    {
        return true;
    }

    return false;
}