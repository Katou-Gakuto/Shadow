#include "SoloBattleScene.h"

#include "../A_GameObject/GameObjectManager.h"
#include "../A_GameObject/FPSCamera.h"
#include "../A_GameObject/LineMap.h"
#include "../A_GameObject/Obstacle.h"
#include "../A_GameObject/Fence.h"
#include "../A_GameObject/TestEmitter.h"
#include "../A_GameObject/Player.h"
#include "../A_GameObject/Enemy.h"
#include "../A_GameObject/PowerSpot.h"

#include "../C_Component/BaseComponentList.h"
#include "../C_Component/CharacterLife.h"
#include "../C_Component/PlayerController.h"

#include "../E_Scene/BaseSceneManager.h"
#include "../E_Scene/ResultScene.h"
#include "../S_Collision/BaseCollision2DManager.h"
#include "../S_Collision/BaseCollision3DManager.h"

#include "../U_Sound/Sound3D.h"
#include "../U_Sound/Audio3D.h"
#include "../U_Sound/Listener.h"
#include "../V_Display/DisplayEnum.h"
#include "../V_Display/System.h"
#include "../R_Input/Input.h"
#include "../Y_Tool/MyFunctions.h"
#include "../Z_Except/ResourceManager.h"
#include "../Z_Except/Master.h"

SoloBattleScene::SoloBattleScene() :
    mpPlayer(nullptr),
    mpEnemy(nullptr),
    mpPowerSpot(nullptr),
    mpIconVisual(nullptr),
    mpIconSound(nullptr),
    mpIconAttack(nullptr),
    mpIconSkill(nullptr),
    mdStartTime(),
    mdAlarm(0),
    mbFirstOver(false),
    BaseScene(SceneTag::ST_SoloBattle)
{
}

SoloBattleScene::~SoloBattleScene()
{

}

int SoloBattleScene::SetLoseObject(GameObject *loseObject)
{
    // ソロモードで戦っていたことを記録しておく
    *(SceneTag *)Master::mpSceneManager->GetCSD(CSDN::CSDN_SceneTag_BattleMode) = this->GetTag();

    // どっちが勝ったか
    if (loseObject == this->mpEnemy)
    {
        // プレイヤーが勝った事を報告する
        *(bool *)Master::mpSceneManager->GetCSD(CSDN::CSDN_bool_PlayerWin) = true;

        // 勝敗の結果発表をするシーンに移行する
        Master::mpSceneManager->ChangeScene(SceneTag::ST_Result);
    }
    else if (loseObject == this->mpPlayer)
    {
        // プレイヤーが負けた事を報告する
        *(bool *)Master::mpSceneManager->GetCSD(CSDN::CSDN_bool_PlayerWin) = false;

        // 勝敗の結果発表をするシーンに移行する
        Master::mpSceneManager->ChangeScene(SceneTag::ST_Result);
    }
    else
    {
        // 引数が異常だったので処理をしない
        return -1;
    }

    // 関数が正常終了したことを返す
    return 0;
}

