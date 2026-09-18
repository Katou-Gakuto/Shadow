#include <cmath>

#include "DxLib.h"

#include "Master.h"

#include "KeyState.h"
#include "ShineObject.h"
#include "ImguiManager.h"

ShineObject::ShineObject()
: TestObjectBase()
, mstShineDirection()
, mfAngle(0.0f)
, mfVisionAngle(25.0f)
{
}

ShineObject::~ShineObject()
{
}

void ShineObject::Init()
{
}

void ShineObject::Finalize()
{
}

void ShineObject::Update()
{

    if (Master::mpKeyState->GetWordKey_Board(KEY_BOARD_WORD::A))
    {
        mv2Position.x -= 10;
    }
    if (Master::mpKeyState->GetWordKey_Board(KEY_BOARD_WORD::D))
    {
        mv2Position.x += 10;
    }
    if (Master::mpKeyState->GetWordKey_Board(KEY_BOARD_WORD::W))
    {
        mv2Position.y -= 10;
    }
    if (Master::mpKeyState->GetWordKey_Board(KEY_BOARD_WORD::S))
    {
        mv2Position.y += 10;
    }
    
    if (Master::mpKeyState->GetWordKey_Board(KEY_BOARD_WORD::ARROW_DOWN))
    {
        mfAngle -= 3;
    }
    if (Master::mpKeyState->GetWordKey_Board(KEY_BOARD_WORD::ARROW_UP))
    {
        mfAngle += 3;
    }
    
    if (Master::mpKeyState->GetWordKey_Board(KEY_BOARD_WORD::ARROW_LEFT))
    {
        mfVisionAngle -= 3;
        if (mfVisionAngle < 0)
        {
            mfVisionAngle = 360.0f;
        }
    }
    if (Master::mpKeyState->GetWordKey_Board(KEY_BOARD_WORD::ARROW_RIGHT))
    {
        mfVisionAngle += 3;
        if (mfVisionAngle > 360.0f)
        {
            mfVisionAngle = 0.0f;
        }
    }
    
    IMGUI_FLOAT_DATA setImguiData = IMGUI_FLOAT_DATA::GetImguiData(
        { &mv2Position.y, &mv2Position.x },
        0.01f,
        0.01f,
        0.01f,
        0.0f,
        2000.0f,
        "_SHINE_POS",
        "%f",
        0x10,
        IMGUI_TYPE::SLIDER2
    );
    Master::mpImguiManager->AddDrawImgui(setImguiData);
    setImguiData.SetSpeed(0.001f);
    setImguiData.SetMin(-6.28);
    setImguiData.SetMax(6.28f);
    setImguiData.SetLabel("_SHINE_DIR1");
    setImguiData.ReSetVariable();
    setImguiData.AddVariable(&mstShineDirection.shineDirectionLeft.y);
    setImguiData.AddVariable(&mstShineDirection.shineDirectionLeft.x);
    Master::mpImguiManager->AddDrawImgui(setImguiData);
    setImguiData.SetLabel("_SHINE_DIR2");
    setImguiData.ReSetVariable();
    setImguiData.AddVariable(&mstShineDirection.shineDirectionRight.y);
    setImguiData.AddVariable(&mstShineDirection.shineDirectionRight.x);
    Master::mpImguiManager->AddDrawImgui(setImguiData);
    setImguiData = IMGUI_FLOAT_DATA::GetImguiData(
        { &mfAngle },
        0.01f,
        0.01f,
        0.01f,
        -720.0f,
        720.0f,
        "_ANGLE",
        "%f",
        0x10,
        IMGUI_TYPE::SLIDER1
    );
    Master::mpImguiManager->AddDrawImgui(setImguiData);
    setImguiData.SetLabel("_VISION_ANGLE");
    setImguiData.SetMin(0.0f);
    setImguiData.SetMax(360.0f);
    setImguiData.ReSetVariable();
    setImguiData.AddVariable(&mfVisionAngle);
    Master::mpImguiManager->AddDrawImgui(setImguiData);


    constexpr float DEG_TO_RAD = 3.14159265358979323846f / 180.0f;

    // 視界の半分の角度
    const float halfVisionAngle = mfVisionAngle * 0.5f;

    // 視界の左端
    const float leftAngle =
        (mfAngle - halfVisionAngle) * DEG_TO_RAD;

    // 視界の右端
    const float rightAngle =
        (mfAngle + halfVisionAngle) * DEG_TO_RAD;

    // 視界の左端ベクトル
    mstShineDirection.shineDirectionLeft =
    {
        std::cos(leftAngle),
        std::sin(leftAngle)
    };

    // 視界の右端ベクトル
    mstShineDirection.shineDirectionRight =
    {
        std::cos(rightAngle),
        std::sin(rightAngle)
    };
}

void ShineObject::Draw()
{
    DrawCircle(mv2Position.x, mv2Position.y, (mv2Size.x * mv2Size.y * 0.5f) * 0.5f, GetColor(0, 255, 0), TRUE);
}