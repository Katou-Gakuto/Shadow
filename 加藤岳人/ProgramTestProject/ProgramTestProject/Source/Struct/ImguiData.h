#pragma once
#include <string>
#include <vector>

#include "ImguiEnum.h"

#include "imgui.h"

template<typename VariableType>
struct IMGUI_TEMPLATE_DATA
{
public:
#ifdef _DEBUG
    static constexpr int IMGUI_VARIABLE_MAX = 4;

    std::vector<VariableType*> VariableDatas = {};    // 変数ポインタ

    VariableType PreVariable[IMGUI_VARIABLE_MAX];    // 前の変数

    VariableType ChangeVariable[IMGUI_VARIABLE_MAX]; // 変更用変数

    float Speed = 1.0f; // ドラッグ時の変化速度

    float Step = 0.0f;      // 刻み幅
    float StepFast = 0.0f;  // Ctrl押しながらの刻み幅

    VariableType Min = 0.0f;  // 最小値
    VariableType Max = 0.0f;  // 最大値
    
    std::string Label = "NONE";  // ラベル
    std::string Format = "%.6f"; // 表示数字

    ImGuiSliderFlags Flag = 0;  // オプションフラグ

    IMGUI_TYPE ImguiType = IMGUI_TYPE::SLIDER1; // Imgui種類

    bool AddNumberDrawFlag = true; // 追加Imguiナンバー用フラグ

#endif
    IMGUI_TEMPLATE_DATA()
#ifdef _DEBUG
    : Speed(1.0f)
    , Step(0.0f)
    , StepFast(0.0f)
    , Min((VariableType)0)
    , Max((VariableType)0)
    , Label("NONE")
    , Format("%.6f")
    , Flag(0)
    , ImguiType(IMGUI_TYPE::SLIDER1)
    , PreVariable()
    , ChangeVariable()
    , AddNumberDrawFlag(true)
#endif
    {
#ifdef _DEBUG
        VariableDatas.clear();
#endif
    }

    IMGUI_TEMPLATE_DATA(
                        std::vector<VariableType*> variableDatas,
                        VariableType* preVariable,
                        VariableType* changeVariable,
                        float speed = 1.0f,
                        float step = 0.0f,
                        float stepFast = 0.0f,
                        VariableType min = (VariableType)0,
                        VariableType max = (VariableType)0,
                        std::string label = "NONE",
                        std::string format = "%.6f",
                        ImGuiSliderFlags flag = 0,
                        IMGUI_TYPE imguiType = IMGUI_TYPE::SLIDER1,
                        bool addNumberFlag = true
                        )
#ifdef _DEBUG
    : Speed(speed)
    , Step(step)
    , StepFast(stepFast)
    , Min(min)
    , Max(max)
    , Label(label)
    , Format(format)
    , Flag(flag)
    , ImguiType(imguiType)
    , VariableDatas(variableDatas)
    ,AddNumberDrawFlag(addNumberFlag)
#endif
    {
#ifdef _DEBUG
        for (int i = 0; i < IMGUI_VARIABLE_MAX; i++)
        {
            PreVariable[i] = preVariable[i];
            ChangeVariable[i] = changeVariable[i];
        }
#endif
    }

    /*------------*/
    /*【リセット】*/
    /*------------*/
    /// <summary>変数リセット</summary>
    void ReSetVariable()
    {
#ifdef _DEBUG
        VariableDatas.clear();
#endif
    }

    /*--------*/
    /*【設定】*/
    /*--------*/
    /// <summary>変数設定</summary>
    void AddVariable(VariableType *src) {
#ifdef _DEBUG
        VariableDatas.push_back(src);
#endif
    }

    /// <summary>速度設定</summary>
    void SetSpeed(float speed) {
#ifdef _DEBUG
        Speed = speed;
#endif
    }

    /// <summary>刻み幅設定</summary>
    void SetStep(float step) {
#ifdef _DEBUG
        Step = step;
#endif
    }
    /// <summary>刻み幅(ctrl)設定</summary>
    void SetStepFast(float stepFast) {
#ifdef _DEBUG
        StepFast = stepFast;
#endif
    }

    /// <summary>最低値設定</summary>
    void SetMin(VariableType min) {
#ifdef _DEBUG
        Min = min;
#endif
    }
    /// <summary>最大値設定</summary>
    void SetMax(VariableType max) {
#ifdef _DEBUG
        Max = max;
#endif
    }

    /// <summary>ラベル設定</summary>
    void SetLabel(std::string label) {
#ifdef _DEBUG
        Label = label;
#endif
    }
    /// <summary>フォーマット設定</summary>
    void SetFormat(std::string format) {
#ifdef _DEBUG
        Format = format;
#endif
    }

    /// <summary>オプションフラグ設定</summary>
    void SetFlag(ImGuiSliderFlags flag) {
#ifdef _DEBUG
        Flag = flag;
#endif
    }

    /// <summary>Imgui種類設定</summary>
    void SetImguiType(IMGUI_TYPE imguiType) {
#ifdef _DEBUG
        ImguiType = imguiType;    
#endif
    }

    /// <summary>追加Imguiナンバー用フラグ設定</summary>
    void SetAddNumberFlag(bool addNumberDrawFlag)
    {
#ifdef _DEBUG
        AddNumberDrawFlag = addNumberDrawFlag;
#endif
    }
};

// float
struct IMGUI_FLOAT_DATA : public IMGUI_TEMPLATE_DATA<float>
{
    static IMGUI_FLOAT_DATA GetImguiData(
        std::vector<float*> variableDatas,
        float speed = 1.0f,
        float step = 0.0f,
        float stepFast = 0.0f,
        float min = -1.0f,
        float max = 1.0f,
        std::string label = "NONE",
        std::string format = "%.6f",
        ImGuiSliderFlags flag = 0,
        IMGUI_TYPE imguiType = IMGUI_TYPE::SLIDER1,
        bool addNumberDrawFlag = true
    )
    {
        IMGUI_FLOAT_DATA imguiData;
#ifdef _DEBUG
        imguiData.VariableDatas = variableDatas;
        imguiData.Speed = speed;
        imguiData.Step = step;
        imguiData.StepFast = stepFast;
        imguiData.Min = min;
        imguiData.Max = max;
        imguiData.Label = label;
        imguiData.Format = format;
        imguiData.Flag = flag;
        imguiData.ImguiType = imguiType;
        imguiData.AddNumberDrawFlag = addNumberDrawFlag;
#endif
        return imguiData;
    }
};

// int
struct IMGUI_INT_DATA : public IMGUI_TEMPLATE_DATA<int>
{
    static IMGUI_INT_DATA GetImguiData(
        std::vector<int*> variableDatas,
        float speed = 1.0f,
        float step = 1.0f,
        float stepFast = 1.0f,
        int min = -1,
        int max = 1,
        std::string label = "NONE",
        std::string format = "%d",
        ImGuiSliderFlags flag = 0,
        IMGUI_TYPE imguiType = IMGUI_TYPE::SLIDER1,
        bool addNumberDrawFlag = true
    )
    {
        IMGUI_INT_DATA imguiData;
#ifdef _DEBUG
        imguiData.VariableDatas = variableDatas;
        imguiData.Speed = speed;
        imguiData.Step = step;
        imguiData.StepFast = stepFast;
        imguiData.Min = min;
        imguiData.Max = max;
        imguiData.Label = label;
        imguiData.Format = format;
        imguiData.Flag = flag;
        imguiData.ImguiType = imguiType;
        imguiData.AddNumberDrawFlag = addNumberDrawFlag;
#endif
        return imguiData;
    }
};