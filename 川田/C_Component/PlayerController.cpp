#include "PlayerController.h"

#include "../A_GameObject/Player.h"
#include "../A_GameObject/SoundVisualizer.h"
#include "../A_GameObject/FPSCamera.h"
#include "../A_GameObject/PowerSpot.h"
#include "../A_GameObject/GameObjectManager.h"
#include "../C_Component/BaseComponentList.h"
#include "../C_Component/CharacterLife.h"
#include "../E_Scene/BaseSceneManager.h"
#include "../U_Sound/Audio3D.h"
#include "../U_Sound/Sound3D.h"
#include "../U_Sound/Listener.h"
#include "../S_Collision/BaseCollision.h"
#include "../S_Collision/BaseCollisionList.h"
#include "../S_Collision/Sphere3D.h"
#include "../V_Display/System.h"
#include "../R_Input/Input.h"
#include "../W_Network/TCPData.h"
#include "../W_Network/NetworkHome.h"
#include "../Y_Tool/MyFunctions.h"
#include "../Z_Except/Master.h"

PlayerFeelMode PlayerController::mbVisualModeNow = PlayerFeelMode::PFM_Visual;
PlayerFeelMode PlayerController::mbVisualModeSave = PlayerFeelMode::PFM_Visual;

const unsigned long SleepTime = 60 * 30;          // 約30秒
const unsigned long SkillInterval = 60 * 60;      // 約60秒
const unsigned long SkillTime = 60 * 15;          // 約15秒

Player *GetPlayer(GameObject *obj)
{
    return static_cast<Player *>(obj);
}

int PlayerController::UpdateFootsteps(PlayerMoveMode newMove)
{
    int temp = 0;
    bool enemySearch = false;
    switch (newMove)
    {
    case PlayerMoveMode::PMM_NotMove:
        if (this->mnMoveOld != newMove)
        {
            // 音声のカウントを止める
            this->mnFootstepsCount.Deactivate();
        }
        this->mbFootStepsNow = false;
        break;
    case PlayerMoveMode::PMM_Tiptoe:
        if (this->mnMoveOld != newMove)
        {
            // 忍び歩きに移行するときの音声を再生する
            this->mnFootstepsCount.Initialize(60, 0);
        }
        // 足音の更新を行う
        this->mbFootStepsNow = mnFootstepsCount.Update(true);
        break;
    case PlayerMoveMode::PMM_Walk:
        if (this->mnMoveOld != newMove)
        {
            // 通常移動に移行するときの音声を再生する
            this->mnFootstepsCount.Initialize(30, 0);
        }

        // サウンドビジュアライザーが敵に見つかる状態であることを記憶しておく
        enemySearch = true;

        // 足音の更新を行う
        this->mbFootStepsNow = mnFootstepsCount.Update(true);
        break;
    case PlayerMoveMode::PMM_Dash:
        if (this->mnMoveOld != newMove)
        {
            // ダッシュに移行するときの音声を再生する
            this->mnFootstepsCount.Initialize(20, 0);
        }

        // サウンドビジュアライザーが敵に見つかる状態であることを記憶しておく
        enemySearch = true;

        // 足音の更新を行う
        this->mbFootStepsNow = mnFootstepsCount.Update(true);
        break;
    default:
        temp = -1;
        break;
    }

    // 足音を発するタイミングであれば今足音を発生させ、サウンドビジュアライザーを起動する
    if (mbFootStepsNow)
    {
        // 足音の音声を再生する
        this->mpFootstepsSound->SetPosNow(this->GetMyObject3D()->GetPosition());
        this->mpFootstepsSound->Play(true);

        // オブジェクトマネージャーを取得する
        auto objectManager = Master::GetGameObjectManagerNow();
        if (objectManager == nullptr)
        {
            return -1;
        }

        // 足音のサウンドビジュアライザーを起動する
        SoundVisualizer *footstepsSoundVisual = new SoundVisualizer(this->GetMyObject3D()->GetPosition() + VECTOR3D(0.0f, 10.0f, 0.0f));
        footstepsSoundVisual->InitializeParam(
            this->mpFootstepsSound,
            GetMaterial2D(0.3f, 0.3f, 1.0f, 1.0f),
            this->mnFootstepsCount.GetSpan(),
            enemySearch);

        // 
        footstepsSoundVisual->SetPlayerObject(this->GetMyObject());
        
        // 足音をリストに追加する
        objectManager->Add(footstepsSoundVisual);
    }

    // 移動状態を更新する
    this->mnMoveOld = newMove;

    // 結果を返す
    return temp;
}

