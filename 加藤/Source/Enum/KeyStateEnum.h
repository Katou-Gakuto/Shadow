#pragma once
/*----------------------------------------------*/
/*          【キーステートenum class】          */
/*----------------------------------------------*/

/*----------------------------*/
/*     【シャドウゲーム】     */
/*----------------------------*/
// シャドウゲーム　キー操作種類
enum class KEY_SHADOW_GAME_TYPE
{
	UP = 0,	// 上
	DOWN,	// 下
	LEFT,	// 左
	RIGHT,	// 右
	MOVE,	// 移動
};


/*--------------------*/
/*     【フラグ】     */
/*--------------------*/

// フラグの種類(このenumを使ってないフラグもある
enum class FLAG_TYPE
{
	/*現在の状態*/
	NOW,
	/*押した始めた*/
	DOWN,
	/*離した*/
	UP,

	/*最大*/
	MAX
};

/*----------------------------*/
/*     【コントローラー】     */
/*----------------------------*/

// コントローラーキーの種類
enum class CONTROLLER_KEY_TYPE
{
	/*有無*/
	EXISTENCE = 0,

	/*Aボタン*/
	A,
	/*Bボタン*/
	B,
	/*Xボタン*/
	X,
	/*Yボタン*/
	Y,

	/*Lボタン(左トリガー)*/
	L,
	/*LTボタン(左奥トリガー)*/
	LT,
	/*Rボタン(右トリガー)*/
	R,
	/*RTボタン(右奥トリガー)*/
	RT,

	/*左ボタン*/
	LEFT,
	/*右ボタン*/
	RIGHT,
	/*上ボタン*/
	UP,
	/*下ボタン*/
	DOWN,

	/*左(左スティック)*/
	LEFT_STICK_LEFT,
	/*右(左スティック)*/
	LEFT_STICK_RIGHT,
	/*上(左スティック)*/
	LEFT_STICK_UP,
	/*下(左スティック)*/
	LEFT_STICK_DOWN,
	/*押し込み(左スティック)*/
	LEFT_STICK_PUSH,

	/*左(右スティック)*/
	RIGHT_STICK_LEFT,
	/*右(右スティック)*/
	RIGHT_STICK_RIGHT,
	/*上(右スティック)*/
	RIGHT_STICK_UP,
	/*下(右スティック)*/
	RIGHT_STICK_DOWN,
	/*押し込み(右スティック)*/
	RIGHT_STICK_PUSH,

	/*スタート*/
	START,
	/*バック*/
	BACK,

	/*最大キー数*/
	KEY_MAX
};

// 変動するコントローラーキーナンバー
enum class CONTROLLER_KEY_VARIABLE_NUMBER
{
	/*右スティック(X軸)*/
	STICK_RIGHT_X = 0,
	/*右スティック(Y軸)*/
	STICK_RIGHT_Y,
	/*左スティック(X軸)*/
	STICK_LEFT_X,
	/*左スティック(Y軸)*/
	STICK_LEFT_Y,
	/*右奥トリガー*/
	RIGHT_TRIGGER,
	/*左奥トリガー*/
	LEFT_TRIGGER
};

// コントローラーナンバー
enum class CONTROLLER_KEY_NUMBER
{
	/*キーボード*/
	KEY_BOARD = 0,
	/*コントローラー(1)*/
	CONTROLLER_1,
	/*コントローラー(2)*/
	CONTROLLER_2,
	/*コントローラー(3)*/
	CONTROLLER_3,
	/*コントローラー(4)*/
	CONTROLLER_4,

	/*コントローラー最大数*/
	MAX_CONTROLLER
};

/*------------------------*/
/*     【キーボード】     */
/*------------------------*/

// キーボードフラグ設定用
namespace KEY_BOARD_SET_NUMBER
{
	// キーボードのキー種類
	enum class BOARD_KEY_TYPE
	{// 137個

		/*------------*/
		/*【数字キー】*/
		/*------------*/

		_0 = '0',
		_1,
		_2,
		_3,
		_4,
		_5,
		_6,
		_7,
		_8,
		_9,
		
		/*------------*/
		/*【文字キー】*/
		/*------------*/

		A = 'A',
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		/*------------*/
		/*【記号文字】*/
		/*------------*/

		COLON = 186,				// コロン
		SENI_COLON,					// セミコロン
		COMMA,						// カンマ
		HYPHEN,						// ハイフン
		PERIOD,						// ピリオド
		SLASH,						// スラッシュ
		AT_SIGN,					// アットマーク
		SQUARE_BRACKET = 219,		// 角括弧
		YEN_SIGN,					// 円マーク
		SQUARE_BRACKET_CLOSE,		// 角括弧
		CARET,						// キャレット(^)
		BACK_SLASH = 226,			// バックスラッシュ

