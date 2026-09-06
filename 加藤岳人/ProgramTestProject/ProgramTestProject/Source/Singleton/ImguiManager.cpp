#include <d3d11.h>
#include <tchar.h>
#include <vector>

#include "ImguiEnum.h"
#include "ImguiData.h"

#include "DxLib.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "ImguiManager.h"
#include "UtilCalc.h"

LRESULT WINAPI ImguiWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImguiManager::ImguiManager()
: mpMode(1)
, mnAddNumber(0)
{
    mstImguiFloatDatas.clear();
    mstImguiIntDatas.clear();
}

ImguiManager::~ImguiManager()
{
}


void ImguiManager::DxInit()
{
#ifdef _DEBUG
    if (mpMode == 1)
    {
        DxLib::SetHookWinProc(ImguiWndProc);
        //DxLib::SetAlwaysRunFlag(TRUE);
    }
#endif
}

// 初期化
void ImguiManager::Initilize()
{
#ifdef _DEBUG
    if (mpMode == 0)
    {
        // //// Make process DPI aware and obtain main monitor scale
        // //ImGui_ImplWin32_EnableDpiAwareness();
        // float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

        // //// Create application window
        // //wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
        // //::RegisterClassExW(&wc);
        // //hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 100, 100, (int)(1280 * main_scale), (int)(800 * main_scale), nullptr, nullptr, wc.hInstance, nullptr);

        // //// Initialize Direct3D
        // //if (!CreateDeviceD3D(hwnd))
        // //{
        // //    CleanupDeviceD3D();
        // //    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        // //    return;
        // //}

        // //// Show the window
        // //::ShowWindow(hwnd, SW_SHOWDEFAULT);
        // //::UpdateWindow(hwnd);

        // DxLib::SetHookWinProc(WndProc);
        // DxLib::SetAlwaysRunFlag(true);

        // // Setup Dear ImGui context
        // IMGUI_CHECKVERSION();
        // ImGui::CreateContext();
        // io = &ImGui::GetIO(); (void)*io;
        // io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        // io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // // Setup Dear ImGui style
        // ImGui::StyleColorsDark();
        // //ImGui::StyleColorsLight();

        // // Setup scaling
        // ImGuiStyle& style = ImGui::GetStyle();
        // style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
        // style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

        // //// Setup Platform/Renderer backends
        // //ImGui_ImplWin32_Init(hwnd);
        // //ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

        // ImGui_ImplWin32_Init(DxLib::GetMainWindowHandle());
        // ImGui_ImplDX11_Init((ID3D11Device*)DxLib::GetUseDirect3D11Device(), (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext());
    }
    else if (mpMode == 1)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        ImGui::StyleColorsDark();

        ImGui_ImplWin32_Init(DxLib::GetMainWindowHandle());
        ImGui_ImplDX11_Init((ID3D11Device*)DxLib::GetUseDirect3D11Device(), (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext());
    }
#endif
}

// 終了
void ImguiManager::Finalize()
{
#ifdef _DEBUG
    if (mpMode == 0)
    {
        // // Cleanup
        // ImGui_ImplDX11_Shutdown();
        // ImGui_ImplWin32_Shutdown();
        // ImGui::DestroyContext();

        // CleanupDeviceD3D();
        // ::DestroyWindow(hwnd);
        // ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    }
    else if (mpMode == 1)
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }
#endif
}

// 更新
void ImguiManager::Update()
{
#ifdef _DEBUG
    if (mpMode == 0)
    {
        // // Poll and handle messages (inputs, window resize, etc.)
        // // See the WndProc() function below for our to dispatch events to the Win32 backend.
        // MSG msg;
        // while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        // {
        //     ::TranslateMessage(&msg);
        //     ::DispatchMessage(&msg);
        //     if (msg.message == WM_QUIT)
        //         done = true;
        // }
        // if (done)
        //     return;

        // // Handle window being minimized or screen locked
        // if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        // {
        //     ::Sleep(10);
        //     return;
        // }
        // g_SwapChainOccluded = false;

        // // Handle window resize (we don't resize directly in the WM_SIZE handler)
        // if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        // {
        //     CleanupRenderTarget();
        //     g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
        //     g_ResizeWidth = g_ResizeHeight = 0;
        //     CreateRenderTarget();
        // }

        // // Start the Dear ImGui frame
        // ImGui_ImplDX11_NewFrame();
        // ImGui_ImplWin32_NewFrame();
        // ImGui::NewFrame();

        // // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        // if (show_demo_window)
        //     ImGui::ShowDemoWindow(&show_demo_window);

        // // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        // {
        //     static float f = 0.0f;
        //     static int counter = 0;

        //     ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        //     ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        //     ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        //     ImGui::Checkbox("Another Window", &show_another_window);

        //     ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        //     ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        //     if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        //         counter++;
        //     ImGui::SameLine();
        //     ImGui::Text("counter = %d", counter);

        //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
        //     ImGui::End();
        // }

        // // 3. Show another simple window.
        // if (show_another_window)
        // {
        //     ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        //     ImGui::Text("Hello from another window!");
        //     if (ImGui::Button("Close Me"))
        //         show_another_window = false;
        //     ImGui::End();
        // }
    }
    else if (mpMode == 1)
    {
        // ImGui フレーム開始
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        mnAddNumber = 0;

        for (int i = 0; i < mstImguiFloatDatas.size(); i++)
        {
            DrawFloatImgui(mstImguiFloatDatas[i]);
        }
        
        for (int i = 0; i < mstImguiIntDatas.size(); i++)
        {
            DrawIntImgui(mstImguiIntDatas[i]);
        }
    }
#endif
}

