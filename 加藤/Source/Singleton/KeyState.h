#pragma once
#include "KeyStateEnum.h"
#include "BitFlag.h"
#include "Vector2.h"

/*------------------------------------------*/
/*          【キーステートクラス】          */
/*------------------------------------------*/
class KeyState
{
	/*--------*/
	/*【共通】*/
	/*--------*/

public:

	/*コンストラクタ*/
	KeyState();
	/*デストラクタ*/
	~KeyState();

	/*更新*/
	void Update();

	/// <summary>WndProcでやるKeyStateの処理</summary>
	void KeyStateWndProcProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	/*キーフラグを設定(コントローラーは現在のキーのみ設定)*/
	void SetKey();
	
	
	/*----------------------------------------------------------------------------------------------------
	* 【シャドウゲーム用】
	*/
private:
	/*
	* 上下左右
	*/
	BIT_FLAG<unsigned long> mulShadowGameFlags[static_cast<int>(FLAG_TYPE::MAX)];

	/// <summary>
	/// シャドウゲームで使用するキーを設定
	/// </summary>
	void SetKey_ShadowGame();

	/// <summary>
	/// シャドウゲームで使用するキーを一つ設定
	/// </summary>
	/// <param name="keyNumber">設定するキー</param>
	/// <param name="flags">フラグたち</param>
	void SetOneFlag_Shadow(KEY_SHADOW_GAME_TYPE keyNumber, std::initializer_list<bool> flags);

public:
	/// <summary>
	/// 押し始めたシャドウゲーム関連キーフラグを全取得(キーボード)
	/// </summary>
	/// <returns>ビットフラグ</returns>
	inline BIT_FLAG<unsigned long> GetShadowGameAllDownKeyFlags() const { return mulShadowGameFlags[static_cast<int>(FLAG_TYPE::DOWN)]; }

	/// <summary>
	/// 押しているシャドウゲーム関連キーフラグを全取得(キーボード)
	/// </summary>
	/// <returns>ビットフラグ</returns>
	inline BIT_FLAG<unsigned long> GetShadowGameAllNowKeyFlags() const { return mulShadowGameFlags[static_cast<int>(FLAG_TYPE::NOW)]; }

	/// <summary>
	/// 離したシャドウゲーム関連キーフラグを全取得(キーボード)
	/// </summary>
	/// <returns>ビットフラグ</returns>
	inline BIT_FLAG<unsigned long> GetShadowGameAllUpKeyFlags() const { return mulShadowGameFlags[static_cast<int>(FLAG_TYPE::UP)]; }

	/*-----【指定のシャドウゲーム関連キーが押しているなら「true」を返す(キーボード)】-----*/
	inline bool GetShadowGameKey(int keyNumber) const { return mulShadowGameFlags[static_cast<int>(FLAG_TYPE::NOW)].GetFlag_BitShift(keyNumber); }
	/*【指定のシャドウゲーム関連キーが押しているなら「true」を返す(キーボード)】*/
	inline bool GetShadowGameKey(KEY_SHADOW_GAME_TYPE keyNumber) const { return mulShadowGameFlags[static_cast<int>(FLAG_TYPE::NOW)].GetFlag_BitShift(keyNumber); }
	/*----------------------------------------------------------------------------*/
	/*-----【指定のシャドウゲーム関連キーが押し始めた瞬間なら「true」を返す(キーボード)】-----*/
	inline bool GetShadowGameKeyDown(int keyNumber) const { return mulShadowGameFlags[static_cast<int>(FLAG_TYPE::DOWN)].GetFlag_BitShift(keyNumber); }
	/*【指定のシャドウゲーム関連キーが押し始めた瞬間なら「true」を返す(キーボード)】*/
	inline bool GetShadowGameKeyDown(KEY_SHADOW_GAME_TYPE keyNumber) const { return mulShadowGameFlags[static_cast<int>(FLAG_TYPE::DOWN)].GetFlag_BitShift(keyNumber); }
	/*--------------------------------------------------------------------------------*/
	/*-----【指定のシャドウゲーム関連キーが離した瞬間なら「true」を返す(キーボード)】-----*/
	inline bool GetShadowGameKeyUp(int keyNumber) const { return mulShadowGameFlags[static_cast<int>(FLAG_TYPE::UP)].GetFlag_BitShift(keyNumber); }
	/*【指定のシャドウゲーム関連キーが離した瞬間なら「true」を返す(キーボード)】*/
	inline bool GetShadowGameKeyUp(KEY_SHADOW_GAME_TYPE keyNumber) const { return mulShadowGameFlags[static_cast<int>(FLAG_TYPE::UP)].GetFlag_BitShift(keyNumber); }
	/*----------------------------------------------------------------------------*/