		/*------------*/
		/*【移動キー】*/
		/*------------*/

		PG_UP = 33,					// ページアップ
		PG_DN,						// ページダウン
		END,						// 行末まで飛ぶ
		HOME,						// 行頭まで飛ぶ
		ARROW_LEFT,					// 左移動
		ARROW_UP,					// 上移動
		ARROW_RIGHT,				// 右移動
		ARROW_DOWN,					// 下移動

		/*------------*/
		/*【特殊キー】*/
		/*------------*/

		BACK_SPACE = 8,				// バックスペース
		TAB,						// タブ
		ENTER = 13,					// エンター
		SPACE = 32,					// スペース
		INSERT = 45,				// インサート
		_DELETE,					// デリート
		APPLICATION_KEY = 93,		// アプリケーションキー(ノートみたいな絵の奴)

		/*------------*/
		/*【修飾キー】*/
		/*------------*/

		SHIFT_LEFT_AND_RIGHT = 16,	// 両シフト
		SIFT_LEFT = 160,			// 左シフト
		SIFT_RIGHT,					// 右シフト
		CTRL_LEFT_AND_RIGHT = 17,	// 両コントロール
		CTRL_LEFT = 162,			// 左コントロール
		CTRL_RIGHT,					// 右コントロール
		ALT_LEFT_AND_RIGHT = 18,	// 両オルト
		ALT_LEFT = 164,				// 左オルト
		ALT_RIGHT,					// 右オルト
		ESC = 27,					// エスケープ
		WINDOWS = 91,				// ウィンドーズ

		/*--------------*/
		/*【半角・全角】*/
		/*--------------*/
		HALF_WIDTH = 240,			// 半角
		FULL_WIDTH = 242,			// 全角


		/*---------*/
		/*【F数字】*/
		/*---------*/

		F1 = 112,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,

		/*--------------*/
		/*【トグルキー】*/
		/*--------------*/

		NUM_LOCK = 144,				// num(ナム)ロック
		CAPS_LOCK,					// caps(キャプチャー)ロック
		SCROLL_LOCK = 20,			// Scroll(スクロール)ロック

		/*----------*/
		/*【NumPad】*/
		/*----------*/

		NUMPAD_0_NUM_ON = 96,		// 0
		NUMPAD_1_NUM_ON,			// 1
		NUMPAD_2_NUM_ON,			// 2
		NUMPAD_3_NUM_ON,			// 3
		NUMPAD_4_NUM_ON,			// 4
		NUMPAD_5_NUM_ON,			// 5
		NUMPAD_6_NUM_ON,			// 6
		NUMPAD_7_NUM_ON,			// 7
		NUMPAD_8_NUM_ON,			// 8
		NUMPAD_9_NUM_ON,			// 9
		NUMPAD_ASTERISK,			// アスタリスク(掛け算)
		NUMPAD_PLUS,				// プラス(足し算)
		NUMPAD_MINUS = 109,			// マイナス(引き算)
		NUMPAD_SLASH = 111,			// スラッシュ(割り算)
		NUMPAD_5_NUM_OFF = 12,		// 何も文字をかけない取得出来るだけ


		/*----------------------------------------*/
		/*【音量キー(使わない予定、取得もしない)】*/
		/*----------------------------------------*/

		VOLUME_DELETE = 173,		// 音量削除
		VOLUME_DOWN,				// 音量下げる
		VOLUME_UP,					// 音量上げる

		/*----------*/
		/*【マウス】*/
		/*----------*/

		MOUSE_LEFT = 1,				// 左マウス
		MOUSE_RIGHT,				// 右マウス
		MOUSE_WHEEL_BUTTON = 4,		// マウスホイール
	};
}

// キーボード 文字操作種類
enum class KEY_BOARD_WORD
{
	/*------------*/
	/*【数字キー】*/
	/*------------*/

	_0 = 0,
	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,

	/*------------*/
	/*【文字キー】*/
	/*------------*/

	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,

	/*------------*/
	/*【記号文字】*/
	/*------------*/

	COLON,					// コロン
	SENI_COLON,				// セミコロン
	COMMA,					// カンマ
	HYPHEN,					// ハイフン
	PERIOD,					// ピリオド
	SLASH,					// スラッシュ
	AT_SIGN,				// アットマーク
	SQUARE_BRACKET,			// 角括弧
	YEN_SIGN,				// 円マーク
	SQUARE_BRACKET_CLOSE,	// 角括弧
	CARET,					// キャレット(^)
	BACK_SLASH,				// バックスラッシュ

