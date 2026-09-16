#include <vector>

#include "DxLib.h"

#include "ShineManager.h"
#include "ShineObject.h"
#include "TestObjectBase.h"
#include "WallObject.h"

ShineManager::ShineManager()
: mpObjects()
, mstSheineTriangles()
, mpShineObject(nullptr)
{
}

ShineManager::~ShineManager()
{
}

void ShineManager::Init()
{
    mpObjects.clear();

    // ここで生成
    mpShineObject = new ShineObject();
    mpShineObject->Init();

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
}

void ShineManager::Draw()
{
    for (auto& object : mpObjects)
    {
        object->Draw();
    }

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

    mpShineObject->Draw();
}

// 光領域の作成
void ShineManager::CreateLightArea()
{
    mstSheineTriangles.clear();
}