int SoloBattleScene::Create()
{
    // UIに使用するモデルを生成する
    mpIconVisual =      new Model2D(TextureHandle::TextureHandle_Battle_IconVisual, nullptr, nullptr, ScreenNumber::ScreenNumber_2DUI);
    mpIconSound =       new Model2D(TextureHandle::TextureHandle_Battle_IconSound, nullptr, nullptr, ScreenNumber::ScreenNumber_2DUI);
    mpIconAttack =      new Model2D(TextureHandle::TextureHandle_Battle_IconAttack, nullptr, nullptr, ScreenNumber::ScreenNumber_2DUI);
    mpIconSkill =       new Model2D(TextureHandle::TextureHandle_Battle_IconSkill, nullptr, nullptr, ScreenNumber::ScreenNumber_2DUI);

    // GameObjectを生成してリストに追加する
    FPSCamera *camera = new FPSCamera(VECTOR3D(0.0f, 0.0f, -500.0f));
    this->mpGameObjectManager->Add(camera);
    this->mpGameObjectManager->AddCamera(camera, UseCameraNumber::UCN_MainCamera);

    float mapSize = 2500.0f;
    float areaSize = (mapSize * 2) / 10;
    float mapLeft = -mapSize;
    float mapFront = mapSize;
    float mapHeight = 500.0f;
    float mapRight = mapSize;
    float mapBack = -mapSize;
    float mapGround = 0.0f;
    float obstacleHeight = 100.0f;

    float left2right = areaSize;
    float front2back = -areaSize;
    int areaAxisMax = 19;

    this->mpGameObjectManager->Add(new LineMap(mapSize));

    this->mpGameObjectManager->Add(new Fence(
        VECTOR3D(mapRight, mapHeight, mapFront),
        VECTOR3D(mapLeft, mapHeight, mapFront),
        VECTOR3D(mapRight, mapGround, mapFront),
        VECTOR3D(mapLeft, mapGround, mapFront),
        VECTOR3D(mapLeft, mapHeight, mapBack),
        VECTOR3D(mapRight, mapHeight, mapBack),
        VECTOR3D(mapLeft, mapGround, mapBack),
        VECTOR3D(mapRight, mapGround, mapBack)));

    // でっかい障害物
    this->mpGameObjectManager->Add(new Obstacle(
        VECTOR3D(mapLeft + left2right * 1, obstacleHeight, mapFront + front2back * 1),
        VECTOR3D(mapLeft + left2right * 4, mapGround, mapFront + front2back * 4)));

    // ぐるぐる障害物
    this->mpGameObjectManager->Add(new Obstacle(
        VECTOR3D(mapLeft + left2right * 5, obstacleHeight, mapFront + front2back * 0),
        VECTOR3D(mapLeft + left2right * 6, mapGround, mapFront + front2back * 4)));

    this->mpGameObjectManager->Add(new Obstacle(
        VECTOR3D(mapLeft + left2right * 7, obstacleHeight, mapFront + front2back * 3),
        VECTOR3D(mapLeft + left2right * 10, mapGround, mapFront + front2back * 4)));

    this->mpGameObjectManager->Add(new Obstacle(
        VECTOR3D(mapLeft + left2right * 7, obstacleHeight, mapFront + front2back * 2),
        VECTOR3D(mapLeft + left2right * 8, mapGround, mapFront + front2back * 3)));

    this->mpGameObjectManager->Add(new Obstacle(
        VECTOR3D(mapLeft + left2right * 7, obstacleHeight, mapFront + front2back * 1),
        VECTOR3D(mapLeft + left2right * 9, mapGround, mapFront + front2back * 2)));


    // 迷路と広場の障害物
    this->mpGameObjectManager->Add(new Obstacle(
        VECTOR3D(mapLeft + left2right * 1, obstacleHeight, mapFront + front2back * 5),
        VECTOR3D(mapLeft + left2right * 10, mapGround, mapFront + front2back * 6)));

    this->mpGameObjectManager->Add(new Obstacle(
        VECTOR3D(mapLeft + left2right * 1, obstacleHeight, mapFront + front2back * 8),
        VECTOR3D(mapLeft + left2right * 4, mapGround, mapFront + front2back * 9)));

    this->mpGameObjectManager->Add(new Obstacle(
        VECTOR3D(mapLeft + left2right * 3, obstacleHeight, mapFront + front2back * 9),
        VECTOR3D(mapLeft + left2right * 4, mapGround, mapFront + front2back * 10)));

    this->mpGameObjectManager->Add(new Obstacle(
        VECTOR3D(mapLeft + left2right * 3, obstacleHeight, mapFront + front2back * 7),
        VECTOR3D(mapLeft + left2right * 4, mapGround, mapFront + front2back * 8)));

    this->mpGameObjectManager->Add(new Obstacle(
        VECTOR3D(mapLeft + left2right * 1, obstacleHeight, mapFront + front2back * 6),
        VECTOR3D(mapLeft + left2right * 2, mapGround, mapFront + front2back * 7)));

    this->mpGameObjectManager->Add(new Obstacle(
        VECTOR3D(mapLeft + left2right * 5, obstacleHeight, mapFront + front2back * 7),
        VECTOR3D(mapLeft + left2right * 6, mapGround, mapFront + front2back * 8)));

    this->mpGameObjectManager->Add(new Obstacle(
        VECTOR3D(mapLeft + left2right * 5, obstacleHeight, mapFront + front2back * 9),
        VECTOR3D(mapLeft + left2right * 6, mapGround, mapFront + front2back * 10)));

    this->mpEnemy = new Enemy(VECTOR3D::GetZero());
    this->mpGameObjectManager->Add(this->mpEnemy);

    this->mpPlayer = new Player(VECTOR3D(0.0f, 0.0f, -500.0f));
    this->mpGameObjectManager->Add(this->mpPlayer);

    this->mpPowerSpot = new PowerSpot();
    this->mpGameObjectManager->Add(this->mpPowerSpot);

    // イコライザーのスクショ用
    // this->mpGameObjectManager->Add(new TestEmitter(VECTOR3D::GetZero()));

    // 関数が正常終了したことを返す
    return 0;
}