int PlayerController::UpdateModeChangeCount(bool change)
{
    if (this->mnModeChangeCount.Update(false))
    {
        if (change)
        {
            unsigned long restartPoint = 600;
            switch (PlayerController::mbVisualModeNow)
            {
            case PlayerFeelMode::PFM_Visual:
                PlayerController::mbVisualModeNow = PlayerFeelMode::PFM_Sound;
                PlayerController::mbVisualModeSave = PlayerFeelMode::PFM_Sound;
                if (this->mnAttackCount.GetCountNow() < restartPoint)
                {
                    this->mnAttackCount.Restart(restartPoint);
                }
                break;
            case PlayerFeelMode::PFM_Sound:
                PlayerController::mbVisualModeNow = PlayerFeelMode::PFM_Visual;
                PlayerController::mbVisualModeSave = PlayerFeelMode::PFM_Visual;
                if (this->mnAttackCount.GetCountNow() < restartPoint)
                {
                    this->mnAttackCount.Restart(restartPoint);
                }
                break;
            case PlayerFeelMode::PFM_All:
                // 何もしません
                break;
            }

            // モード変更のリキャスト
            this->mnModeChangeCount.Restart();
        }
    }
    return 0;
}

int PlayerController::UpdateSleep(bool move)
{
    if (move)
    {
        this->mnSleepCount.Restart();
        return 0;
    }
    this->mnSleepCount.Update(false);
    return 0;
}

int PlayerController::UpdateSkill(bool change)
{
    // 現在スキル使用中かを判断する
    if (PlayerController::mbVisualModeNow == PlayerFeelMode::PFM_All)
    {
        // スキルが終了するまでのカウントダウンを進め、終了する瞬間になったらさらに処理を行う
        if (this->mnSkillCount.Update(false))
        {
            // スキルのカウントダウンを再設定する
            this->mnSkillCount.Initialize(SkillInterval);

            // スキルを使用する前の状態に戻す
            PlayerController::mbVisualModeNow = PlayerController::mbVisualModeSave;
        }
    }
    else
    {
        // スキルを使用できるようになるまでのカウントダウンを進め、その瞬間になったらさらに処理を行う
        if (this->mnSkillCount.Update(false))
        {
            // スキルを使用しようとしている場合は処理を行う
            if (change)
            {
                // スキルのカウントダウンを再設定する
                this->mnSkillCount.Initialize(SkillTime);

                // 現在のモードを記憶しておく
                PlayerController::mbVisualModeSave = PlayerController::mbVisualModeNow;

                // PFMをスキル使用中の設定に変更する
                PlayerController::mbVisualModeNow = PlayerFeelMode::PFM_All;
            }
        }
    }

    // 関数が正常終了したことを返す
    return 0;
}

bool PlayerController::CheckUpdateAttack()
{
    return this->mnAttackCount.Update(false);
}

PlayerController::PlayerController(GameObject *myObject) :
    mpFootstepsSound(nullptr),
    mpCamera(nullptr),
    mnFootstepsCount(),
    mnAttackCount(),
    mnSleepCount(),
    mnModeChangeCount(),
    mnMoveOld(PlayerMoveMode::PMM_NotMove),
    mnSkillCount(),
    mbFootStepsNow(false),
    mbPowerSpotFirst(true),
    BaseComponent(myObject, ComponentTagAndOrder::CTAO_PlayerController)
{

}