	/*------------------*/
	/*【コントローラー】*/
	/*------------------*/

private:
	/*コントローラーフラグズ*/
	BIT_FLAG<unsigned int> munControllerKeyFlags[(int)FLAG_TYPE::MAX][(int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER];

	/*右スティック横*/
	short mshNowRightStickX[(int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER];
	/*右スティック縦*/
	short mshNowRightStickY[(int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER];

	/*左スティック横*/
	short mshNowLeftStickX[(int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER];
	/*左スティック縦*/
	short mshNowLeftStickY[(int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER];

	/*左奥トリガー押し込み度合*/
	short mshNowLeftTrigger[(int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER];
	/*右奥トリガー押し込み度合*/
	short mshNowRightTrigger[(int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER];

	/*コントローラー最大数*/
	static constexpr int CONTROLLER_MAX = (const int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER;

public:

	/*-----【指定のコントローラーの押し始めたキーフラグを全取得(コントローラー)】-----*/
	inline BIT_FLAG<unsigned int> GetDownKeyFlags_Controller(int controllerNumber) const { return munControllerKeyFlags[(int)FLAG_TYPE::DOWN][controllerNumber]; }
	/*【指定のコントローラーの押し始めたキーフラグを全取得(コントローラー)】*/
	inline BIT_FLAG<unsigned int> GetDownKeyFlags_Controller(CONTROLLER_KEY_NUMBER controllerNumber) const { return GetDownKeyFlags_Controller((int)controllerNumber); }
	/*--------------------------------------------------------------------------------*/
	/// <summary>
	/// 全コントローラー分の押し始めたキーフラグを全取得(コントローラー)
	/// </summary>
	/// <param name="keyBoardFlag">キーボード分も取得するなら「true」にする</param>
	/// <returns>全コントローラーの押し始めたすべてのキーフラグ</returns>
	BIT_FLAG<unsigned int> GetAllDownKeyFlags_Controller(bool keyBoardFlag = true)  const;


	/*-----【指定のコントローラーの押しているキーフラグを全取得(コントローラー)】-----*/
	inline BIT_FLAG<unsigned int> GetNowKeyFlags_Controller(int controllerNumber) const { return munControllerKeyFlags[(int)FLAG_TYPE::NOW][controllerNumber]; }
	/*【指定のコントローラーの押しているキーフラグを全取得(コントローラー)】*/
	inline BIT_FLAG<unsigned int> GetNowKeyFlags_Controller(CONTROLLER_KEY_NUMBER controllerNumber) const { return GetNowKeyFlags_Controller((int)controllerNumber); }
	/*--------------------------------------------------------------------------------*/
	/// <summary>
	/// 全コントローラー分の押しているキーフラグを全取得(コントローラー)
	/// </summary>
	/// <param name="keyBoardFlag">キーボード分も取得するなら「true」にする</param>
	/// <returns>全コントローラーの押しているすべてのキーフラグ</returns>
	BIT_FLAG<unsigned int> GetAllNowKeyFlags_Controller(bool keyBoardFlag = true)  const;


	/*-----【指定のコントローラーの離したキーフラグを全取得(コントローラー)】-----*/
	inline BIT_FLAG<unsigned int> GetUpKeyFlags_Controller(int controllerNumber)  const { return munControllerKeyFlags[(int)FLAG_TYPE::UP][controllerNumber]; }
	/*【指定のコントローラーの離したキーフラグを全取得(コントローラー)】*/
	inline BIT_FLAG<unsigned int> GetUpKeyFlags_Controller(CONTROLLER_KEY_NUMBER controllerNumber)  const { return GetUpKeyFlags_Controller((int)controllerNumber); }
	/*----------------------------------------------------------------------------*/
	/// <summary>
	/// 全コントローラー分の離したキーフラグを全取得(コントローラー)
	/// </summary>
	/// <param name="keyBoardFlag">キーボード分も取得するなら「true」にする</param>
	/// <returns>全コントローラーの離したすべてのキーフラグ</returns>
	BIT_FLAG<unsigned int> GetAllUpKeyFlags_Controller(bool keyBoardFlag = true)  const;


	/*-----【キーナンバーに応じたキーの押しているなら「true」を返す(コントローラー)】-----*/
	inline bool GetKey_Controller(int keyNumber, int controllerNumber)  const { return munControllerKeyFlags[(int)FLAG_TYPE::NOW][controllerNumber].GetFlag_BitShift(keyNumber); }
	/*【キーナンバーに応じたキーの押しているなら「true」を返す(コントローラー)】*/
	inline bool GetKey_Controller(CONTROLLER_KEY_TYPE keyNumber, int controllerNumber)  const { return GetKey_Controller((int)keyNumber, controllerNumber); }
	/*【キーナンバーに応じたキーの押しているなら「true」を返す(コントローラー)】*/
	inline bool GetKey_Controller(CONTROLLER_KEY_TYPE keyNumber, CONTROLLER_KEY_NUMBER controllerNumber) const { return GetKey_Controller((int)keyNumber, (int)controllerNumber); }
	/*------------------------------------------------------------------------------------*/
	/*-----【全コントローラーのキーナンバーに応じたキーを誰かが押しているなら「true」を返す(コントローラー)】-----*/
	inline bool GetKeyAllController(int keyNumber, bool keyBoard = true) const { for (int i = (keyBoard ? 0 : 1); i < (int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER; i++) { if (GetKey_Controller(keyNumber, i)) { return true; } } return false; }
	/*【全コントローラーのキーナンバーに応じたキーを誰かが押しているなら「true」を返す(コントローラー)】*/
	inline bool GetKeyAllController(CONTROLLER_KEY_TYPE keyNumber, bool keyBoard = true) const { return GetKeyAllController((int)keyNumber, keyBoard); }
	/*------------------------------------------------------------------------------------------------------------*/

	/*-----【キーナンバーに応じたキーが押された瞬間なら「true」を返す(コントローラー)】-----*/
	inline bool GetKeyDown_Controller(int keyNumber, int controllerNumber) const { return munControllerKeyFlags[(int)FLAG_TYPE::DOWN][controllerNumber].GetFlag_BitShift(keyNumber); }
	/*【キーナンバーに応じたキーが押された瞬間なら「true」を返す(コントローラー)】*/
	inline bool GetKeyDown_Controller(CONTROLLER_KEY_TYPE keyNumber, int controllerNumber) const { return GetKeyDown_Controller((int)keyNumber, controllerNumber); }
	/*【キーナンバーに応じたキーが押された瞬間なら「true」を返す(コントローラー)】*/
	inline bool GetKeyDown_Controller(CONTROLLER_KEY_TYPE keyNumber, CONTROLLER_KEY_NUMBER controllerNumber) const { return GetKeyDown_Controller((int)keyNumber, (int)controllerNumber); }
	/*--------------------------------------------------------------------------------------*/
	/*-----【全コントローラーのキーナンバーに応じたキーを誰かが押した瞬間なら「true」を返す(コントローラー)】-----*/
	inline bool GetKeyDownAllController(int keyNumber, bool keyBoard = true) const { for (int i = (keyBoard ? 0 : 1); i < (int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER; i++) { if (GetKeyDown_Controller(keyNumber, i)) { return true; } } return false; }
	/*【全コントローラーのキーナンバーに応じたキーを誰かが押した瞬間なら「true」を返す(コントローラー)】*/
	inline bool GetKeyDownAllController(CONTROLLER_KEY_TYPE keyNumber, bool keyBoard = true) const { return GetKeyDownAllController((int)keyNumber, keyBoard); }
	/*------------------------------------------------------------------------------------------------------------*/

	/*-----【キーナンバーに応じたキーが押されなくなった瞬間なら「true」を返す(コントローラー)】-----*/
	inline bool GetKeyUp_Controller(int keyNumber, int controllerNumber) const { return munControllerKeyFlags[(int)FLAG_TYPE::UP][controllerNumber].GetFlag_BitShift(keyNumber); }
	/*【キーナンバーに応じたキーが押されなくなった瞬間なら「true」を返す(コントローラー)】*/
	inline bool GetKeyUp_Controller(CONTROLLER_KEY_TYPE keyNumber, int controllerNumber) const { return GetKeyUp_Controller((int)keyNumber, controllerNumber); }
	/*【キーナンバーに応じたキーが押されなくなった瞬間なら「true」を返す(コントローラー)】*/
	inline bool GetKeyUp_Controller(CONTROLLER_KEY_TYPE keyNumber, CONTROLLER_KEY_NUMBER controllerNumber) const { return GetKeyUp_Controller((int)keyNumber, (int)controllerNumber); }
	/*----------------------------------------------------------------------------------------------*/
	/*-----【全コントローラーのキーナンバーに応じたキーを誰かが離した瞬間なら「true」を返す(コントローラー)】-----*/
	inline bool GetKeyUpAllController(int keyNumber, bool keyBoard = true) const { for (int i = (keyBoard ? 0 : 1); i < (int)CONTROLLER_KEY_NUMBER::MAX_CONTROLLER; i++) { if (GetKeyUp_Controller(keyNumber, i)) { return true; } } return false; }
	/*【全コントローラーのキーナンバーに応じたキーを誰かが押した瞬間なら「true」を返す(コントローラー)】*/
	inline bool GetKeyUpAllController(CONTROLLER_KEY_TYPE keyNumber, bool keyBoard = true) const { return GetKeyUpAllController((int)keyNumber, keyBoard); }
	/*------------------------------------------------------------------------------------------------------------*/


	/*-----【キーの変動値取得(コントローラー)】-----*/
	short GetKeyVariable_Controller(CONTROLLER_KEY_VARIABLE_NUMBER getNumber, int controllerNumber) const;
	/*【キーの変動値取得(コントローラー)】*/
	inline short GetKeyVariable_Controller(CONTROLLER_KEY_VARIABLE_NUMBER getNumber, CONTROLLER_KEY_NUMBER controllerNumber = CONTROLLER_KEY_NUMBER::MAX_CONTROLLER) const { return GetKeyVariable_Controller(getNumber, (int)controllerNumber); }
	/*----------------------------------------------*/

private:

	/*フラグを現在のキーに反映させる(コントローラー)*/
	void SetNowKey_Controller(bool flag, CONTROLLER_KEY_TYPE number, int controllerNumber = 0) { munControllerKeyFlags[(int)FLAG_TYPE::NOW][controllerNumber].SetFlag(flag, number); }

	/*--------------*/
	/*【キーボード】*/
	/*--------------*/

private:
	// 文字操作系キーフラグズ
	BIT_FLAG<unsigned long long> mullWordFlags[(int)FLAG_TYPE::MAX];

	// 特殊キーフラグズ
	BIT_FLAG<unsigned long> mulSpecialFlags[(int)FLAG_TYPE::MAX];

	// テンキーフラグズ
	BIT_FLAG<unsigned short> mushNumpadFlags[(int)FLAG_TYPE::MAX];

	// トグル状態
	BIT_FLAG<unsigned char> mucToggleFlag;

public:


	/// <summary>
	/// 押し始めた文字操作系キーフラグを全取得(キーボード)
	/// </summary>
	inline BIT_FLAG<unsigned long long> GetDownWordKeyFlags_Board() const { return mullWordFlags[(int)FLAG_TYPE::DOWN]; }

	/// <summary>
	/// 押している文字操作系キーフラグを全取得(キーボード)
	/// </summary>
	inline BIT_FLAG<unsigned long long> GetNowWordKeyFlags_Board() const { return mullWordFlags[(int)FLAG_TYPE::NOW]; }

	/// <summary>
	/// 離した文字操作系キーフラグを全取得(キーボード)
	/// </summary>
	inline BIT_FLAG<unsigned long long> GetUpWordKeyFlags_Board() const { return mullWordFlags[(int)FLAG_TYPE::UP]; }
	/*-----【指定の文字操作系キーが押しているなら「true」を返す(キーボード)】-----*/
	inline bool GetWordKey_Board(int keyNumber) const { return mullWordFlags[(int)FLAG_TYPE::NOW].GetFlag_BitShift(keyNumber); }
	/*【指定の文字操作系キーが押しているなら「true」を返す(キーボード)】*/
	inline bool GetWordKey_Board(KEY_BOARD_WORD keyNumber) const { return GetWordKey_Board((int)keyNumber); }
	/*----------------------------------------------------------------------------*/
	/*-----【指定の文字操作系キーが押し始めた瞬間なら「true」を返す(キーボード)】-----*/
	inline bool GetWordKeyDown_Board(int keyNumber) const { return mullWordFlags[(int)FLAG_TYPE::DOWN].GetFlag_BitShift(keyNumber); }
	/*【指定の文字操作系キーが押し始めた瞬間なら「true」を返す(キーボード)】*/
	inline bool GetWordKeyDown_Board(KEY_BOARD_WORD keyNumber) const { return GetWordKeyDown_Board((int)keyNumber); }
	/*--------------------------------------------------------------------------------*/
	/*-----【指定の文字操作系キーが離した瞬間なら「true」を返す(キーボード)】-----*/
	inline bool GetWordKeyUp_Board(int keyNumber) const { return mullWordFlags[(int)FLAG_TYPE::UP].GetFlag_BitShift(keyNumber); }
	/*【指定の文字操作系キーが離した瞬間なら「true」を返す(キーボード)】*/
	inline bool GetWordKeyUp_Board(KEY_BOARD_WORD keyNumber) const { return GetWordKeyUp_Board((int)keyNumber); }
	/*----------------------------------------------------------------------------*/


	/// <summary>
	/// 押し始めた特殊キーフラグを全取得(キーボード)
	/// </summary>
	inline BIT_FLAG<unsigned long> GetDownSpecialKeyFlags_Board() const { return mulSpecialFlags[(int)FLAG_TYPE::DOWN]; }

	/// <summary>
	/// 押している特殊キーフラグを全取得(キーボード)
	/// </summary>
	inline BIT_FLAG<unsigned long> GetNowSpecialKeyFlags_Board() const { return mulSpecialFlags[(int)FLAG_TYPE::NOW]; }

	/// <summary>
	/// 離した特殊キーフラグを全取得(キーボード)
	/// </summary>
	inline BIT_FLAG<unsigned long> GetUpSpecialKeyFlags_Board() const { return mulSpecialFlags[(int)FLAG_TYPE::UP]; }
	/*-----【指定の特殊キーが押しているなら「true」を返す(キーボード)】-----*/
	inline bool GetSpecialKey_Board(int keyNumber) const { return mulSpecialFlags[(int)FLAG_TYPE::NOW].GetFlag_BitShift(keyNumber); }
	/*【指定の特殊キーが押しているなら「true」を返す(キーボード)】*/
	inline bool GetSpecialKey_Board(KEY_BOARD_SPECIAL keyNumber) const { return GetSpecialKey_Board((int)keyNumber); }
	/*----------------------------------------------------------------------*/
	/*-----【指定の特殊キーが押し始めた瞬間なら「true」を返す(キーボード)】-----*/
	inline bool GetSpecialKeyDown_Board(int keyNumber) const { return mulSpecialFlags[(int)FLAG_TYPE::DOWN].GetFlag_BitShift(keyNumber); }
	/*【指定の特殊キーが押し始めた瞬間なら「true」を返す(キーボード)】*/
	inline bool GetSpecialKeyDown_Board(KEY_BOARD_SPECIAL keyNumber) const { return GetSpecialKeyDown_Board((int)keyNumber); }
	/*--------------------------------------------------------------------------*/
	/*-----【指定の特殊キーが離した瞬間なら「true」を返す(キーボード)】-----*/
	inline bool GetSpecialKeyUp_Board(int keyNumber) const { return mulSpecialFlags[(int)FLAG_TYPE::UP].GetFlag_BitShift(keyNumber);}
	/*【指定の特殊キーが離した瞬間なら「true」を返す(キーボード)】*/
	inline bool GetSpecialKeyUp_Board(KEY_BOARD_SPECIAL keyNumber) const { return GetSpecialKeyUp_Board((int)keyNumber); }
	/*--------------------------------------------------------------------------*/


	/// <summary>
	/// 押し始めたテンキーキーフラグを全取得(キーボード)
	/// </summary>
	inline BIT_FLAG<unsigned short> GetDownNumpadKeyFlags_Board() const { return mushNumpadFlags[(int)FLAG_TYPE::DOWN]; }

	/// <summary>
	/// 押しているテンキーキーフラグを全取得(キーボード)
	/// </summary>
	inline BIT_FLAG<unsigned short> GetNowNumpadKeyFlags_Board() const { return mushNumpadFlags[(int)FLAG_TYPE::NOW]; }

	/// <summary>
	/// 離したテンキーキーフラグを全取得(キーボード)
	/// </summary>
	inline BIT_FLAG<unsigned short> GetUpNumpadKeyFlags_Board() const { return mushNumpadFlags[(int)FLAG_TYPE::UP]; }
	/*-----【指定のテンキーキーが押しているなら「true」を返す(キーボード)】-----*/
	inline bool GetNumpadKey_Board(int keyNumber) const { return mushNumpadFlags[(int)FLAG_TYPE::NOW].GetFlag_BitShift(keyNumber); }
	/*【指定のテンキーキーが押しているなら「true」を返す(キーボード)】*/
	inline bool GetNumpadKey_Board(KEY_BOARD_NUM_PAD keyNumber) const { return GetNumpadKey_Board((int)keyNumber); }
	/*--------------------------------------------------------------------------*/
	/*-----【指定のテンキーキーが押し始めた瞬間なら「true」を返す(キーボード)】-----*/
	inline bool GetNumpadKeyDown_Board(int keyNumber) const { return mushNumpadFlags[(int)FLAG_TYPE::DOWN].GetFlag_BitShift(keyNumber); }
	/*【指定のテンキーキーが押し始めた瞬間なら「true」を返す(キーボード)】*/
	inline bool GetNumpadKeyDown_Board(KEY_BOARD_NUM_PAD keyNumber) const { return GetNumpadKeyDown_Board((int)keyNumber); }
	/*------------------------------------------------------------------------------*/
	/*-----【指定のテンキーキーが離した瞬間なら「true」を返す(キーボード)】-----*/
	inline bool GetNumpadKeyUp_Board(int keyNumber) const { return mushNumpadFlags[(int)FLAG_TYPE::UP].GetFlag_BitShift(keyNumber); }
	/*【指定のテンキーキーが離した瞬間なら「true」を返す(キーボード)】*/
	inline bool GetNumpadKeyUp_Board(KEY_BOARD_NUM_PAD keyNumber) const { return GetNumpadKeyUp_Board((int)keyNumber); }
	/*--------------------------------------------------------------------------*/

	/// <summary>
	/// トグル状態全取得(キーボード)
	/// </summary>
	inline BIT_FLAG<unsigned char> GetAllToggleState() const { return mucToggleFlag; }
	/*-----【指定のトグル状態が有効なら「true」を返す(キーボード)】-----*/
	inline bool GetToggleState(int keyNumber) const { return mucToggleFlag.GetFlag_BitShift(keyNumber); }
	/*【指定のトグル状態が有効なら「true」を返す(キーボード)】*/
	inline bool GetToggleState(KEY_BOARD_TOGGLE keyNumber) const { return GetToggleState((int)keyNumber); }
	/*------------------------------------------------------------------*/

private:
	/*キーフラグを設定する(キーボード)*/
	void SetKey_Board(unsigned char* key);

	/*フラグを現在のキーに反映させる(文字操作系キーボードキー)*/
	inline void SetNowWordKey_Board(bool flag, KEY_BOARD_WORD keyNumber) { mullWordFlags[(int)FLAG_TYPE::NOW].SetFlag(flag, keyNumber); }
	/*フラグを現在のキーに反映させる(特殊キーボードキー)*/
	inline void SetNowSpecialKey_Board(bool flag, KEY_BOARD_SPECIAL keyNumber) { mulSpecialFlags[(int)FLAG_TYPE::NOW].SetFlag(flag, keyNumber); }
	/*フラグを現在のキーに反映させる(テンキーキー　キーボードキー)*/
	inline void SetNowNumpadKey_Board(bool flag, KEY_BOARD_NUM_PAD keyNumber) { mushNumpadFlags[(int)FLAG_TYPE::NOW].SetFlag(flag, keyNumber); }


	/*----------*/
	/*【マウス】*/
	/*----------*/

private:
	// マウスフラグズ(現在の反応している/反応し始めた/反応していた)
	BIT_FLAG<unsigned int> munMouseFlags;

	// マウスのポジション
	Vector2 mstCursorPos[(int)CURSOR_POSITION_TYPE::MAX];

	// マウスホイールの回転量
	short mshWheelVolume;

	// マウスホイール取得用メッセージ
	MSG mdxsMsg;

	// メッセージ設定フラグ
	bool mbMsgSetFlag;
public:
	/*メッセージ設定*/
	void SetMsg(MSG msg) { mdxsMsg = msg; mbMsgSetFlag = true; }

	/*マウスフラグを全取得*/
	inline BIT_FLAG<unsigned int> GetMouseFlags() const { return munMouseFlags; }

	/*-----【指定のフラグが反応しているなら「true」を返す】-----*/
	inline bool GetMouse(int number) const { return munMouseFlags.GetFlag_BitShift(number); }
	/*【指定のフラグが反応しているなら「true」を返す】*/
	inline bool GetMouse(MOUSE_TYPE number) const { return GetMouse((int)number); }
	/*----------------------------------------------------------*/

	/*-----【指定のフラグが反応し始めたなら「true」を返す】-----*/
	inline bool GetDownMouse(int number) const { return munMouseFlags.GetFlag_BitShift(number + (int)MOUSE_TYPE::MAX); }
	/*【指定のフラグが反応し始めたなら「true」を返す】*/
	inline bool GetDownMouse(MOUSE_TYPE number) const { return GetDownMouse((int)number); }
	/*----------------------------------------------------------*/

	/*-----【指定のフラグが反応していたなら「true」を返す】-----*/
	inline bool GetUpMouse(int number) const { return munMouseFlags.GetFlag_BitShift(number + (int)MOUSE_TYPE::MAX + (int)MOUSE_TYPE::MAX); }
	/*【指定のフラグが反応していたなら「true」を返す】*/
	inline bool GetUpMouse(MOUSE_TYPE number) const { return GetUpMouse((int)number); }
	/*----------------------------------------------------------*/

	/*-----【指定タイミングのカーソルポジション取得】-----*/
	inline Vector2 GetCursorPosition(int number) const { return mstCursorPos[number]; }
	/*【指定タイミングのカーソルポジション取得】*/
	inline Vector2 GetCursorPosition(CURSOR_POSITION_TYPE number) const { return GetCursorPosition((int)number); }
	/*----------------------------------------------------*/

	/*マウスホイール回転取得(他の入力があるまで反応し続ける(キーボードも含めた))*/
	inline short GetWheelVolume() const { return mshWheelVolume; }

private:
	/*マウスフラグを設定*/
	inline void SetMouseFlag(bool flag, MOUSE_TYPE number) { munMouseFlags.SetFlag(flag, number); }
};