int SoloBattleScene::Initialize()
{
    // オブジェクトマネージャーをInitializeしてますぜ
    if (this->mpGameObjectManager != nullptr)
    {
        this->mpGameObjectManager->Initialize();
        this->mpGameObjectManager->ObjectCollisionActivate();
    }
    
    VECTOR3D playerPos(-2250.0f, 0.0f, -2250.0f);
    VECTOR3D enemyPos(2250.0f, 0.0f, 2250.0f);

    // プレイヤーの座標を設定する
    this->mpPlayer->SetStartPos(enemyPos);

    // エネミーの座標を設定する
    this->mpEnemy->SetStartPos(playerPos);

    // アラームに設定されている時刻を全て消去する
    this->mdAlarm.Clear();

    // 設定時刻を生成する
    TimeParam nowTime;
    nowTime.Initialize();
    TimeDiff diff;
    diff.mnSec = 5;
    this->mdStartTime.Initialize(nowTime, diff);

    // アラームに時刻を設定する
    this->mdAlarm.AddSetTime(this->mdStartTime);

    // オブジェクトの当たり判定処理を行わないことを明示的にしておく
    this->mpGameObjectManager->ObjectCollisionDeactivate();

    // 次オーバーした時が初めてのオーバーになる
    this->mbFirstOver = true;

    return 0;
}

int SoloBattleScene::Finalize()
{
    if (this->mpGameObjectManager != nullptr)
    {
        this->mpGameObjectManager->ObjectCollisionDeactivate();
    }
    return 0;
}