	/*------------*/
	/*【移動キー】*/
	/*------------*/

	PG_UP,					// ページアップ
	PG_DN,					// ページダウン
	END,					// 行末まで飛ぶ
	HOME,					// 行頭まで飛ぶ
	ARROW_LEFT,				// 左移動
	ARROW_UP,				// 上移動
	ARROW_RIGHT,			// 右移動
	ARROW_DOWN,				// 下移動
};

// キーボード 特殊
enum class KEY_BOARD_SPECIAL
{
	/*------------*/
	/*【特殊キー】*/
	/*------------*/

	BACK_SPACE = 0,			// バックスペース
	TAB,					// タブ
	ENTER,					// エンター
	SPACE,					// スペース
	INSERT,					// インサート
	_DELETE,				// デリート
	APPLICATION_KEY,		// アプリケーションキー(ノートみたいな絵の奴)

	/*------------*/
	/*【修飾キー】*/
	/*------------*/

	SHIFT_LEFT_AND_RIGHT,	// 両シフト
	SIFT_LEFT,				// 左シフト
	SIFT_RIGHT,				// 右シフト
	CTRL_LEFT_AND_RIGHT,	// 両コントロール
	CTRL_LEFT,				// 左コントロール
	CTRL_RIGHT,				// 右コントロール
	ALT_LEFT_AND_RIGHT,		// 両オルト
	ALT_LEFT,				// 左オルト
	ALT_RIGHT,				// 右オルト
	ESC,					// エスケープ
	WINDOWS,				// ウィンドーズ

	/*--------------*/
	/*【半角・全角】*/
	/*--------------*/
	HALF_WIDTH,				// 半角
	FULL_WIDTH,				// 全角

	/*---------*/
	/*【F数字】*/
	/*---------*/

	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
};

// キーボード テンキー操作
enum class KEY_BOARD_NUM_PAD
{
	/*--------------*/
	/*【トグルキー】*/
	/*--------------*/

	NUM_LOCK = 0,				// num(ナム)ロック

	/*-----------*/
	/*【FNumPad】*/
	/*-----------*/
	
	NUMPAD_0_NUM_ON,			// 0
	NUMPAD_1_NUM_ON,			// 1
	NUMPAD_2_NUM_ON,			// 2
	NUMPAD_3_NUM_ON,			// 3
	NUMPAD_4_NUM_ON,			// 4
	NUMPAD_5_NUM_ON,			// 5
	NUMPAD_6_NUM_ON,			// 6
	NUMPAD_7_NUM_ON,			// 7
	NUMPAD_8_NUM_ON,			// 8
	NUMPAD_9_NUM_ON,			// 9
	NUMPAD_ASTERISK,			// アスタリスク(掛け算)
	NUMPAD_PLUS,				// プラス(足し算)
	NUMPAD_MINUS,				// マイナス(引き算)
	NUMPAD_SLASH,				// スラッシュ(割り算)
	NUMPAD_5_NUM_OFF,			// 何も文字をかけない取得出来るだけ
};

// キーボード　トグルの状態
enum class KEY_BOARD_TOGGLE
{
	NUM_LOCK = 0,	// num(ナム)ロック
	CAPS_LOCK,		// caps(キャプチャー)ロック
	SCROLL_LOCK,	// Scroll(スクロール)ロック
};

/*--------------------*/
/*     【マウス】     */
/*--------------------*/

// マウスのフラグ種類
enum class MOUSE_TYPE
{
	/*右ボタン*/
	RIGHT_BUTTON = 0,
	/*左ボタン*/
	LEFT_BUTTON,
	/*ホイールボタン*/
	WHEEL_BUTTON,
	/*ホイール前方向回転*/
	WHEEL_FRONT,
	/*ホイール後ろ方向回転*/
	WHEEL_BACK,
	/*マウス右移動*/
	MOVE_RIGHT,
	/*マウス左移動*/
	MOVE_LEFT,
	/*マウス前移動*/
	MOVE_FRONT,
	/*マウス後ろ移動*/
	MOVE_BACK,

	/*マウスのフラグ種類最大数*/
	MAX
};

// カーソルのポジションデータ種類
enum class CURSOR_POSITION_TYPE
{
	/*現在移動している方向に進み始めたポジション*/
	START = 0,
	/*一つ前*/
	PREV,
	/*現在*/
	NOW,

	/*カーソルのポジションデータ種類最大数*/
	MAX
};