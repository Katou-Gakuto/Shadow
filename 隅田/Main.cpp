
//* @note リファレンス https://dxlib.xsrv.jp/function/dxfunc_input.html#R5N4


#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);
	SetGraphMode(640, 480, 32);
	if (DxLib_Init() == -1)return -1;

	SetDrawScreen(DX_SCREEN_BACK);
	int Key, color;

	while (ProcessMessage() == 0)
	{
		ClearDrawScreen();

		{
			//使用するパッドを指定する
			Key = GetJoypadInputState(DX_INPUT_PAD1);

			//十字キー
			if (Key & PAD_INPUT_DOWN) {
				color = 0xff0000;
			}
			else {
				color = 0x800000;
			}
			DrawTriangle(48, 80, 32, 64, 64, 64, color, TRUE);

			if (Key & PAD_INPUT_LEFT) {
				color = 0xff0000;
			}
			else {
				color = 0x800000;
			}
			DrawTriangle(16, 48, 32, 32, 32, 64, color, TRUE);

			if (Key & PAD_INPUT_RIGHT) {
				color = 0xff0000;
			}
			else {
				color = 0x800000;
			}
			DrawTriangle(80, 48, 64, 32, 64, 64, color, TRUE);

			if (Key & PAD_INPUT_UP) {
				color = 0xff0000;
			}
			else {
				color = 0x800000;
			}
			DrawTriangle(48, 16, 32, 32, 64, 32, color, TRUE);

			//ボタン
			for (int i = 0; i < 28; i++) {
				if (Key & (PAD_INPUT_1 << i)) {
					color = 0xff0000;
				}
				else {
					color = 0x800000;
				}
				DrawCircle(128 + 32 * (i % 5), 32 + 32 * (i / 5), 14, color, TRUE);
			}
		}


		{
			// DirectInput

			DINPUT_JOYSTATE input;
			int Color;

			// 入力状態を取得
			GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

			// 画面に構造体の中身を描画
			Color = GetColor(255, 255, 255);

			// 左スティック(ZはLT,RTトリガー)
			DrawFormatString(0, 100, Color, "X:%d Y:%d Z:%d",
				input.X, input.Y, input.Z);

			// 右スティック
			DrawFormatString(0, 116, Color, "Rx:%d Ry:%d Rz:%d",
				input.Rx, input.Ry, input.Rz);

			// 何かに使う
			DrawFormatString(0, 132, Color, "Slider 0:%d 1:%d",
				input.Slider[0], input.Slider[1]);

			// 十字キー (POV[0]に入る)
			// ニュートラル -1
			// 上   0
			// 下   18000
			// 右   9000
			// 左   27000
			// 右上 4500
			// 左上 31500
			// 右下 13500
			// 左下 22500
			DrawFormatString(0, 148, Color, "POV 0:%d 1:%d 2:%d 3:%d",
				input.POV[0], input.POV[1],
				input.POV[2], input.POV[3]);


			DrawString(0, 64, "Button", Color);
			for (int i = 0; i < 32; i++)
			{
				DrawFormatString(64 + i % 8 * 64, (64 + i / 8 * 16) + 100, Color,
					"%2d:%d", i, input.Buttons[i]);
			}
		}
		

		{
			// XInput

			XINPUT_STATE Xinput;
			int Color;

			// 入力状態を取得
			GetJoypadXInputState(DX_INPUT_PAD1, &Xinput);

			// 画面に構造体の中身を描画
			Color = GetColor(255, 255, 255);

			// 左スティック(感度高すぎるので非推奨)
			DrawFormatString(0, 230, Color, "X:%d Y:%d",
				Xinput.ThumbLX, Xinput.ThumbLY);

			// 右スティック(感度高すぎるので非推奨)
			DrawFormatString(0, 246, Color, "Rx:%d Ry:%d",
				Xinput.ThumbRX, Xinput.ThumbRY);

			// LRトリガー(最大値は255)
			DrawFormatString(0, 262, Color, "Trigger L:%d R:%d",
				Xinput.LeftTrigger, Xinput.RightTrigger);


			DrawString(0, 290, "Button", Color);
			for (int i = 0; i < 16; i++)
			{
				DrawFormatString(64 + i % 8 * 64, (64 + i / 8 * 16) + 268, Color,
					"%2d:%d", i, Xinput.Buttons[i]);
			}
		}

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}