// 描画
void ImguiManager::Draw()
{
#ifdef _DEBUG	
    if (mpMode == 0)
    {
        // // Rendering
        // ImGui::Render();
        // ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
    else if (mpMode == 1)
    {
        // ImGui フレーム終了と描画
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // DXLib Direct3D設定を復元
        RefreshDxLibDirect3DSetting();
    }
#endif
}

// floatのImgui情報設定
/*
std::string ImguiManager::SetFloatImgui(IMGUI_FLOAT_DATA imguiFloatData)
{
#ifdef _DEBUG
   for (int i = 0; i < imguiFloatData.VariableDatas.size(); i++)
    {
        if (i >= 4){break; }
        imguiFloatData.ChangeVariable[i] = *(imguiFloatData.VariableDatas[i]);
        imguiFloatData.PreVariable[i] = *(imguiFloatData.VariableDatas[i]);
    }
    if (imguiFloatData.AddNumberDrawFlag == true)
    {
        imguiFloatData.Label = imguiFloatData.Label + std::to_string(mstImguiFloatDatas.size());
    }
    mstImguiFloatDatas.push_back(imguiFloatData);
    return imguiFloatData.Label;
#endif
    return "";
}*/

// intのImgui情報設定
/*
std::string ImguiManager::SetIntImgui(IMGUI_INT_DATA imguiIntData)
{
#ifdef _DEBUG
   for (int i = 0; i < imguiIntData.VariableDatas.size(); i++)
    {
        if (i >= 4) {break;}
        imguiIntData.ChangeVariable[i] = *(imguiIntData.VariableDatas[i]);
        imguiIntData.PreVariable[i] = *(imguiIntData.VariableDatas[i]);
    }
    if (imguiIntData.AddNumberDrawFlag == true)
    {
        imguiIntData.Label = imguiIntData.Label + std::to_string(mstImguiIntDatas.size());
    }
    mstImguiIntDatas.push_back(imguiIntData);
    return imguiIntData.Label;
#endif
    return "";
}
*/

// Imgui情報削除
/*
void ImguiManager::DeleteImguiData(std::string labelName)
{
#ifdef _DEBUG
    for (int i = 0; i < mstImguiFloatDatas.size(); i++)
    {
        if (labelName == mstImguiFloatDatas[i].Label.substr(0, mstImguiFloatDatas[i].Label.size() - 1))
        {
            mstImguiFloatDatas.erase(mstImguiFloatDatas.begin() + i);
            return;
        }
    }
    for (int i = 0; i < mstImguiIntDatas.size(); i++)
    {
        if (labelName == mstImguiIntDatas[i].Label.substr(0, mstImguiIntDatas[i].Label.size() - 1))
        {
            mstImguiIntDatas.erase(mstImguiIntDatas.begin() + i);
            return;
        }
    }


    for (int i = 0; i < mstImguiFloatDatas.size(); i++)
    {
        if (labelName == mstImguiFloatDatas[i].Label)
        {
            mstImguiFloatDatas.erase(mstImguiFloatDatas.begin() + i);
            return;
        }
    }
#endif
}
*/

// Imgui追加描画
void ImguiManager::AddDrawImgui(IMGUI_FLOAT_DATA imguiFloatData)
{
#ifdef _DEBUG
    if (imguiFloatData.AddNumberDrawFlag == true)
    {
        imguiFloatData.Label = imguiFloatData.Label + "_" + std::to_string(mnAddNumber);
    }
    ++mnAddNumber;
    DrawFloatImgui(imguiFloatData);
#endif
}
// Imgui追加描画
void ImguiManager::AddDrawImgui(IMGUI_INT_DATA imguiIntData)
{
#ifdef _DEBUG
    if (imguiIntData.AddNumberDrawFlag == true)
    {
        imguiIntData.Label = imguiIntData.Label + "_" + std::to_string(mnAddNumber);
    }
    ++mnAddNumber;
    DrawIntImgui(imguiIntData);
#endif
}

// floatのImguui描画
void ImguiManager::DrawFloatImgui(IMGUI_FLOAT_DATA imguiFloatData)
{
#ifdef _DEBUG
    for (int i = 0; i < imguiFloatData.VariableDatas.size(); i++)
    {
        if (i >= 4)
        {
            break;
        }
        if (UtilCalc::FloatEqual(*(imguiFloatData.VariableDatas[i]), imguiFloatData.PreVariable[i], UtilCalc::FloatError))
        {
            continue;
        }

        imguiFloatData.ChangeVariable[i] += *(imguiFloatData.VariableDatas[i]) - imguiFloatData.PreVariable[i];
    }
    switch (imguiFloatData.ImguiType)
    {
    case IMGUI_TYPE::SLIDER1:
        ImGui::SliderFloat(imguiFloatData.Label.c_str(), imguiFloatData.ChangeVariable, imguiFloatData.Min, imguiFloatData.Max, imguiFloatData.Format.c_str(), imguiFloatData.Flag);
        break;

    case IMGUI_TYPE::SLIDER2:
        ImGui::SliderFloat2(imguiFloatData.Label.c_str(), imguiFloatData.ChangeVariable, imguiFloatData.Min, imguiFloatData.Max, imguiFloatData.Format.c_str(), imguiFloatData.Flag);
        break;

    case IMGUI_TYPE::SLIDER3:
        ImGui::SliderFloat3(imguiFloatData.Label.c_str(), imguiFloatData.ChangeVariable, imguiFloatData.Min, imguiFloatData.Max, imguiFloatData.Format.c_str(), imguiFloatData.Flag);
        break;

    case IMGUI_TYPE::SLIDER4:
        ImGui::SliderFloat4(imguiFloatData.Label.c_str(), imguiFloatData.ChangeVariable, imguiFloatData.Min, imguiFloatData.Max, imguiFloatData.Format.c_str(), imguiFloatData.Flag);
        break;

    case IMGUI_TYPE::DRAG1:
        ImGui::DragFloat(imguiFloatData.Label.c_str(), imguiFloatData.ChangeVariable, imguiFloatData.Speed, imguiFloatData.Min, imguiFloatData.Max, imguiFloatData.Format.c_str(), imguiFloatData.Flag);
        break;

    case IMGUI_TYPE::DRAG2:
        ImGui::DragFloat2(imguiFloatData.Label.c_str(), imguiFloatData.ChangeVariable, imguiFloatData.Speed, imguiFloatData.Min, imguiFloatData.Max, imguiFloatData.Format.c_str(), imguiFloatData.Flag);
        break;

    case IMGUI_TYPE::DRAG3:
        ImGui::DragFloat3(imguiFloatData.Label.c_str(), imguiFloatData.ChangeVariable, imguiFloatData.Speed, imguiFloatData.Min, imguiFloatData.Max, imguiFloatData.Format.c_str(), imguiFloatData.Flag);
        break;

    case IMGUI_TYPE::DRAG4:
        ImGui::DragFloat4(imguiFloatData.Label.c_str(), imguiFloatData.ChangeVariable, imguiFloatData.Speed, imguiFloatData.Min, imguiFloatData.Max, imguiFloatData.Format.c_str(), imguiFloatData.Flag);
        break;

    case IMGUI_TYPE::INPUT1:
        ImGui::InputFloat(imguiFloatData.Label.c_str(), imguiFloatData.ChangeVariable, imguiFloatData.Step, imguiFloatData.StepFast, imguiFloatData.Format.c_str(), imguiFloatData.Flag);
        break;

    case IMGUI_TYPE::INPUT2:
        ImGui::InputFloat2(imguiFloatData.Label.c_str(), imguiFloatData.ChangeVariable, imguiFloatData.Format.c_str(), imguiFloatData.Flag);
        break;

    case IMGUI_TYPE::INPUT3:
        ImGui::InputFloat3(imguiFloatData.Label.c_str(), imguiFloatData.ChangeVariable, imguiFloatData.Format.c_str(), imguiFloatData.Flag);
        break;

    case IMGUI_TYPE::INPUT4:
        ImGui::InputFloat4(imguiFloatData.Label.c_str(), imguiFloatData.ChangeVariable, imguiFloatData.Format.c_str(), imguiFloatData.Flag);
        break;

    case IMGUI_TYPE::ANGLE:
        ImGui::SliderAngle(imguiFloatData.Label.c_str(), imguiFloatData.ChangeVariable, imguiFloatData.Min, imguiFloatData.Max, imguiFloatData.Format.c_str(), imguiFloatData.Flag);
        break;
    }

    for (int i = 0; i < imguiFloatData.VariableDatas.size(); i++)
    {
        if (i >= 4)
        {
            break;
        }

        *(imguiFloatData.VariableDatas[i]) = imguiFloatData.ChangeVariable[i];
        imguiFloatData.PreVariable[i] = *(imguiFloatData.VariableDatas[i]);
    }
#endif
}


// intのImguui描画
void ImguiManager::DrawIntImgui(IMGUI_INT_DATA imguiIntData)
{
#ifdef _DEBUG
    for (int i = 0; i < imguiIntData.VariableDatas.size(); i++)
    {
        if (i >= 4)
        {
            break;
        }

        if (*(imguiIntData.VariableDatas[i]) == imguiIntData.PreVariable[i])
        {
            continue;
        }

        imguiIntData.ChangeVariable[i] += *(imguiIntData.VariableDatas[i]) - imguiIntData.PreVariable[i];
    }
    switch (imguiIntData.ImguiType)
    {
    case IMGUI_TYPE::SLIDER1:
        ImGui::SliderInt(imguiIntData.Label.c_str(), imguiIntData.ChangeVariable, imguiIntData.Min, imguiIntData.Max, imguiIntData.Format.c_str(), imguiIntData.Flag);
        break;

    case IMGUI_TYPE::SLIDER2:
        ImGui::SliderInt2(imguiIntData.Label.c_str(), imguiIntData.ChangeVariable, imguiIntData.Min, imguiIntData.Max, imguiIntData.Format.c_str(), imguiIntData.Flag);
        break;

    case IMGUI_TYPE::SLIDER3:
        ImGui::SliderInt3(imguiIntData.Label.c_str(), imguiIntData.ChangeVariable, imguiIntData.Min, imguiIntData.Max, imguiIntData.Format.c_str(), imguiIntData.Flag);
        break;

    case IMGUI_TYPE::SLIDER4:
        ImGui::SliderInt4(imguiIntData.Label.c_str(), imguiIntData.ChangeVariable, imguiIntData.Min, imguiIntData.Max, imguiIntData.Format.c_str(), imguiIntData.Flag);
        break;

    case IMGUI_TYPE::DRAG1:
        ImGui::DragInt(imguiIntData.Label.c_str(), imguiIntData.ChangeVariable, imguiIntData.Speed, imguiIntData.Min, imguiIntData.Max, imguiIntData.Format.c_str(), imguiIntData.Flag);
        break;

    case IMGUI_TYPE::DRAG2:
        ImGui::DragInt2(imguiIntData.Label.c_str(), imguiIntData.ChangeVariable, imguiIntData.Speed, imguiIntData.Min, imguiIntData.Max, imguiIntData.Format.c_str(), imguiIntData.Flag);
        break;

    case IMGUI_TYPE::DRAG3:
        ImGui::DragInt3(imguiIntData.Label.c_str(), imguiIntData.ChangeVariable, imguiIntData.Speed, imguiIntData.Min, imguiIntData.Max, imguiIntData.Format.c_str(), imguiIntData.Flag);
        break;

    case IMGUI_TYPE::DRAG4:
        ImGui::DragInt4(imguiIntData.Label.c_str(), imguiIntData.ChangeVariable, imguiIntData.Speed, imguiIntData.Min, imguiIntData.Max, imguiIntData.Format.c_str(), imguiIntData.Flag);
        break;

    case IMGUI_TYPE::INPUT1:
        ImGui::InputInt(imguiIntData.Label.c_str(), imguiIntData.ChangeVariable, imguiIntData.Step, imguiIntData.StepFast, imguiIntData.Flag);
        break;

    case IMGUI_TYPE::INPUT2:
        ImGui::InputInt2(imguiIntData.Label.c_str(), imguiIntData.ChangeVariable, imguiIntData.Flag);
        break;

    case IMGUI_TYPE::INPUT3:
        ImGui::InputInt3(imguiIntData.Label.c_str(), imguiIntData.ChangeVariable, imguiIntData.Flag);
        break;

    case IMGUI_TYPE::INPUT4:
        ImGui::InputInt4(imguiIntData.Label.c_str(), imguiIntData.ChangeVariable, imguiIntData.Flag);
        break;
    }

    for (int i = 0; i < imguiIntData.VariableDatas.size(); i++)
    {
        if (i >= 4)
        {
            break;
        }

        *(imguiIntData.VariableDatas[i]) = imguiIntData.ChangeVariable[i];
        imguiIntData.PreVariable[i] = *(imguiIntData.VariableDatas[i]);
    }
#endif
}

// // Helper functions

// bool CreateDeviceD3D(HWND hWnd)
// {
//     // Setup swap chain
//     // This is a basic setup. Optimally could use e.g. DXGI_SWAP_EFFECT_FLIP_DISCARD and handle fullscreen mode differently. See #8979 for suggestions.
//     DXGI_SWAP_CHAIN_DESC sd;
//     ZeroMemory(&sd, sizeof(sd));
//     sd.BufferCount = 2;
//     sd.BufferDesc.Width = 0;
//     sd.BufferDesc.Height = 0;
//     sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//     sd.BufferDesc.RefreshRate.Numerator = 60;
//     sd.BufferDesc.RefreshRate.Denominator = 1;
//     sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
//     sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//     sd.OutputWindow = hWnd;
//     sd.SampleDesc.Count = 1;
//     sd.SampleDesc.Quality = 0;
//     sd.Windowed = TRUE;
//     sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

//     UINT createDeviceFlags = 0;
//     //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
//     D3D_FEATURE_LEVEL featureLevel;
//     const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
//     HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
//     if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
//         res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
//     if (res != S_OK)
//         return false;

//     CreateRenderTarget();
//     return true;
// }

// void CleanupDeviceD3D()
// {
//     CleanupRenderTarget();
//     if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
//     if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
//     if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
// }

// void CreateRenderTarget()
// {
//     ID3D11Texture2D* pBackBuffer;
//     g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
//     g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
//     pBackBuffer->Release();
// }

// void CleanupRenderTarget()
// {
//     if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
// }

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI ImguiWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    //    return true;

    //switch (msg)
    //{
    //case WM_SIZE:
    //    if (wParam == SIZE_MINIMIZED)
    //        return 0;
    //    g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
    //    g_ResizeHeight = (UINT)HIWORD(lParam);
    //    return 0;
    //case WM_SYSCOMMAND:
    //    if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
    //        return 0;
    //    break;
    //case WM_DESTROY:
    //    ::PostQuitMessage(0);
    //    return 0;
    //}
    //return ::DefWindowProcW(hWnd, msg, wParam, lParam);

    ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

    return 0;
}


