#include "DxLib.h"

#include "Master.h"

#include "KeyState.h"



LRESULT CALLBACK MyWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    MSG setMsg = {};
    setMsg.message = msg;
    setMsg.wParam = wp;
    setMsg.lParam = lp;
    Master::mpKeyState->SetMsg(setMsg);

    return 0;
}

void KeyState::Initialize()
{
    SetHookWinProc(MyWndProc);
}