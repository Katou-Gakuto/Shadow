
//* @note リファレンス https://dxlib.xsrv.jp/function/dxfunc_input.html#R5N4


#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);
	SetGraphMode(640, 480, 32);
	if (DxLib_Init() == -1)return -1;

	SetDrawScreen(DX_SCREEN_BACK);
	int Graph = LoadGraph("Jump.png");

	while (ProcessMessage() == 0)
	{
		ClearDrawScreen();
		DrawRectExtendGraph(0, 0, 150, 150, 0, 0, 150, 150, Graph, false);
		DrawRectExtendGraph(0, 150, 150, 300, 0, 0, 150, 150, Graph, true);

		ScreenFlip();
	}
	DxLib_End();
	return 0;
}