PlayerController::~PlayerController()
{

}

int PlayerController::Create()
{
    // 音の設定をする
    this->mpFootstepsSound = new Sound3D();
    this->mpFootstepsSound->Initialize(SoundHandle::SoundHandle_Footsteps);
    this->mpFootstepsSound->SetLoopFlag(false);
    this->mpFootstepsSound->WorldConnectMySelf(SoundTag::SoundTag_PlayerWalk);
    this->mpFootstepsSound->SetVolumeChangePlayerModeFlag(true);

    // 
    return 0;
}

int PlayerController::Initialize()
{
    // カメラを取得する
    auto objectManager = Master::GetGameObjectManagerNow();
    if (objectManager == nullptr)
    {
        return -1;
    }
    this->mpCamera = dynamic_cast<FPSCamera *>(objectManager->GetCameraNum(UseCameraNumber::UCN_MainCamera));
    if (this->mpCamera != nullptr)
    {
        // カメラの回転を全て0にしておく
        this->mpCamera->SetHorizontalRadian(0.0f);
        this->mpCamera->SetVerticalRadian(0.0f);
        this->mpCamera->SetCameraRollRadian(0.0f);
        this->mpCamera->SetViewingAngle(MyFunctions::Deg2Rad(120.0f));
    }

    this->mnFootstepsCount.Deactivate();
    this->mnAttackCount.Initialize(900, 600);
    this->mnModeChangeCount.Initialize(200, 0);
    this->mnSleepCount.Initialize(100000);
    this->mnSkillCount.Initialize(SkillInterval);

    PlayerController::mbVisualModeNow = PlayerFeelMode::PFM_Visual;
    PlayerController::mbVisualModeSave = PlayerFeelMode::PFM_Visual;

    return 0;
}

int PlayerController::Finalize()
{
    return 0;
}

int PlayerController::EarlyUpdate()
{
    return 0;
}

