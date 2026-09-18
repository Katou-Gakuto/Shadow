#include <algorithm>
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

static constexpr int testIndex = 2;
static int testNumber[testIndex] = {10, 10};

ShineManager::ShineManager()
: mpObjects()
, mstSheineTriangles()
, mpShineObject(nullptr)
, mnDrawMode(SHINE_DRAW_MODE::GRID_SHINE_DRAW_MODE)
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
        IMGUI_TYPE::SLIDER1));

    for (int i = 0; i < testIndex; ++i)
    {
        Master::mpImguiManager->AddDrawImgui(IMGUI_INT_DATA::GetImguiData(
            {&testNumber[i]},
            0.1f, 
            0.1f, 
            0.1f, 
            -100, 
            100, 
            "_TEST_NUMBER", 
            "%d",
            0,
            IMGUI_TYPE::DRAG1));
    }
}

void ShineManager::Draw()
{
    int x = 0, y = 0;
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
            x += testNumber[0];
            y += testNumber[1];
            // 光領域の描画
            DrawTriangle(
                shineTriangle.Vertex1.x + x, shineTriangle.Vertex1.y + y,
                shineTriangle.Vertex2.x, shineTriangle.Vertex2.y,
                shineTriangle.Vertex3.x, shineTriangle.Vertex3.y,
                GetColor(255, 255, 255), FALSE
            );
        }
        break;
    
    case SHINE_DRAW_MODE::TRIANGLE_SHINE_DRAW_MODE_TRUE:
        for (const SHINE_TRIANGLE& shineTriangle : mstSheineTriangles)
        {
            x += testNumber[0];
            y += testNumber[1];
            // 光領域の描画
            DrawTriangle(
                shineTriangle.Vertex1.x + x, shineTriangle.Vertex1.y + y,
                shineTriangle.Vertex2.x, shineTriangle.Vertex2.y,
                shineTriangle.Vertex3.x, shineTriangle.Vertex3.y,
                GetColor(255, 255, 255), TRUE
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
    std::vector<SHINE_DIRECTION> shineDirections;
    shineDirections.push_back(mpShineObject->GetShineDirection());

    while (!nextCheckShinePos.empty())
    {
        // 今回調べるグリッドを取り出す
        std::queue<Vector2_Int> nowCheckShinePos;
        nowCheckShinePos.swap(nextCheckShinePos);

        // 今回の探索中に見つかった障害物
        std::stack<BLOCK_POS_DATA> blockPoss;
        
        // 光源から見て左側の光領域から順番に処理する
        for (int shineDirectionsIndex = 0; shineDirectionsIndex < static_cast<int>(shineDirections.size()); ++shineDirectionsIndex)
        {
            // 現在の光領域を左端から右端へ走査
            for (const Vector2_Int& checkPos : GetLightGridPositions(nowCheckShinePos))
            {
                // マップ外なら除外
                if (IsOutsideLightStage(checkPos))
                {
                    // ここで光を遮るものを追加
                    BLOCK_POS_DATA blockPos;
                    blockPos.OutsideGridFlag = true;
                    blockPos.BlockPos = checkPos;
                    blockPos.ArrayIndex = shineDirectionsIndex;
                    blockPoss.push(blockPos);
                    continue;
                }

                // すでに光が届いているなら除外
                if (mstMapObjectGridData[checkPos.y][checkPos.x].LitFlag)
                {
                    continue;
                }

                // グリッドの判定
                SHINE_GRID_TYPE lightGridResult = JudgeGrid(checkPos, shineDirections, shineDirectionsIndex);

                // グリッドの状況によって処理
                switch (lightGridResult)
                {
                case SHINE_GRID_TYPE::NOT_SHINE_GRID:
                {
                    break;
                }
                case SHINE_GRID_TYPE::SHINE_GRID:
                {
                    // 光範囲内として登録
                    mstMapObjectGridData[checkPos.y][checkPos.x].LitFlag = true;

                    // 次に調べるグリッドへ追加
                    nextCheckShinePos.push(checkPos);
                    break;
                }
                case SHINE_GRID_TYPE::SHINE_AND_OBJECT_GRID:
                {
                    // ここで光を遮るものを追加
                    BLOCK_POS_DATA blockPos;
                    blockPos.OutsideGridFlag = false;
                    blockPos.BlockPos = checkPos;
                    blockPos.ArrayIndex = shineDirectionsIndex;
                    blockPoss.push(blockPos);
                    break;
                }
                }
            }
        }

        // 光を遮る物の処理
        LightBlockProcess(blockPoss, nextCheckShinePos, shineDirections);
    }
}

// グリッドが光範囲内か判定
SHINE_GRID_TYPE ShineManager::JudgeGrid(const Vector2_Int& gridPos, const std::vector<SHINE_DIRECTION>& shineDirections, int shineDirectionsIndex)
{
    // 光源の位置
    Vector2 shinePos = mpShineObject->GetPosition();

    // 光の方向
    SHINE_DIRECTION shineDirection = shineDirections[shineDirectionsIndex];

    Vector2 direction1 = shineDirection.shineDirectionLeft;

    Vector2 direction2 = shineDirection.shineDirectionRight;

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
        Vector2 toCorner = corner - shinePos;

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

// // // グ光を遮るものを確認し、それに応じた処理を行う
// // void ShineManager::LightBlockProcess(std::stack<BLOCK_POS_DATA>& blockPoss, std::queue<Vector2_Int>& nextCheckShinePos, std::vector<SHINE_DIRECTION>& shineDirections)
// // {
// //     // 遮っている場所探す
// //     while (blockPoss.size())
// //     {
// //         BLOCK_POS_DATA blockPos = blockPoss.top();
// //         blockPoss.pop();

// //         // 光を遮る位置にオブジェクトがあるなら三角頂点入力
// //         if (blockPos.OutsideGridFlag)
// //         {
// //         }
// //         else
// //         {
// //         }
// //     }
// // }
// // 光を遮るものを確認し、それに応じた処理を行う
// void ShineManager::LightBlockProcess(std::stack<BLOCK_POS_DATA>& blockPoss, std::queue<Vector2_Int>& nextCheckShinePos, std::vector<SHINE_DIRECTION>& shineDirections)
// {

//     // 光源の位置
//     const Vector2 shinePos = mpShineObject->GetPosition();
    
//     // 遮っている場所を探す
//     while (!blockPoss.empty())
//     {
//         BLOCK_POS_DATA blockPos = blockPoss.top();
//         blockPoss.pop();

//         // 現在の光方向を取得
//         const SHINE_DIRECTION& shineDirection =
//             shineDirections[blockPos.ArrayIndex];

//         // グリッドの範囲
//         const float left =
//             blockPos.BlockPos.x * ONE_GRID_SIZE_X;

//         const float right =
//             left + ONE_GRID_SIZE_X;

//         const float top =
//             blockPos.BlockPos.y * ONE_GRID_SIZE_Y;

//         const float bottom =
//             top + ONE_GRID_SIZE_Y;

//         // 光源から各方向へ伸ばした線と
//         // グリッドの辺との交点を求める
//         Vector2 intersection1;
//         Vector2 intersection2;

//         auto GetIntersection =
//             [&](const Vector2& direction, Vector2& intersection) -> bool
//         {
//             float nearestT = FLT_MAX;
//             bool found = false;

//             // 左辺・右辺・上辺・下辺
//             const Vector2 edgeStart[4] =
//             {
//                 Vector2(left,  top),
//                 Vector2(right, top),
//                 Vector2(left,  top),
//                 Vector2(left,  bottom)
//             };

//             const Vector2 edgeEnd[4] =
//             {
//                 Vector2(left,  bottom),
//                 Vector2(right, bottom),
//                 Vector2(right, top),
//                 Vector2(right, bottom)
//             };

//             for (int i = 0; i < 4; ++i)
//             {
//                 const Vector2 edge = edgeEnd[i] - edgeStart[i];

//                 const float cross =
//                     direction.x * edge.y -
//                     direction.y * edge.x;

//                 // 平行なら交わらない
//                 if (cross == 0.0f)
//                 {
//                     continue;
//                 }

//                 const Vector2 toEdge =
//                     edgeStart[i] - shinePos;

//                 const float t =
//                     (toEdge.x * edge.y -
//                      toEdge.y * edge.x) / cross;

//                 const float u =
//                     (toEdge.x * direction.y -
//                      toEdge.y * direction.x) / cross;

//                 // 光源から前方にあり、
//                 // 辺の範囲内にある交点だけを使用
//                 if (t < 0.0f ||
//                     u < 0.0f ||
//                     u > 1.0f)
//                 {
//                     continue;
//                 }

//                 // 最も光源に近い交点を取得
//                 if (t < nearestT)
//                 {
//                     nearestT = t;

//                     intersection =
//                         shinePos + direction * t;

//                     found = true;
//                 }
//             }

//             return found;
//         };

//         // 光を遮る位置にオブジェクトがあるなら三角頂点入力
//         if (blockPos.OutsideGridFlag)
//         {
//             // マップ外の場合
//             // TODO: マップ端との交点を求める
//         }
//         else
//         {
//             // 1本目の光境界との交点
//             const bool found1 =
//                 GetIntersection(
//                     shineDirection.shineDirection1,
//                     intersection1);

//             // 2本目の光境界との交点
//             const bool found2 =
//                 GetIntersection(
//                     shineDirection.shineDirection2,
//                     intersection2);

//             if (found1 && found2)
//             {
//                 // 遮蔽物によってできる光領域の三角形
//                 SHINE_TRIANGLE shineTriangle;

//                 shineTriangle.Vertex1 =
//                     Vector2_Int(
//                         static_cast<int>(shinePos.x),
//                         static_cast<int>(shinePos.y));

//                 shineTriangle.Vertex2 =
//                     Vector2_Int(
//                         static_cast<int>(intersection1.x),
//                         static_cast<int>(intersection1.y));

//                 shineTriangle.Vertex3 =
//                     Vector2_Int(
//                         static_cast<int>(intersection2.x),
//                         static_cast<int>(intersection2.y));

//                 mstSheineTriangles.push_back(shineTriangle);
//             }
//         }
//     }
// }
// 光を遮るものを確認し、それに応じた処理を行う
void ShineManager::LightBlockProcess(
    std::stack<BLOCK_POS_DATA>& blockPoss,
    std::queue<Vector2_Int>& nextCheckShinePos,
    std::vector<SHINE_DIRECTION>& shineDirections)
{
    // 遮っている場所を探す
    while (!blockPoss.empty())
    {
        BLOCK_POS_DATA blockPos = blockPoss.top();
        blockPoss.pop();

        // 現在の光方向を取得
        const SHINE_DIRECTION& shineDirection =
            shineDirections[blockPos.ArrayIndex];

        // 光源の位置
        const Vector2 shinePos =
            mpShineObject->GetPosition();

        // 画面外
        if (blockPos.OutsideGridFlag)
        {
            // X・Yの両方が範囲外なら判定しない
            const bool outsideX =
                blockPos.BlockPos.x < 0 ||
                blockPos.BlockPos.x >= MAP_ARRAY_SIZE_X;

            const bool outsideY =
                blockPos.BlockPos.y < 0 ||
                blockPos.BlockPos.y >= MAP_ARRAY_SIZE_Y;

            if (outsideX && outsideY)
            {
                continue;
            }

            // 画面端の2点
            Vector2 edgePos1;
            Vector2 edgePos2;

            // 左端
            if (blockPos.BlockPos.x < 0)
            {
                edgePos1 = Vector2(0.0f, 0.0f);
                edgePos2 = Vector2(0.0f, static_cast<float>(MAP_SIZE_Y));
            }
            // 右端
            else if (blockPos.BlockPos.x >= MAP_ARRAY_SIZE_X)
            {
                edgePos1 = Vector2(static_cast<float>(MAP_SIZE_X), 0.0f);

                edgePos2 = Vector2(static_cast<float>(MAP_SIZE_X), static_cast<float>(MAP_SIZE_Y));
            }
            // 上端
            else if (blockPos.BlockPos.y < 0)
            {
                edgePos1 = Vector2(0.0f, 0.0f);
                edgePos2 = Vector2(static_cast<float>(MAP_SIZE_X), 0.0f);
            }
            // 下端
            else
            {
                edgePos1 = Vector2(0.0f, static_cast<float>(MAP_SIZE_Y));

                edgePos2 = Vector2(static_cast<float>(MAP_SIZE_X), static_cast<float>(MAP_SIZE_Y));
            }

            const Vector2 edge =
                edgePos2 - edgePos1;

            Vector2 intersection1;
            Vector2 intersection2;

            // 光方向1との交点
            const float cross1 =
                shineDirection.shineDirectionLeft.x * edge.y -
                shineDirection.shineDirectionLeft.y * edge.x;

            if (cross1 != 0.0f)
            {
                const Vector2 toEdge =
                    edgePos1 - shinePos;

                const float t =
                    (toEdge.x * edge.y -
                     toEdge.y * edge.x) / cross1;

                intersection1 =
                    shinePos +
                    shineDirection.shineDirectionLeft * t;
            }

            // 光方向2との交点
            const float cross2 =
                shineDirection.shineDirectionRight.x * edge.y -
                shineDirection.shineDirectionRight.y * edge.x;

            if (cross2 != 0.0f)
            {
                const Vector2 toEdge =
                    edgePos1 - shinePos;

                const float t =
                    (toEdge.x * edge.y -
                     toEdge.y * edge.x) / cross2;

                intersection2 =
                    shinePos +
                    shineDirection.shineDirectionRight * t;
            }

            {
                // グリッド内に補正
                {// intersection1
                    if (intersection1.x < (blockPos.BlockPos.x * MAP_SIZE_X))
                    {
                        intersection1.x = (blockPos.BlockPos.x * MAP_SIZE_X);
                    }
                    else if (intersection1.x > ((blockPos.BlockPos.x + 1) * MAP_SIZE_X))
                    {
                        intersection1.x = ((blockPos.BlockPos.x + 1) * MAP_SIZE_X);
                    }
                    
                    if (intersection1.y < (blockPos.BlockPos.y * MAP_SIZE_Y))
                    {
                        intersection1.y = (blockPos.BlockPos.y * MAP_SIZE_Y);
                    }
                    else if (intersection1.y > ((blockPos.BlockPos.y + 1) * MAP_SIZE_Y))
                    {
                        intersection1.y = ((blockPos.BlockPos.y + 1) * MAP_SIZE_Y);
                    }
                }
                {// intersection2
                    if (intersection2.x < (blockPos.BlockPos.x * MAP_SIZE_X))
                    {
                        intersection2.x = (blockPos.BlockPos.x * MAP_SIZE_X);
                    }
                    else if (intersection2.x > ((blockPos.BlockPos.x + 1) * MAP_SIZE_X))
                    {
                        intersection2.x = ((blockPos.BlockPos.x + 1) * MAP_SIZE_X);
                    }
                    
                    if (intersection2.y < (blockPos.BlockPos.y * MAP_SIZE_Y))
                    {
                        intersection2.y = (blockPos.BlockPos.y * MAP_SIZE_Y);
                    }
                    else if (intersection2.y > ((blockPos.BlockPos.y + 1) * MAP_SIZE_Y))
                    {
                        intersection2.y = ((blockPos.BlockPos.y + 1) * MAP_SIZE_Y);
                    }
                }
            }

            // 光源と画面端との交点2つで三角形を作成
            SHINE_TRIANGLE shineTriangle;

            shineTriangle.Vertex1 =
            {
                static_cast<int>(shinePos.x),
                static_cast<int>(shinePos.y)
            };

            shineTriangle.Vertex2 =
            {
                static_cast<int>(intersection1.x),
                static_cast<int>(intersection1.y)
            };

            shineTriangle.Vertex3 =
            {
                static_cast<int>(intersection2.x),
                static_cast<int>(intersection2.y)
            };

            mstSheineTriangles.push_back(shineTriangle);
        }
        else
        {
            // TODO: オブジェクトによる遮蔽処理
        }
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

// 光領域を左端から右端へ走査するグリッドを取得
std::vector<Vector2_Int> ShineManager::GetLightGridPositions(const std::queue<Vector2_Int>& nowCheckShinePos)
{
    std::vector<Vector2_Int> lightGridPositions;

    const Vector2 shinePos = mpShineObject->GetPosition();

    // 光源から見た左方向
    const Vector2 leftDirection =
    {
        -mpShineObject->GetShineDirection().shineDirectionLeft.y,
         mpShineObject->GetShineDirection().shineDirectionLeft.x
    };

    std::queue<Vector2_Int> checkPos = nowCheckShinePos;

    while (!checkPos.empty())
    {
        const Vector2_Int nowPos = checkPos.front();
        checkPos.pop();

        for (int y = -1; y <= 1; ++y)
        {
            for (int x = -1; x <= 1; ++x)
            {
                if (x == 0 && y == 0)
                {
                    continue;
                }

                Vector2_Int nextPos =
                {
                    nowPos.x + x,
                    nowPos.y + y
                };

                // マップ外でも光ないなら欲しい(マップ外判定は別でやる)
                // if (IsOutsideLightStage(nextPos))
                // {
                //     continue;
                // }

                bool isAlreadyAdded = false;

                for (const Vector2_Int& addedPos : lightGridPositions)
                {
                    if (addedPos.x == nextPos.x &&
                        addedPos.y == nextPos.y)
                    {
                        isAlreadyAdded = true;
                        break;
                    }
                }

                if (!isAlreadyAdded)
                {
                    lightGridPositions.push_back(nextPos);
                }
            }
        }
    }

    // 光源から見て左側から右側へ並べる
    std::sort(
        lightGridPositions.begin(),
        lightGridPositions.end(),
        [&](const Vector2_Int& lhs, const Vector2_Int& rhs)
        {
            const Vector2 lhsPos =
            {
                lhs.x * ONE_GRID_SIZE_X + ONE_GRID_SIZE_X * 0.5f,
                lhs.y * ONE_GRID_SIZE_Y + ONE_GRID_SIZE_Y * 0.5f
            };

            const Vector2 rhsPos =
            {
                rhs.x * ONE_GRID_SIZE_X + ONE_GRID_SIZE_X * 0.5f,
                rhs.y * ONE_GRID_SIZE_Y + ONE_GRID_SIZE_Y * 0.5f
            };

            const Vector2 lhsToGrid =
            {
                lhsPos.x - shinePos.x,
                lhsPos.y - shinePos.y
            };

            const Vector2 rhsToGrid =
            {
                rhsPos.x - shinePos.x,
                rhsPos.y - shinePos.y
            };

            const float lhsValue =
                lhsToGrid.x * leftDirection.x +
                lhsToGrid.y * leftDirection.y;

            const float rhsValue =
                rhsToGrid.x * leftDirection.x +
                rhsToGrid.y * leftDirection.y;

            return lhsValue > rhsValue;
        });

    return lightGridPositions;
}