/*

#include "DxLib.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

	// メッセージ処理はDxLibで行っているようだ
	
	return 0;
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				 LPSTR lpCmdLine, int nCmdShow )
{
	int gh = -1; // グラフィックハンドル

	// 初期設定
	{
		// DxLib
		{
			DxLib::SetGraphMode(1280, 720, 32);
			DxLib::ChangeWindowMode(true);
			DxLib::SetUseDirect3DVersion(DX_DIRECT3D_11);
			DxLib::SetHookWinProc(WndProc);
			DxLib::SetAlwaysRunFlag(true);
			if (DxLib::DxLib_Init() == -1) { return false; }
			DxLib::SetDrawScreen(DX_SCREEN_BACK);

			gh = LoadGraph("test1.bmp"); // ★このファイルはDxLibのサンプルに入ってたものです(キャラ画像)
		}

		// ImGui
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows　★ここ消すとマルチウィンドウ解除

			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			ImGui_ImplWin32_Init(DxLib::GetMainWindowHandle());
			ImGui_ImplDX11_Init((ID3D11Device*)DxLib::GetUseDirect3D11Device(), (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext());
		}
	}

	// メインループ
	while (!DxLib::ProcessMessage())
	{
		// 更新部
		{
			// DxLib
			{
				 // 特になし
			}

			// ImGui
			{
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();
				ImGui::ShowDemoWindow();
			}
		}

		// 描画部
		{
			DxLib::ClearDrawScreen();

			// DxLib
			{
				DrawGraph(0, 0, gh, true); // ★これが消えてしまう
			}

			// ImGui
			{
				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
				// Update and Render additional Platform Windows
				if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault(); // ★これが怪しい
				}
			}

			DxLib::ScreenFlip();
		}
	}

	// 終了処理
	{
		// ImGui
		{
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
		}

		// DxLib
		{
			DxLib::DxLib_End();
		}
	}

	return 0;
}
*/