int PlayerController::Update()
{
    unsigned long myNumber = Master::mpNetworkHome->GetMyNumber();
    if (this->mpCamera == nullptr)
    {
        return 0;
    }
    if (this->mnSleepCount.CheckNow())
    {
        // いびきを再生して、いびきのサウンドビジュアライザーを生成する

        return 0;
    }

    // カメラの方向設定
    float horizontal = 0.0f;
    float vertical = 0.0f;

    // マウスの移動量を取得
    VECTOR2D mouseMove = Master::mpInput->GetMouseVec();
    if (mouseMove != VECTOR2D::GetZero())
    {
        // マウス感度を設定する
        const float mouseSensitivity = 0.001f;

        // マウスの移動量をそのまま回転に入力する
        horizontal = mouseMove.GetX() * 0.0005f;
        vertical = mouseMove.GetY() * 0.0005f;

        // カメラに回転量を追加する
        this->mpCamera->AddHorizontalRadian(horizontal);
        this->mpCamera->AddVerticalRadian(vertical);
    }

    // スペースキーを押していて、切り替えが可能ならここでVisualとSoundを切り替える
    bool changeFlag = false;
    if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Key_Space))
    {
        changeFlag = true;
    }
    this->UpdateModeChangeCount(changeFlag);

    // Fキーの入力状態を取得し、スキルに関する処理をまとめて行う
    bool skillUse = false;
    if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Key_F))
    // if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Mouse_RightCrick))
    {
        skillUse = true;
    }
    this->UpdateSkill(skillUse);

    // 自身の進む方向を取得する
    VECTOR3D cameraFrontVec = this->mpCamera->GetFrontVec().To2D('x', 'z').To3D('x', 0.0f, 'y');
    VECTOR3D cameraLeftVec = VECTOR3D::Cross(cameraFrontVec, VECTOR3D::GetUp());
    bool moveFlag = false;

    // 自身の移動に関する変数を取得
    float speed = 5.0f;
    VECTOR3D moveVec;
    PlayerMoveMode nowPMM = PlayerMoveMode::PMM_NotMove;
    if (Master::mpInput->CheckInputKey(InputTag::InputTag_Key_W))
    {
        moveVec += +cameraFrontVec;
    }
    if (Master::mpInput->CheckInputKey(InputTag::InputTag_Key_S))
    {
        moveVec += -cameraFrontVec;
    }
    if (Master::mpInput->CheckInputKey(InputTag::InputTag_Key_A))
    {
        moveVec += +cameraLeftVec;
    }
    if (Master::mpInput->CheckInputKey(InputTag::InputTag_Key_D))
    {
        moveVec += -cameraLeftVec;
    }

    // 自身のmoveVecが0ベクトルでないなら処理を行う
    if (moveVec != VECTOR3D::GetZero())
    {
        // 左Ctrlキーを押していたら忍び歩き
        if (Master::mpInput->CheckInputKey(InputTag::InputTag_Key_LeftCtrl))
        {
            speed *= 0.5f;
            nowPMM = PlayerMoveMode::PMM_Tiptoe;
        }
        // shiftキーを押していたらダッシュ
        else if (Master::mpInput->CheckInputKey(InputTag::InputTag_Key_LeftShift))
        {
            speed *= 2.0f;
            nowPMM = PlayerMoveMode::PMM_Dash;
        }
        // 特に押していない場合は通常の移動
        else
        {
            nowPMM = PlayerMoveMode::PMM_Walk;
        }

        // 移動量を調節する
        moveVec = moveVec.Normalize() * speed;

        // 自身のオブジェクト情報を取得する
        GameObject3D *myObject = this->GetMyObject3D();
        if (myObject != nullptr)
        {
            // 移動量を設定する
            myObject->SetMoveVec3D(moveVec);

            // 体の当たり判定を取得する
            BaseCollision *body = myObject->GetBaseCollisionList()->SearchCollisionNum(0);

            // 体の当たり判定の移動量も設定しておく
            body->SetMoveVec(&moveVec);

            // myObject->GetBaseCollisionList()->SetCollisionMoveVec(CollisionDimension_3D, &moveVec);
        }

        // 移動していることを明確にする
        moveFlag = true;
    }

    // 自身の足音関連のアップデートを行う
    this->UpdateFootsteps(nowPMM);

    // オンライン状態なら処理を行う
    if (myNumber != 0)
    {
        // 現在の移動状態に関するデータを生成する
        UDPData_OtherPlayerMove data;
        data.mnGameFrame = Master::GetGameFrameCount();
        data.mnPMM = this->mnMoveOld;
        data.mvPos = this->GetMyObject3D()->GetPosition();
        data.mvMoveVec = moveVec;

        // データを送信する
        Master::mpNetworkHome->SendGameDataUDP(UDPDataType::PDT_ITR_OtherPlayerMove, &data, sizeof(data), 0);
    }

    // 眠ってないかの確認をする
    this->UpdateSleep(moveFlag);

    // パワースポット関連の処理を行う
    bool powerSpotProcessing = false;
    if (Master::mpInput->CheckInputKey(InputTag::InputTag_Mouse_LeftCrick))
    {
        auto objManager = Master::mpSceneManager->SearchSceneNow()->GetGameObjectManager();
        auto powerSpot = objManager->SearchGameObjectsTag(GameObjectTag::GOT_PowerSpot);
        if (0 < powerSpot.Count())
        {
            auto spot = static_cast<PowerSpot *>(powerSpot[0]);
            if (spot != nullptr)
            {
                auto myBody = static_cast<BaseCollision3D *>(this->GetMyObject()->GetBaseCollisionList()->SearchCollisionNum(0));
                if (spot->CheckInSpot(myBody))
                {
                    spot->PowerCharge(this->GetMyObject());
                    powerSpotProcessing = true;

                    // 初めてパワースポットに触った場合は処理
                    if (this->mbPowerSpotFirst)
                    {
                        // 既に今回のパワースポットに触ったことを記憶しておく
                        this->mbPowerSpotFirst = false;

                        // オンライン状態なら処理を行う
                        if (myNumber != 0)
                        {
                            // パワースポットに触ったことを、再送信アリの設定で送信する
                            UDPData_OtherPlayerPowerCharge data;
                            data.mnGameFrame = Master::GetGameFrameCount();
                            Master::mpNetworkHome->SendGameDataUDP(
                                UDPDataType::PDT_ITR_OtherPlayerPowerCharge,
                                &data,
                                sizeof(data),
                                Master::mpNetworkHome->GetNextACK());
                        }
                    }
                }
            }
        }
    }

    // パワースポットに触れていない場合は処理を行う
    if (powerSpotProcessing == false)
    {
        // パワースポットから手を離したので
        this->mbPowerSpotFirst = true;

        // 攻撃関連の処理をまとめて行う
        bool canAttack = this->mnAttackCount.Update(false);
        if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Mouse_LeftCrick))
        {
            // 攻撃が可能なら処理の確認を行う
            if (canAttack && PlayerFeelMode::PFM_Visual != PlayerController::mbVisualModeNow)
            {
                Player *myPlayer = static_cast<Player *>(this->GetMyObject());
                if (myPlayer != nullptr)
                {
                    this->mnAttackCount.Restart();
                    myPlayer->Attack(cameraFrontVec);
                }
            }
        }
    }

    // 成功したことを返す
    return 0;
}

