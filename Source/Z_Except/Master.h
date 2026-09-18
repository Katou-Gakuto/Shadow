#pragma once

// シーンマネージャー
class BaseSceneManager;
// 終了マネージャー
class EndManager;
// 時間マネージャー
class TimeManager;
// キー情報
class KeyState;
// 光域の管理をするクラス
class LightAreaManager;

// 
class Player;

// 
class VECTOR2D;

// 
class Master
{
public:
    // シーンのManager
    static BaseSceneManager *mpBaseSceneManager;

    // 終了の管理
    static EndManager* mpEndManager;
    // 時間の管理
    static TimeManager* mpTimeManager;

    // キーの情報管理
    static KeyState* mpKeyState;

    // 
    static Player *mpPlayerLight;

    // 
    static Player *mpPlayerShadow;

    // そのほかManager
    static LightAreaManager *mpLightManager;

    // Masterの各メンバをnewする関数
    static int Initialize();

    // Masterの各メンバをdeleteする関数
    static int Finalize();

    // プレイヤーの大きさ
    static int PlayerSizeXY;

    // 画像を拡大/縮小し、UV座標を指定して描画する関数
    static int DrawGraphAnim(
        const VECTOR2D &posLeftUp, const VECTOR2D &posRightDown,
        const VECTOR2D &uvLeftUp, const VECTOR2D &uvRightDown,
        int graphHandle);
};