int SoloBattleScene::Update()
{
    // 最後に過ぎたやつ
    int overIndex = this->mdAlarm.CheckOverIndex();

    // スタート時刻を超えているかどうかで処理を変える
    if (0 <= overIndex)
    {
        // 初めてスタート時刻を超えたかを確認する
        if (this->mbFirstOver)
        {
            // オブジェクトの当たり判定処理を行うことを明記しておく
            this->mpGameObjectManager->ObjectCollisionActivate();

            // ゲームフレームのカウントをリセットする
            Master::ResetGameFrameCount();

            // すでにスタート時刻を超えていたことを記憶しておく
            this->mbFirstOver = false;
        }

        // フルスペックです
        // 移動ベクトルを0ベクトルに設定する
        this->mpGameObjectManager->ResetMoveVec();

        // 通常更新の前にしなければいけない何か
        this->mpGameObjectManager->EarlyUpdate();

        // 通常更新
        this->mpGameObjectManager->Update();

        // 当たり判定の計算
        Master::mpBaseCollision2DManager->CheckHitAllMove();
        Master::mpBaseCollision3DManager->CheckHitAllMove();

        // 当たり判定とオブジェクトの座標の確定
        this->mpGameObjectManager->SetPositionToMoveVec();
        this->mpGameObjectManager->SetCollisionPosToCollisionMoveVec();

        // 当たり判定計算後の更新
        this->mpGameObjectManager->LateUpdate();

        // 削除フラグの立っている当たり判定を削除する
        this->mpGameObjectManager->DeleteToFlagMember();

        // 削除フラグの立っているオブジェクトを削除する
        this->mpGameObjectManager->DeleteToFlag();

        // モデルの更新
        this->mpGameObjectManager->UpdateModel();
    }
    else
    {
        // 現在時刻を取得
        TimeParam now;
        now.Initialize();

        // 開始時刻 - 現在時刻を取得し、残り何秒かを取得する
        auto timeDiff = this->mdStartTime - now;

        // 
        std::wstring str;
        switch (timeDiff.mnSec)
        {
        case 0:
            str += L"0";
            break;
        case 1:
            str += L"1";
            break;
        case 2:
            str += L"2";
            break;
        case 3:
            str += L"3";
            break;
        case 4:
            str += L"4";
            break;
        case 5:
            str += L"5";
            break;
        default:
            str += L"?";
            break;
        }

        // カウントダウンを表示
        Master::mpSystem->DrawText2D(str, 600, 300, Material2D(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // 関数が正常終了したことを返す
    return 0;
}

int SoloBattleScene::Draw()
{
    // モデルの描画
    this->mpGameObjectManager->Draw();

    // プレイヤーの情報などを取得
    auto playerController = (PlayerController *)this->mpPlayer->GetBaseComponentList()->SearchComponent(CTAO_PlayerController)[0];
    auto playerLife = (CharacterLife *)this->mpPlayer->GetBaseComponentList()->SearchComponent(ComponentTagAndOrder::CTAO_CharacterLife)[0];

    // 敵のライフを取得
    auto enemyLife = (CharacterLife *)this->mpEnemy->GetBaseComponentList()->SearchComponent(ComponentTagAndOrder::CTAO_CharacterLife)[0];

    // パワースポットを取得
    PowerSpot *power = (PowerSpot *)this->mpPowerSpot;

    // 取得したものすべてがnullptrじゃないなら処理を行う
    if (playerController == nullptr ||
        playerLife == nullptr ||
        enemyLife == nullptr)
    {
        return -1;
    }

    if (Master::mpSystem->ChangeDrawScreen(ScreenNumber::ScreenNumber_3DObject) != 0)
    {
        return -1;
    }

    // 東西南北をわかりやすくするための線を描画する(×4)
    // 北(赤)
    Master::mpSystem->DrawLineLight3D(
        VECTOR3D(0.0f, 0.0f, 3000.0f),
        VECTOR3D(0.0f, 1000.0f, 3000.0f),
        Material2D(1.0f, 0.3f, 0.3f, 1.0f), 
        1.3f,
        30.0f);
    // 東(青)
    Master::mpSystem->DrawLineLight3D(
        VECTOR3D(3000.0f, 0.0f, 0.0f),
        VECTOR3D(3000.0f, 1000.0f, 0.0f),
        Material2D(0.3f, 0.3f, 1.0f, 1.0f), 
        1.3f,
        30.0f);
    // 南(緑)
    Master::mpSystem->DrawLineLight3D(
        VECTOR3D(0.0f, 0.0f, -3000.0f),
        VECTOR3D(0.0f, 1000.0f, -3000.0f),
        Material2D(0.3f, 1.0f, 0.3f, 1.0f),
        1.3f,
        30.0f);
    // 西(黄)
    Master::mpSystem->DrawLineLight3D(
        VECTOR3D(-3000.0f, 0.0f, 0.0f),
        VECTOR3D(-3000.0f, 1000.0f, 0.0f),
        Material2D(1.0f, 1.0f, 0.2f, 1.0f),
        1.3f,
        30.0f);

    if (Master::mpSystem->ChangeDrawScreen(ScreenNumber::ScreenNumber_2DUI) != 0)
    {
        return -1;
    }

    // 使用する値を取得
    CountDownSystem &attackCount = playerController->GetAttackCount();
    CountDownSystem &modeChangeCount = playerController->GetModeChangeCount();
    CountDownSystem &skillCount = playerController->GetSkillCount();
    auto playerHP = playerLife->GetLifeCount();
    auto enemyHP = enemyLife->GetLifeCount();
    PlayerFeelMode nowPFM = PlayerController::GetFeelModeNow();

    // HP
    float hpStart = 40.0f;
    float hpWidth = 120.0f;
    float hpSpaceWidth = 10.0f;
    float hpPlayerUp = 550.0f;
    float hpBottom = 20.0f;
    float hpEnemyUp = 590.0f;
    VECTOR2D tempStart0;
    VECTOR2D tempEnd0;
    VECTOR2D tempStart1 = { 0.0f, 0.0f };
    VECTOR2D tempEnd1 = { 1.0f, 1.0f };
    Material2D color;
    for (unsigned long i = 0; i < 3; i++)
    {
        if (i <= playerHP)
        {
            tempStart0.SetXY(   hpStart + (hpSpaceWidth * i) + (hpWidth * i) ,      hpPlayerUp);
            tempEnd0.SetXY(     hpStart + (hpSpaceWidth * i) + (hpWidth * (i + 1)), hpPlayerUp + hpBottom);
            color = GetMaterial2D(0.5f, 0.5f, 1.0f, 1.0f);
            Master::mpSystem->DrawPoly2DLight(tempStart0, tempEnd0, color, 1.0f);
        }
        if (i <= enemyHP)
        {
            tempStart0.SetXY(   hpStart + (hpSpaceWidth * i) + hpWidth * i,          hpEnemyUp);
            tempEnd0.SetXY(     hpStart + (hpSpaceWidth * i) + hpWidth * (i + 1),    hpEnemyUp + hpBottom);
            color = GetMaterial2D(1.0f, 0.5f, 0.5f, 1.0f);
            Master::mpSystem->DrawPoly2DLight(tempStart0, tempEnd0, color, 1.0f);
        }
    }

    // アイコン
    float darkIcon = 0.3f;
    float ratio = 0.0f;
    float normalIconSize = 100.0f;
    float modeIconSize = 150.0f;
    float noneModeIconSize = 75.0f;

    // 攻撃アイコン
    tempStart0.SetXY(1080.0f, 420.0f);
    tempEnd0 = tempStart0 + VECTOR2D(normalIconSize, normalIconSize);
    if (PlayerController::GetFeelModeNow() == PlayerFeelMode::PFM_Visual)
    {
        ratio = attackCount.GetRatioProgress(true);
        Master::mpSystem->DrawPoly2DDark(
            this->mpIconAttack->GetHandle(),
            tempStart0, tempEnd0,
            VECTOR2D(0.0f, 0.0f), VECTOR2D(1.0f, 1.0f),
            VECTOR2D(0.0f, 0.0f), VECTOR2D(1.0f, 1.0f),
            1.0f, darkIcon);
    }
    else
    {
        ratio = attackCount.GetRatioProgress(true);
        Master::mpSystem->DrawPoly2DDark(
            this->mpIconAttack->GetHandle(),
            tempStart0, tempEnd0,
            VECTOR2D(0.0f, 0.0f), VECTOR2D(1.0f, 1.0f),
            VECTOR2D(0.0f, 0.0f), VECTOR2D(1.0f, 1.0f - ratio),
            1.0f, darkIcon);
    }

    // スキルアイコン
    // スキル使用中かどうかで処理を変える
    tempStart0.SetXY(1170.0f, 500.0f);
    tempEnd0 = tempStart0 + VECTOR2D(normalIconSize, normalIconSize);
    if (PlayerController::GetFeelModeNow() == PlayerFeelMode::PFM_All)
    {
        // スキル使用中なので、スキルの効果時間が切れるまでの値を取得する
        ratio = skillCount.GetRatioRemaining(true);
        Master::mpSystem->DrawPoly2DDark(
            this->mpIconSkill->GetHandle(),
            tempStart0, tempEnd0,
            VECTOR2D(0.0f, 0.0f), VECTOR2D(1.0f, 1.0f),
            VECTOR2D(0.0f, 0.0f), VECTOR2D(1.0f, 1.0f - ratio),
            1.0f, darkIcon);
    }
    else
    {
        // スキル使用前なので、スキルを使用出来るようになるでの値を取得する
        ratio = skillCount.GetRatioProgress(true);
        Master::mpSystem->DrawPoly2DDark(
            this->mpIconSkill->GetHandle(),
            tempStart0, tempEnd0,
            VECTOR2D(0.0f, 0.0f), VECTOR2D(1.0f, 1.0f),
            VECTOR2D(0.0f, 0.0f), VECTOR2D(1.0f, 1.0f - ratio),
            1.0f, darkIcon);
    }

    // モードアイコン
    // スキル中かどうかに関わらず、
    tempStart0.SetXY(980.0f, 520.0f);
    tempEnd0 = tempStart0 + VECTOR2D(modeIconSize, modeIconSize);
    tempStart1.SetXY(1140.0f, 610.0f);
    tempEnd1 = tempStart1 + VECTOR2D(noneModeIconSize, noneModeIconSize);
    if (PlayerController::GetFeelModeSave() == PlayerFeelMode::PFM_Visual)
    {
        // 現在のモードアイコン
        Master::mpSystem->DrawSprite(
            this->mpIconVisual->GetHandle(),
            tempStart0, tempEnd0,
            VECTOR2D(0.0f, 0.0f), VECTOR2D(1.0f, 1.0f));

        // 裏のモードアイコン
        ratio = modeChangeCount.GetRatioProgress(true);
        Master::mpSystem->DrawPoly2DDark(
            this->mpIconSound->GetHandle(),
            tempStart1, tempEnd1,
            VECTOR2D(0.0f, 0.0f), VECTOR2D(1.0f, 1.0f),
            VECTOR2D(0.0f, 0.0f), VECTOR2D(1.0f, 1.0f - ratio),
            1.0f, darkIcon);
    }
    else
    {
        // 現在のモードアイコン
        Master::mpSystem->DrawSprite(
            this->mpIconSound->GetHandle(),
            tempStart0, tempEnd0,
            VECTOR2D(0.0f, 0.0f), VECTOR2D(1.0f, 1.0f));

        // 裏のモードアイコン
        ratio = modeChangeCount.GetRatioProgress(true);
        Master::mpSystem->DrawPoly2DDark(
            this->mpIconVisual->GetHandle(),
            tempStart1, tempEnd1,
            VECTOR2D(0.0f, 0.0f), VECTOR2D(1.0f, 1.0f),
            VECTOR2D(0.0f, 0.0f), VECTOR2D(1.0f, 1.0f - ratio),
            1.0f, darkIcon);
    }

    // パワースポットが再出現している場合は処理を行う
    tempStart0.SetXY(450.0f, 80.0f);
    tempEnd0.SetXY(850.0f, 130.0f);
    color = GetMaterial2D(0.7f, 0.3f, 0.7f, 1.0f);
    if (power->CheckStealthFlag() == false)
    {
        // 『パワースポットが出現しました』
        Master::mpSystem->DrawText2D(L"パワースポットに力が集まっていく……", 435.0f, 50.0f, Material2D(1.0f, 1.0f, 1.0f, 1.0f));

        // 
        ratio = power->GetChargeProgress();
        if (1.0f <= ratio)
        {
            Master::mpSystem->DrawPoly2DLight(
                tempStart0,
                VECTOR2D(MyFunctions::GetFloatLerp(tempStart0.GetX(), tempEnd0.GetX(), ratio), tempEnd0.GetY()),
                Material2D(0.7f, 0.5f, 0.7f, 1.0f),
                1.0f);
        }
        else
        {
            Master::mpSystem->DrawPoly2DLight(
                tempStart0,
                VECTOR2D(MyFunctions::GetFloatLerp(tempStart0.GetX(), tempEnd0.GetX(), ratio), tempEnd0.GetY()),
                color,
                1.0f);
        }
    }

    // 関数が正常終了したことを返す
    return 0;
}