int PlayerController::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    // 自身の攻撃の当たり判定が相手の体の当たり判定にあたっている場合は処理を行う
    if (myCollision->GetCollisionTag() == CollisionTag::CollisionTag_Attack &&
        hitCollision->GetCollisionTag() == CollisionTag::CollisionTag_CharaBody &&
        myCollision->GetMyObject() != hitCollision->GetMyObject())
    {
        auto life = (CharacterLife *)hitCollision->GetMyObject()->GetBaseComponentList()->SearchComponent(CTAO_CharacterLife)[0];
        if (life != nullptr)
        {
            // 残りライフを減らす
            life->Hit();

            // オンライン状態であればさらに処理を行う
            if (Master::mpNetworkHome->GetMyNumber() != 0)
            {
                // 
                TCPData_OtherPlayerAttackHit data;
                data.mnGameFrame = Master::GetGameFrameCount();

                // 当てたことを送信する
                Master::mpNetworkHome->SendGameDataTCP(TCPDataType::PDT_ITR_OtherPlayerAttackHit, &data, sizeof(data));
            }
        }
    }
    return 0;
}

int PlayerController::LateUpdate()
{
    // 
    if (this->mpCamera != nullptr)
    {
        // 
        this->mpCamera->SetCameraPosition(this->GetMyObject3D()->GetPosition() + VECTOR3D(0.0f, 180.0f, 0.0f));

        // 音声の再生関係も設定しておく
        auto masterAudio = dynamic_cast<Audio3D *>(Master::mpAudio);
        if (masterAudio != nullptr)
        {
            // リスナーの移動を行う
            auto listener = masterAudio->GetListener();
            if (listener != nullptr)
            {
                // 座標と各種方向を更新する
                listener->SetNowParam(
                    this->mpCamera->GetPosition(),
                    this->mpCamera->GetFrontVec(),
                    this->mpCamera->GetTopVec().To3D(0.0f));
            }
        }
    }

    // 成功したことを返す
    return 0;
}

int PlayerController::Draw()
{
    // Master::mpSystem->DrawText2D(L"波形のInstancingDraw、完了！", 0, 0, D2D1::ColorF::White);
    return 0;
}

PlayerFeelMode PlayerController::GetFeelModeNow()
{
    return PlayerController::mbVisualModeNow;
}

PlayerFeelMode PlayerController::GetFeelModeSave()
{
    return PlayerController::mbVisualModeSave;
}
