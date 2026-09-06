#include <iostream>
#include <Windows.h>
#include <Xinput.h>

#include "KeyStateEnum.h"
#include "BitFlag.h"
#include "Vector2.h"

#include "KeyState.h"

#pragma comment(lib, "Xinput.lib")

/*--------*/
/*【共通】*/
/*--------*/

// コンストラクタ
KeyState::KeyState()
: mshWheelVolume(0)
, mucToggleFlag(BIT_FLAG<unsigned char>())
, mdxsMsg()
, mbMsgSetFlag(false)
{
	// キーフラグ(コントローラー)
	memset(munControllerKeyFlags, 0, sizeof(BIT_FLAG<unsigned int>) * (int)FLAG_TYPE::MAX * (int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER);

	// キー変動値
	memset(mshNowRightStickX, 0, sizeof(short) * (int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER);
	memset(mshNowRightStickY, 0, sizeof(short) * (int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER);
	memset(mshNowLeftStickX, 0, sizeof(short) * (int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER);
	memset(mshNowLeftStickY, 0, sizeof(short) * (int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER);
	memset(mshNowRightTrigger, 0, sizeof(short) * (int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER);
	memset(mshNowLeftTrigger, 0, sizeof(short) * (int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER);

	// キーフラグ(キーボード)
	memset(mullWordFlags, 0, sizeof(BIT_FLAG<unsigned long long>) * (int)FLAG_TYPE::MAX);
	memset(mulSpecialFlags, 0, sizeof(BIT_FLAG<unsigned long>) * (int)FLAG_TYPE::MAX);
	memset(mushNumpadFlags, 0, sizeof(BIT_FLAG<unsigned short>) * (int)FLAG_TYPE::MAX);

	// キーフラグ(マウス)
	munMouseFlags.Init();

	for (int i = 0; i < (int)CURSOR_POSITION_TYPE::MAX; i++)
	{
		mstCursorPos[i] = Vector2::Zero();
	}
}

// デストラクタ
KeyState::~KeyState()
{
}

// 更新
void KeyState::Update()
{
	// コントローラーのフラグを保存
	BIT_FLAG<unsigned int> prevControllerKey[CONTROLLER_MAX];
	memcpy(prevControllerKey, munControllerKeyFlags[(int)FLAG_TYPE::NOW], sizeof(BIT_FLAG<unsigned int>) * CONTROLLER_MAX);

	// マウスのフラグを保存
	unsigned int prevMouseKey = munMouseFlags.flags;

	SetKey();

	// 押しはじめと離したコントローラーフラグを設定
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		munControllerKeyFlags[(int)FLAG_TYPE::DOWN][i] = ((~prevControllerKey[i].flags) & munControllerKeyFlags[(int)FLAG_TYPE::NOW][i]);
		munControllerKeyFlags[(int)FLAG_TYPE::UP][i] = (prevControllerKey[i].flags & (~munControllerKeyFlags[(int)FLAG_TYPE::NOW][i]));
	}

	// マウスフラグを設定
	munMouseFlags = (0b1'1111'1111u & munMouseFlags) | //now
					(((~prevMouseKey) & (0b1'1111'1111u & munMouseFlags)) << ((int)MOUSE_TYPE::MAX)) | // down
					((prevMouseKey & (~(0b1'1111'1111u & munMouseFlags))) << ((int)MOUSE_TYPE::MAX + (int)MOUSE_TYPE::MAX)); // up
}

// キーフラグを設定(コントローラーは現在のキーのみ設定)
void KeyState::SetKey()
{

	int controllerNumber = (int)CONTROLLER_KEY_NUMBER::CONTROLLER_1;
	// 【コントローラー】
	for (int i = controllerNumber; i < (4 + controllerNumber); i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// 取得
		unsigned long result = XInputGetState(i - controllerNumber, &state);

		// 接続がないなら何もしない
		if (result == ERROR_SUCCESS)
		{
			SetNowKey_Controller(true, CONTROLLER_KEY_TYPE::EXISTENCE, i);
			// ボタン取得
			unsigned short buttons = state.Gamepad.wButtons;

			SetNowKey_Controller(buttons & XINPUT_GAMEPAD_A, CONTROLLER_KEY_TYPE::A, i);
			SetNowKey_Controller(buttons & XINPUT_GAMEPAD_B, CONTROLLER_KEY_TYPE::B, i);
			SetNowKey_Controller(buttons & XINPUT_GAMEPAD_X, CONTROLLER_KEY_TYPE::X, i);
			SetNowKey_Controller(buttons & XINPUT_GAMEPAD_Y, CONTROLLER_KEY_TYPE::Y, i);

			SetNowKey_Controller(buttons & XINPUT_GAMEPAD_DPAD_LEFT, CONTROLLER_KEY_TYPE::LEFT, i);
			SetNowKey_Controller(buttons & XINPUT_GAMEPAD_DPAD_RIGHT, CONTROLLER_KEY_TYPE::RIGHT, i);
			SetNowKey_Controller(buttons & XINPUT_GAMEPAD_DPAD_UP, CONTROLLER_KEY_TYPE::UP, i);
			SetNowKey_Controller(buttons & XINPUT_GAMEPAD_DPAD_DOWN, CONTROLLER_KEY_TYPE::DOWN, i);

			SetNowKey_Controller(buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER, CONTROLLER_KEY_TYPE::R, i);
			SetNowKey_Controller(buttons & XINPUT_GAMEPAD_LEFT_SHOULDER, CONTROLLER_KEY_TYPE::L, i);

			SetNowKey_Controller(buttons & XINPUT_GAMEPAD_LEFT_THUMB, CONTROLLER_KEY_TYPE::LEFT_STICK_PUSH, i);
			SetNowKey_Controller(buttons & XINPUT_GAMEPAD_RIGHT_THUMB, CONTROLLER_KEY_TYPE::RIGHT_STICK_PUSH, i);

			SetNowKey_Controller(buttons & XINPUT_GAMEPAD_START, CONTROLLER_KEY_TYPE::START, i);
			SetNowKey_Controller(buttons & XINPUT_GAMEPAD_BACK, CONTROLLER_KEY_TYPE::BACK, i);

			// RT LT 取得
			mshNowLeftTrigger[i] = state.Gamepad.bLeftTrigger;
			mshNowRightTrigger[i] = state.Gamepad.bRightTrigger;
			SetNowKey_Controller(mshNowLeftTrigger[i] > 0, CONTROLLER_KEY_TYPE::LT, i);
			SetNowKey_Controller(mshNowRightTrigger[i] > 0, CONTROLLER_KEY_TYPE::RT, i);

			// スティック取得
			mshNowLeftStickX[i] = state.Gamepad.sThumbLX;
			mshNowLeftStickY[i] = state.Gamepad.sThumbLY;

			mshNowRightStickX[i] = state.Gamepad.sThumbRX;
			mshNowRightStickY[i] = state.Gamepad.sThumbRY;

			SetNowKey_Controller(mshNowLeftStickX[i] > 0, CONTROLLER_KEY_TYPE::LEFT_STICK_RIGHT, i);
			SetNowKey_Controller(mshNowLeftStickX[i] < 0, CONTROLLER_KEY_TYPE::LEFT_STICK_LEFT, i);

			SetNowKey_Controller(mshNowLeftStickY[i] < 0, CONTROLLER_KEY_TYPE::LEFT_STICK_UP, i);
			SetNowKey_Controller(mshNowLeftStickY[i] > 0, CONTROLLER_KEY_TYPE::LEFT_STICK_DOWN, i);

			SetNowKey_Controller(mshNowRightStickX[i] > 0, CONTROLLER_KEY_TYPE::RIGHT_STICK_RIGHT, i);
			SetNowKey_Controller(mshNowRightStickX[i] < 0, CONTROLLER_KEY_TYPE::RIGHT_STICK_LEFT, i);

			SetNowKey_Controller(mshNowRightStickY[i] < 0, CONTROLLER_KEY_TYPE::RIGHT_STICK_UP, i);
			SetNowKey_Controller(mshNowRightStickY[i] > 0, CONTROLLER_KEY_TYPE::RIGHT_STICK_DOWN, i);
		}
	}


	// 【キーボード】	
	SetNowKey_Controller(true, CONTROLLER_KEY_TYPE::EXISTENCE);

	unsigned char getKey[256] = { 0 };
	// 取得と同時に押したキーがあるかを確認
	if (GetKeyboardState(getKey))
	{
		// コントローラー準拠でキーを取得
		{
			SetNowKey_Controller((getKey['D'] & 0x80) != 0, CONTROLLER_KEY_TYPE::RIGHT);
			SetNowKey_Controller((getKey[VK_RIGHT] & 0x80) != 0, CONTROLLER_KEY_TYPE::RIGHT);

			SetNowKey_Controller((getKey['A'] & 0x80) != 0, CONTROLLER_KEY_TYPE::LEFT);
			SetNowKey_Controller((getKey[VK_LEFT] & 0x80) != 0, CONTROLLER_KEY_TYPE::LEFT);

			SetNowKey_Controller((getKey['W'] & 0x80) != 0, CONTROLLER_KEY_TYPE::UP);
			SetNowKey_Controller((getKey[VK_UP] & 0x80) != 0, CONTROLLER_KEY_TYPE::UP);

			SetNowKey_Controller((getKey['S'] & 0x80) != 0, CONTROLLER_KEY_TYPE::DOWN);
			SetNowKey_Controller((getKey[VK_DOWN] & 0x80) != 0, CONTROLLER_KEY_TYPE::DOWN);

			SetNowKey_Controller((getKey['L'] & 0x80) != 0, CONTROLLER_KEY_TYPE::L);
			SetNowKey_Controller((getKey['R'] & 0x80) != 0, CONTROLLER_KEY_TYPE::R);
			SetNowKey_Controller((getKey['T'] & 0x80) != 0, CONTROLLER_KEY_TYPE::RT);

			SetNowKey_Controller((getKey[VK_RETURN] & 0x80) != 0, CONTROLLER_KEY_TYPE::A);// エンターキー

			SetNowKey_Controller((getKey['X'] & 0x80) != 0, CONTROLLER_KEY_TYPE::X);

			SetNowKey_Controller((getKey['Y'] & 0x80) != 0, CONTROLLER_KEY_TYPE::Y);

			SetNowKey_Controller((getKey[VK_BACK] & 0x80) != 0, CONTROLLER_KEY_TYPE::B);	// バックスペース


			SetNowKey_Controller((getKey['P'] & 0x80) != 0, CONTROLLER_KEY_TYPE::LEFT_STICK_PUSH);	// バックスペース
		}

		// キーボードの状態を取得
		SetKey_Board(getKey);

		// ここだけマウス
		// クリック
		SetMouseFlag((getKey[1] & 0x80) != 0, MOUSE_TYPE::LEFT_BUTTON);
		SetMouseFlag((getKey[2] & 0x80) != 0, MOUSE_TYPE::RIGHT_BUTTON);
		SetMouseFlag((getKey[4] & 0x80) != 0, MOUSE_TYPE::WHEEL_BUTTON);
	}
	else
	{
	}


	//【マウス】
	{

		// カーソル
		POINT cursorPos;
		if (GetCursorPos(&cursorPos))
		{
			bool startPosFlag = false;

			if (mstCursorPos[(int)CURSOR_POSITION_TYPE::NOW].IntX() == cursorPos.x &&
				mstCursorPos[(int)CURSOR_POSITION_TYPE::NOW].IntY() == cursorPos.y)
			{
				startPosFlag = true;
			}
			else
			{
				Vector2 moveVecs[2] = { Vector2::Zero(), Vector2::Zero() };
				moveVecs[0] = mstCursorPos[(int)CURSOR_POSITION_TYPE::PREV] - mstCursorPos[(int)CURSOR_POSITION_TYPE::NOW];
				moveVecs[1].x = mstCursorPos[(int)CURSOR_POSITION_TYPE::NOW].IntX() - (float)cursorPos.x;
				moveVecs[1].y = mstCursorPos[(int)CURSOR_POSITION_TYPE::NOW].IntY() - (float)cursorPos.y;

				if ((moveVecs[0].IntX() > 0) != (moveVecs[1].IntX() > 0) ||
					(moveVecs[0].IntY() > 0) != (moveVecs[1].IntY() > 0))
				{
					startPosFlag = true;
				}
				else if ((moveVecs[0].IntX() > moveVecs[0].IntY()) != (moveVecs[1].IntX() > moveVecs[1].IntY()))
				{
					startPosFlag = true;
				}

			}

			mstCursorPos[(int)CURSOR_POSITION_TYPE::PREV] = mstCursorPos[(int)CURSOR_POSITION_TYPE::NOW];
			mstCursorPos[(int)CURSOR_POSITION_TYPE::NOW].x = (float)cursorPos.y;
			mstCursorPos[(int)CURSOR_POSITION_TYPE::NOW].y = (float)cursorPos.x;

			if (startPosFlag)
			{
				mstCursorPos[(int)CURSOR_POSITION_TYPE::START] = mstCursorPos[(int)CURSOR_POSITION_TYPE::NOW];
			}

			if (cursorPos.x > mstCursorPos[(int)CURSOR_POSITION_TYPE::PREV].IntX())
			{
				SetMouseFlag(true, MOUSE_TYPE::MOVE_RIGHT);
				SetMouseFlag(false, MOUSE_TYPE::MOVE_LEFT);
			}
			else if (cursorPos.x < mstCursorPos[(int)CURSOR_POSITION_TYPE::PREV].IntX())
			{
				SetMouseFlag(false, MOUSE_TYPE::MOVE_RIGHT);
				SetMouseFlag(true, MOUSE_TYPE::MOVE_LEFT);
			}
			else
			{
				SetMouseFlag(false, MOUSE_TYPE::MOVE_RIGHT);
				SetMouseFlag(false, MOUSE_TYPE::MOVE_LEFT);
			}

			if (cursorPos.y > mstCursorPos[(int)CURSOR_POSITION_TYPE::PREV].IntY())
			{
				SetMouseFlag(true, MOUSE_TYPE::MOVE_FRONT);
				SetMouseFlag(false, MOUSE_TYPE::MOVE_BACK);
			}
			else if (cursorPos.y < mstCursorPos[(int)CURSOR_POSITION_TYPE::PREV].IntY())
			{
				SetMouseFlag(false, MOUSE_TYPE::MOVE_FRONT);
				SetMouseFlag(true, MOUSE_TYPE::MOVE_BACK);
			}
			else
			{
				SetMouseFlag(false, MOUSE_TYPE::MOVE_FRONT);
				SetMouseFlag(false, MOUSE_TYPE::MOVE_BACK);
			}
		}
		else
		{
			SetMouseFlag(false, MOUSE_TYPE::MOVE_RIGHT);
			SetMouseFlag(false, MOUSE_TYPE::MOVE_LEFT);
			SetMouseFlag(false, MOUSE_TYPE::MOVE_FRONT);
			SetMouseFlag(false, MOUSE_TYPE::MOVE_BACK);
		}

		//マウスホイール
		if ((mbMsgSetFlag) && (WM_MOUSEWHEEL == mdxsMsg.message))
		{
			mbMsgSetFlag = false;
			mshWheelVolume = GET_WHEEL_DELTA_WPARAM(mdxsMsg.wParam);



			if (mshWheelVolume == 0)
			{
				SetMouseFlag(false, MOUSE_TYPE::WHEEL_FRONT);
				SetMouseFlag(false, MOUSE_TYPE::WHEEL_BACK);
			}
			else if (mshWheelVolume > 0)
			{
				SetMouseFlag(false, MOUSE_TYPE::WHEEL_FRONT);
				SetMouseFlag(true, MOUSE_TYPE::WHEEL_BACK);
			}
			else
			{
				SetMouseFlag(true, MOUSE_TYPE::WHEEL_FRONT);
				SetMouseFlag(false, MOUSE_TYPE::WHEEL_BACK);
			}
		}
		else
		{
			mshWheelVolume = 0;
			SetMouseFlag(false, MOUSE_TYPE::WHEEL_FRONT);
			SetMouseFlag(false, MOUSE_TYPE::WHEEL_BACK);
		}
	}

	// すべてのキーを設定した後にゲームで使うキーを判定
	SetKey_ShadowGame();
}

/*----------------------------------------------------------------------------------------------------
* 【シャドウゲーム用】
*/
// シャドウゲームで使用するキーを設定
void KeyState::SetKey_ShadowGame()
{
	// 一つ前の状態を保存
	unsigned long long prevShadowGameFlags = mulShadowGameFlags[(int)FLAG_TYPE::NOW];
	mulShadowGameFlags[(int)FLAG_TYPE::NOW] = 0;

	// now設定
	{
		// 移動
		{
			// 上
			mulShadowGameFlags[(int)FLAG_TYPE::NOW].flags |= ((GetWordKey_Board(KEY_BOARD_WORD::W) ? 1u : 0u) << (int)KEY_SHADOW_GAME_TYPE::UP);
			mulShadowGameFlags[(int)FLAG_TYPE::NOW].flags |= ((GetWordKey_Board(KEY_BOARD_WORD::ARROW_UP) ? 1u : 0u) << (int)KEY_SHADOW_GAME_TYPE::UP);

			// 下
			mulShadowGameFlags[(int)FLAG_TYPE::NOW].flags |= ((GetWordKey_Board(KEY_BOARD_WORD::S) ? 1u : 0u) << (int)KEY_SHADOW_GAME_TYPE::DOWN);
			mulShadowGameFlags[(int)FLAG_TYPE::NOW].flags |= ((GetWordKey_Board(KEY_BOARD_WORD::ARROW_DOWN) ? 1u : 0u) << (int)KEY_SHADOW_GAME_TYPE::DOWN);

			// 左
			mulShadowGameFlags[(int)FLAG_TYPE::NOW].flags |= ((GetWordKey_Board(KEY_BOARD_WORD::A) ? 1u : 0u) << (int)KEY_SHADOW_GAME_TYPE::LEFT);
			mulShadowGameFlags[(int)FLAG_TYPE::NOW].flags |= ((GetWordKey_Board(KEY_BOARD_WORD::ARROW_LEFT) ? 1u : 0u) << (int)KEY_SHADOW_GAME_TYPE::LEFT);

			// 右
			mulShadowGameFlags[(int)FLAG_TYPE::NOW].flags |= ((GetWordKey_Board(KEY_BOARD_WORD::D) ? 1u : 0u) << (int)KEY_SHADOW_GAME_TYPE::RIGHT);
			mulShadowGameFlags[(int)FLAG_TYPE::NOW].flags |= ((GetWordKey_Board(KEY_BOARD_WORD::ARROW_RIGHT) ? 1u : 0u) << (int)KEY_SHADOW_GAME_TYPE::RIGHT);

			// 移動(どれか)
			mulShadowGameFlags[(int)FLAG_TYPE::NOW].flags |= (((mulShadowGameFlags[(int)FLAG_TYPE::NOW].GetNumber(0xf, KEY_SHADOW_GAME_TYPE::UP) != 0) ? 1u : 0u) << (int)KEY_SHADOW_GAME_TYPE::MOVE);
		}

		/*
		* ここに追加
		*/
	}

	// downとupを設定
	{
		mulShadowGameFlags[(int)FLAG_TYPE::DOWN] = ((~prevShadowGameFlags) & mulShadowGameFlags[(int)FLAG_TYPE::NOW]);
		mulShadowGameFlags[(int)FLAG_TYPE::UP] = (prevShadowGameFlags & (~mulShadowGameFlags[(int)FLAG_TYPE::NOW]));
	}
}

/*------------------*/
/*【コントローラー】*/
/*------------------*/

// 押し始めたキーフラグを全コントローラー分全取得(コントローラー)
BIT_FLAG<unsigned int> KeyState::GetAllDownKeyFlags_Controller(bool keyBoardFlag)  const
{
	unsigned int result = (unsigned int)0;

	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		if (!keyBoardFlag && (i == (int)CONTROLLER_KEY_NUMBER::KEY_BOARD))
		{
			continue;
		}
		result |= munControllerKeyFlags[(int)FLAG_TYPE::DOWN][i].flags;
	}

	return result;
}

// 押しているキーフラグを全コントローラー分全取得(コントローラー)
BIT_FLAG<unsigned int> KeyState::GetAllNowKeyFlags_Controller(bool keyBoardFlag)  const
{
	unsigned int result = (unsigned int)0;

	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		if (!keyBoardFlag && (i == (int)CONTROLLER_KEY_NUMBER::KEY_BOARD))
		{
			continue;
		}
		result |= munControllerKeyFlags[(int)FLAG_TYPE::NOW][i].flags;
	}

	return result;
}

// 離したキーフラグを全コントローラー分全取得(コントローラー)
BIT_FLAG<unsigned int> KeyState::GetAllUpKeyFlags_Controller(bool keyBoardFlag)  const
{
	unsigned int result = (unsigned int)0;

	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		if (!keyBoardFlag && (i == (int)CONTROLLER_KEY_NUMBER::KEY_BOARD))
		{
			continue;
		}
		result |= munControllerKeyFlags[(int)FLAG_TYPE::UP][i].flags;
	}

	return result;
}

// キーの変動値取得(コントローラー)
short KeyState::GetKeyVariable_Controller(CONTROLLER_KEY_VARIABLE_NUMBER getNumber, int controllerNumber)  const
{
	switch (getNumber)
	{
	case CONTROLLER_KEY_VARIABLE_NUMBER::STICK_RIGHT_X:
		return mshNowRightStickX[controllerNumber];

	case CONTROLLER_KEY_VARIABLE_NUMBER::STICK_RIGHT_Y:
		return mshNowRightStickY[controllerNumber];

	case CONTROLLER_KEY_VARIABLE_NUMBER::STICK_LEFT_X:
		return mshNowLeftStickX[controllerNumber];

	case CONTROLLER_KEY_VARIABLE_NUMBER::STICK_LEFT_Y:
		return mshNowLeftStickY[controllerNumber];

	case CONTROLLER_KEY_VARIABLE_NUMBER::RIGHT_TRIGGER:
		return mshNowRightTrigger[controllerNumber];

	case CONTROLLER_KEY_VARIABLE_NUMBER::LEFT_TRIGGER:
		return mshNowLeftTrigger[controllerNumber];
	}

	return (short)0;
}

/*--------------*/
/*【キーボード】*/
/*--------------*/

//キーフラグを設定する(キーボード)
void KeyState::SetKey_Board(unsigned char* key)
{
	// 一つ前の状態を保存
	unsigned long long prevWordFlags = mullWordFlags[(int)FLAG_TYPE::NOW];
	unsigned long prevSpecialFlags = mulSpecialFlags[(int)FLAG_TYPE::NOW];
	unsigned short prevNumpadFlags = mushNumpadFlags[(int)FLAG_TYPE::NOW];

	// 取得
	{
		// 文字操作系キーフラグ
		{
			// 数字キー
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::_0] & 0x80) != 0, KEY_BOARD_WORD::_0);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::_1] & 0x80) != 0, KEY_BOARD_WORD::_1);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::_2] & 0x80) != 0, KEY_BOARD_WORD::_2);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::_3] & 0x80) != 0, KEY_BOARD_WORD::_3);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::_4] & 0x80) != 0, KEY_BOARD_WORD::_4);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::_5] & 0x80) != 0, KEY_BOARD_WORD::_5);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::_6] & 0x80) != 0, KEY_BOARD_WORD::_6);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::_7] & 0x80) != 0, KEY_BOARD_WORD::_7);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::_8] & 0x80) != 0, KEY_BOARD_WORD::_8);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::_9] & 0x80) != 0, KEY_BOARD_WORD::_9);

			// 文字キー
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::A] & 0x80) != 0, KEY_BOARD_WORD::A);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::B] & 0x80) != 0, KEY_BOARD_WORD::B);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::C] & 0x80) != 0, KEY_BOARD_WORD::C);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::D] & 0x80) != 0, KEY_BOARD_WORD::D);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::E] & 0x80) != 0, KEY_BOARD_WORD::E);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::F] & 0x80) != 0, KEY_BOARD_WORD::F);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::G] & 0x80) != 0, KEY_BOARD_WORD::G);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::H] & 0x80) != 0, KEY_BOARD_WORD::H);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::I] & 0x80) != 0, KEY_BOARD_WORD::I);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::J] & 0x80) != 0, KEY_BOARD_WORD::J);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::K] & 0x80) != 0, KEY_BOARD_WORD::K);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::L] & 0x80) != 0, KEY_BOARD_WORD::L);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::M] & 0x80) != 0, KEY_BOARD_WORD::M);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::N] & 0x80) != 0, KEY_BOARD_WORD::N);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::O] & 0x80) != 0, KEY_BOARD_WORD::O);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::P] & 0x80) != 0, KEY_BOARD_WORD::P);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::Q] & 0x80) != 0, KEY_BOARD_WORD::Q);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::R] & 0x80) != 0, KEY_BOARD_WORD::R);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::S] & 0x80) != 0, KEY_BOARD_WORD::S);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::T] & 0x80) != 0, KEY_BOARD_WORD::T);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::U] & 0x80) != 0, KEY_BOARD_WORD::U);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::V] & 0x80) != 0, KEY_BOARD_WORD::V);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::W] & 0x80) != 0, KEY_BOARD_WORD::W);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::X] & 0x80) != 0, KEY_BOARD_WORD::X);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::Y] & 0x80) != 0, KEY_BOARD_WORD::Y);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::Z] & 0x80) != 0, KEY_BOARD_WORD::Z);

			// 記号文字
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::COLON] & 0x80) != 0, KEY_BOARD_WORD::COLON);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::SENI_COLON] & 0x80) != 0, KEY_BOARD_WORD::SENI_COLON);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::COMMA] & 0x80) != 0, KEY_BOARD_WORD::COMMA);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::HYPHEN] & 0x80) != 0, KEY_BOARD_WORD::HYPHEN);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::PERIOD] & 0x80) != 0, KEY_BOARD_WORD::PERIOD);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::SLASH] & 0x80) != 0, KEY_BOARD_WORD::SLASH);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::AT_SIGN] & 0x80) != 0, KEY_BOARD_WORD::AT_SIGN);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::SQUARE_BRACKET] & 0x80) != 0, KEY_BOARD_WORD::SQUARE_BRACKET);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::YEN_SIGN] & 0x80) != 0, KEY_BOARD_WORD::YEN_SIGN);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::SQUARE_BRACKET_CLOSE] & 0x80) != 0, KEY_BOARD_WORD::SQUARE_BRACKET_CLOSE);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::CARET] & 0x80) != 0, KEY_BOARD_WORD::CARET);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::BACK_SLASH] & 0x80) != 0, KEY_BOARD_WORD::BACK_SLASH);

			// 移動キー
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::PG_UP] & 0x80) != 0, KEY_BOARD_WORD::PG_UP);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::PG_DN] & 0x80) != 0, KEY_BOARD_WORD::PG_DN);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::END] & 0x80) != 0, KEY_BOARD_WORD::END);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::HOME] & 0x80) != 0, KEY_BOARD_WORD::HOME);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::ARROW_LEFT] & 0x80) != 0, KEY_BOARD_WORD::ARROW_LEFT);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::ARROW_UP] & 0x80) != 0, KEY_BOARD_WORD::ARROW_UP);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::ARROW_RIGHT] & 0x80) != 0, KEY_BOARD_WORD::ARROW_RIGHT);
			SetNowWordKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::ARROW_DOWN] & 0x80) != 0, KEY_BOARD_WORD::ARROW_DOWN);
		}

		// 特殊
		{
			// 特殊キー
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::BACK_SPACE] & 0x80) != 0, KEY_BOARD_SPECIAL::BACK_SPACE);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::TAB] & 0x80) != 0, KEY_BOARD_SPECIAL::TAB);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::ENTER] & 0x80) != 0, KEY_BOARD_SPECIAL::ENTER);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::SPACE] & 0x80) != 0, KEY_BOARD_SPECIAL::SPACE);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::INSERT] & 0x80) != 0, KEY_BOARD_SPECIAL::INSERT);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::_DELETE] & 0x80) != 0, KEY_BOARD_SPECIAL::_DELETE);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::APPLICATION_KEY] & 0x80) != 0, KEY_BOARD_SPECIAL::APPLICATION_KEY);

			// 修飾キー
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::SHIFT_LEFT_AND_RIGHT] & 0x80) != 0, KEY_BOARD_SPECIAL::SHIFT_LEFT_AND_RIGHT);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::SIFT_LEFT] & 0x80) != 0, KEY_BOARD_SPECIAL::SIFT_LEFT);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::SIFT_RIGHT] & 0x80) != 0, KEY_BOARD_SPECIAL::SIFT_RIGHT);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::CTRL_LEFT_AND_RIGHT] & 0x80) != 0, KEY_BOARD_SPECIAL::CTRL_LEFT_AND_RIGHT);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::CTRL_LEFT] & 0x80) != 0, KEY_BOARD_SPECIAL::CTRL_LEFT);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::CTRL_RIGHT] & 0x80) != 0, KEY_BOARD_SPECIAL::CTRL_RIGHT);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::ALT_LEFT_AND_RIGHT] & 0x80) != 0, KEY_BOARD_SPECIAL::ALT_LEFT_AND_RIGHT);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::ALT_LEFT] & 0x80) != 0, KEY_BOARD_SPECIAL::ALT_LEFT);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::ALT_RIGHT] & 0x80) != 0, KEY_BOARD_SPECIAL::ALT_RIGHT);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::ESC] & 0x80) != 0, KEY_BOARD_SPECIAL::ESC);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::WINDOWS] & 0x80) != 0, KEY_BOARD_SPECIAL::WINDOWS);

			// 半角・全角
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::HALF_WIDTH] & 0x80) != 0, KEY_BOARD_SPECIAL::HALF_WIDTH);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::FULL_WIDTH] & 0x80) != 0, KEY_BOARD_SPECIAL::FULL_WIDTH);


			// F数字
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::F1] & 0x80) != 0, KEY_BOARD_SPECIAL::F1);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::F2] & 0x80) != 0, KEY_BOARD_SPECIAL::F2);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::F3] & 0x80) != 0, KEY_BOARD_SPECIAL::F3);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::F4] & 0x80) != 0, KEY_BOARD_SPECIAL::F4);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::F5] & 0x80) != 0, KEY_BOARD_SPECIAL::F5);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::F6] & 0x80) != 0, KEY_BOARD_SPECIAL::F6);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::F7] & 0x80) != 0, KEY_BOARD_SPECIAL::F7);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::F8] & 0x80) != 0, KEY_BOARD_SPECIAL::F8);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::F9] & 0x80) != 0, KEY_BOARD_SPECIAL::F9);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::F10] & 0x80) != 0, KEY_BOARD_SPECIAL::F10);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::F11] & 0x80) != 0, KEY_BOARD_SPECIAL::F11);
			SetNowSpecialKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::F12] & 0x80) != 0, KEY_BOARD_SPECIAL::F12);
		}

		// テンキー
		{
			// NumLockトグルキー
			SetNowNumpadKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::NUM_LOCK] & 0x80) != 0, KEY_BOARD_NUM_PAD::NUM_LOCK);

			// NumPad
			SetNowNumpadKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::NUMPAD_0_NUM_ON] & 0x80) != 0, KEY_BOARD_NUM_PAD::NUMPAD_0_NUM_ON);
			SetNowNumpadKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::NUMPAD_1_NUM_ON] & 0x80) != 0, KEY_BOARD_NUM_PAD::NUMPAD_1_NUM_ON);
			SetNowNumpadKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::NUMPAD_2_NUM_ON] & 0x80) != 0, KEY_BOARD_NUM_PAD::NUMPAD_2_NUM_ON);
			SetNowNumpadKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::NUMPAD_3_NUM_ON] & 0x80) != 0, KEY_BOARD_NUM_PAD::NUMPAD_3_NUM_ON);
			SetNowNumpadKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::NUMPAD_4_NUM_ON] & 0x80) != 0, KEY_BOARD_NUM_PAD::NUMPAD_4_NUM_ON);
			SetNowNumpadKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::NUMPAD_5_NUM_ON] & 0x80) != 0, KEY_BOARD_NUM_PAD::NUMPAD_5_NUM_ON);
			SetNowNumpadKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::NUMPAD_6_NUM_ON] & 0x80) != 0, KEY_BOARD_NUM_PAD::NUMPAD_6_NUM_ON);
			SetNowNumpadKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::NUMPAD_7_NUM_ON] & 0x80) != 0, KEY_BOARD_NUM_PAD::NUMPAD_7_NUM_ON);
			SetNowNumpadKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::NUMPAD_8_NUM_ON] & 0x80) != 0, KEY_BOARD_NUM_PAD::NUMPAD_8_NUM_ON);
			SetNowNumpadKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::NUMPAD_9_NUM_ON] & 0x80) != 0, KEY_BOARD_NUM_PAD::NUMPAD_9_NUM_ON);
			SetNowNumpadKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::NUMPAD_ASTERISK] & 0x80) != 0, KEY_BOARD_NUM_PAD::NUMPAD_ASTERISK);
			SetNowNumpadKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::NUMPAD_PLUS] & 0x80) != 0, KEY_BOARD_NUM_PAD::NUMPAD_PLUS);
			SetNowNumpadKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::NUMPAD_MINUS] & 0x80) != 0, KEY_BOARD_NUM_PAD::NUMPAD_MINUS);
			SetNowNumpadKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::NUMPAD_SLASH] & 0x80) != 0, KEY_BOARD_NUM_PAD::NUMPAD_SLASH);
			SetNowNumpadKey_Board((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::NUMPAD_5_NUM_OFF] & 0x80) != 0, KEY_BOARD_NUM_PAD::NUMPAD_5_NUM_OFF);
		}

		// トグルキー状態
		{
			mucToggleFlag.SetFlag((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::NUM_LOCK] & 0x1), KEY_BOARD_TOGGLE::NUM_LOCK);
			mucToggleFlag.SetFlag((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::CAPS_LOCK] & 0x1), KEY_BOARD_TOGGLE::CAPS_LOCK);
			mucToggleFlag.SetFlag((key[(int)KEY_BOARD_SET_NUMBER::BOARD_KEY_TYPE::SCROLL_LOCK] & 0x1), KEY_BOARD_TOGGLE::SCROLL_LOCK);
		}
	}

	// downとupを設定
	{
		mullWordFlags[(int)FLAG_TYPE::DOWN] = ((~prevWordFlags) & mullWordFlags[(int)FLAG_TYPE::NOW]);
		mullWordFlags[(int)FLAG_TYPE::UP] = (prevWordFlags & (~mullWordFlags[(int)FLAG_TYPE::NOW]));
		if (mullWordFlags[0].Bool() || mulSpecialFlags[0].Bool() || mushNumpadFlags->Bool())
		{
			int test = 1;
		}


		mulSpecialFlags[(int)FLAG_TYPE::DOWN] = ((~prevSpecialFlags) & mulSpecialFlags[(int)FLAG_TYPE::NOW]);
		mulSpecialFlags[(int)FLAG_TYPE::UP] = (prevSpecialFlags & (~mulSpecialFlags[(int)FLAG_TYPE::NOW]));

		mushNumpadFlags[(int)FLAG_TYPE::DOWN] = ((~prevNumpadFlags) & mushNumpadFlags[(int)FLAG_TYPE::NOW]);
		mushNumpadFlags[(int)FLAG_TYPE::UP] = (prevNumpadFlags & (~mushNumpadFlags[(int)FLAG_TYPE::NOW]));
	}
}