#include "OtherPlayerController.h"

#include "../A_GameObject/Player.h"
#include "../A_GameObject/OtherPlayer.h"
#include "../A_GameObject/SoundVisualizer.h"
#include "../A_GameObject/FPSCamera.h"
#include "../A_GameObject/PowerSpot.h"
#include "../A_GameObject/GameObjectManager.h"
#include "../C_Component/PlayerController.h"
#include "../C_Component/BaseComponentList.h"
#include "../C_Component/CharacterLife.h"
#include "../E_Scene/BaseSceneManager.h"
#include "../U_Sound/Audio3D.h"
#include "../U_Sound/Sound3D.h"
#include "../U_Sound/Listener.h"
#include "../S_Collision/BaseCollision.h"
#include "../S_Collision/BaseCollisionList.h"
#include "../S_Collision/Sphere3D.h"
#include "../S_Collision/Capsule3D.h"
#include "../V_Display/System.h"
#include "../W_Network/UDPData.h"
#include "../W_Network/TCPData.h"
#include "../W_Network/NetworkHome.h"
#include "../R_Input/Input.h"
#include "../Y_Tool/MyFunctions.h"
#include "../Z_Except/Master.h"

const unsigned long SleepTime = 60 * 30;          // 約30秒
const unsigned long SkillInterval = 60 * 60;      // 約60秒
const unsigned long SkillTime = 60 * 15;          // 約15秒

OtherPlayer *GetOtherPlayer(GameObject *obj)
{
    return static_cast<OtherPlayer *>(obj);
}

int OtherPlayerController::UpdateFootsteps(PlayerMoveMode newMove)
{
    int temp = 0;
    switch (newMove)
    {
    case PlayerMoveMode::PMM_NotMove:
        if (this->mnOldMove != newMove)
        {
            // 音声のカウントを止める
            this->mnFootstepsCount.Deactivate();
        }
        this->mbFootStepsNow = false;
        break;
    case PlayerMoveMode::PMM_Tiptoe:
        if (this->mnOldMove != newMove)
        {
            // 忍び歩きに移行するときの音声を再生する
            this->mnFootstepsCount.Initialize(60, 0);
        }
        // 足音の更新を行う
        this->mbFootStepsNow = mnFootstepsCount.Update(true);
        break;
    case PlayerMoveMode::PMM_Walk:
        if (this->mnOldMove != newMove)
        {
            // 通常移動に移行するときの音声を再生する
            this->mnFootstepsCount.Initialize(30, 0);
        }
        // 足音の更新を行う
        this->mbFootStepsNow = mnFootstepsCount.Update(true);
        break;
    case PlayerMoveMode::PMM_Dash:
        if (this->mnOldMove != newMove)
        {
            // ダッシュに移行するときの音声を再生する
            this->mnFootstepsCount.Initialize(20, 0);
        }
        // 足音の更新を行う
        this->mbFootStepsNow = mnFootstepsCount.Update(true);
        break;
    default:
        temp = -1;
        break;
    }

    // 足音を発するタイミングであれば今足音を発生させ、サウンドビジュアライザーを起動する
    if (this->mbFootStepsNow)
    {
        // 足音の音声を再生する
        this->mpFootstepsSound->SetPosNow(this->GetMyObject3D()->GetPosition());
        this->mpFootstepsSound->Play(true);

        // 足音の波形が出る設定の場合のみ、追加で処理を行う
        if (newMove == PlayerMoveMode::PMM_Walk || newMove == PlayerMoveMode::PMM_Dash)
        {
            // オブジェクトマネージャーを取得する
            auto objectManager = Master::GetGameObjectManagerNow();
            if (objectManager == nullptr)
            {
                return -1;
            }

            // 足音のサウンドビジュアライザーを起動する
            SoundVisualizer* footstepsSound = new SoundVisualizer(this->GetMyObject3D()->GetPosition());
            footstepsSound->InitializeParam(
                this->mpFootstepsSound,
                GetMaterial2D(0.3f, 0.3f, 1.0f, 1.0f),
                this->mnFootstepsCount.GetSpan(),
                true);

            // 足音をリストに追加する
            objectManager->Add(footstepsSound);
        }
    }

    // 移動状態を更新する
    this->mnOldMove = newMove;

    // 結果を返す
    return temp;
}

OtherPlayerController::OtherPlayerController(GameObject *myObject) :
    mpFootstepsSound(nullptr),
    mnFootstepsCount(),
    mnPersonNumber(0),
    mnOldMove(PlayerMoveMode::PMM_NotMove),
    mbFootStepsNow(false),
    mbPowerCharge(false),
    BaseComponent(myObject, ComponentTagAndOrder::CTAO_PlayerController)
{
    unsigned long *data = (unsigned long *)Master::mpSceneManager->GetCSD(CrossSceneDataNumber::CSDN_unsignedLong_OtherPlayerNum);
    this->mnPersonNumber = *data;
}

OtherPlayerController::~OtherPlayerController()
{
}

int OtherPlayerController::Create()
{
    this->mpFootstepsSound = new Sound3D();
    this->mpFootstepsSound->Initialize(SoundHandle::SoundHandle_Footsteps);
    this->mpFootstepsSound->SetLoopFlag(false);
    this->mpFootstepsSound->WorldConnectMySelf(SoundTag::SoundTag_PlayerWalk);
    this->mpFootstepsSound->SetVolumeChangePlayerModeFlag(true);

    return 0;
}

int OtherPlayerController::Initialize()
{
    this->mnFootstepsCount.Deactivate();

    return 0;
}

int OtherPlayerController::Finalize()
{
    return 0;
}

int OtherPlayerController::EarlyUpdate()
{
    return 0;
}

int OtherPlayerController::Update()
{
    // 自身のオブジェクトを取得
    auto myObj = GetOtherPlayer(this->GetMyObject());

    // このフレームで移動状態を変更する可能性があるので保存しておく
    PlayerMoveMode nowPMM = this->mnOldMove;

    // 移動速度
    float speed = 5.0f;

    // 移動に関するデータを取得し、データが空でなければ処理を行う
    auto moveDataList = Master::mpNetworkHome->PullGameDataUDP(UDPDataType::PDT_ITR_OtherPlayerMove);
    if (0 < moveDataList.GetSize())
    {
        // 最新のデータを取得し、nullptrでなければ処理を行う
        auto lastData = Master::mpNetworkHome->GetLastGameData(moveDataList);
        if (lastData != nullptr)
        {
            UDPData_OtherPlayerMove *moveData = (UDPData_OtherPlayerMove *)lastData->GetData();

            // このデータを送信した時点での確定座標をまず代入する
            VECTOR3D newPos = moveData->mvPos;

            // 移動状態に合わせて、速度に補正をかける
            switch (moveData->mnPMM)
            {
            case PlayerMoveMode::PMM_NotMove:
                // 何もしない
                speed = 0.0f;
                break;
            case PlayerMoveMode::PMM_Tiptoe:
                // 移動速度を半減させる
                speed *= 0.5f;
                break;
            case PlayerMoveMode::PMM_Walk:
                // 速度はそのまま
                break;
            case PlayerMoveMode::PMM_Dash:
                // 移動速度を倍増させる
                speed *= 2.0f;
                break;
            default:
                break;
            }

            // 確定座標 += 移動の方向ベクトル×1フレームでの移動量×このデータ送信時からの経過時間
            newPos += (moveData->mvMoveVec * speed) * (Master::GetGameFrameCount() - moveData->mnGameFrame);

            // 渡されたデータ通りの座標に移動する
            myObj->SetPosition(newPos);

            // 自身の当たり判定も移動させる
            auto collisionList = myObj->GetBaseCollisionList();
            auto body = static_cast<Capsule3D *>(collisionList->SearchCollisionNum(0));
            body->SetShapeParameter(
                newPos,
                newPos + VECTOR3D(0.0f, 180.0f, 0.0f),
                100.0f * 100.0f);

            // 次の移動状態を取得する
            nowPMM = moveData->mnPMM;

            // 移動方向を取得しておく
            this->mvNextDir = moveData->mvMoveVec;
        }
    }
    
    // 移動速度
    speed = 5.0f;

    // このフレームから次のフレームにかけての移動を予測する
    VECTOR3D moveVec = this->mvNextDir;
    switch (nowPMM)
    {
    case PlayerMoveMode::PMM_NotMove:
        speed *= 0.0f;
        break;
    case PlayerMoveMode::PMM_Tiptoe:
        speed *= 0.5f;
        break;
    case PlayerMoveMode::PMM_Walk:
        // 特に何もしない
        break;
    case PlayerMoveMode::PMM_Dash:
        speed *= 2.0f;
        break;
    default:
        // 
        break;
    }

    // 計算した移動量をオブジェクトの方に設定する
    myObj->SetMoveVec3D(moveVec);

    // 足音のカウントと自身の移動状態を更新する
    this->UpdateFootsteps(nowPMM);

    // 攻撃を当てたかのデータを確認する
    auto attackDataList = Master::mpNetworkHome->PullGameDataTCP(TCPDataType::PDT_ITR_OtherPlayerAttackHit);
    if (0 < attackDataList.GetSize())
    {
        // 渡されたデータ通りの行動を行う
        auto objManager = Master::GetGameObjectManagerNow();

        // まぁプレイヤーがいないなんてことはないので、nullチェック抜きにしてね
        auto player = (Player *)objManager->SearchGameObjectsTag(GameObjectTag::GOT_Player).operator[](0);
        auto componentList = player->GetBaseComponentList();
        auto life = static_cast<CharacterLife *>(componentList->SearchComponent(ComponentTagAndOrder::CTAO_CharacterLife)[0]);

        // 
        for (unsigned long i = 0; i < attackDataList.GetSize(); i++)
        {
            TCPData_OtherPlayerAttackHit *attackData = (TCPData_OtherPlayerAttackHit *)attackDataList.SearchDataPtrIndex(i)->GetData();

            // プレイヤーに攻撃を当てたらしいので、攻撃判定を。
            life->Hit();
        }
    }

    // パワーチャージを始めているかで処理を変える
    if (this->mbPowerCharge)
    {
        auto objManager = Master::GetGameObjectManagerNow();
        auto powerSpotList = objManager->SearchGameObjectsTag(GameObjectTag::GOT_PowerSpot);
        if (0 < powerSpotList.Count())
        {
            auto powerSpot = static_cast<PowerSpot *>(powerSpotList[0]);

            // パワースポットがステルスモードになっている場合は触れない
            if (powerSpot->CheckStealthFlag())
            {
                this->mbPowerCharge = false;
            }
            else
            {
                // パワースポットに触れる
                powerSpot->PowerCharge(myObj);
            }
        }
    }
    else
    {
        // チャージを始めたらしいです
        auto chargeDataList = Master::mpNetworkHome->PullGameDataUDP(UDPDataType::PDT_ITR_OtherPlayerPowerCharge);
        if (0 < chargeDataList.GetSize())
        {
            // 渡されたデータ通りの行動を行う
            auto gameData = Master::mpNetworkHome->GetLastGameData(chargeDataList);
            UDPData_OtherPlayerPowerCharge *data = (UDPData_OtherPlayerPowerCharge *)gameData->GetData();            

            // 
            auto objManager = Master::GetGameObjectManagerNow();
            auto powerSpotList = objManager->SearchGameObjectsTag(GameObjectTag::GOT_PowerSpot);
            if (0 < powerSpotList.Count())
            {
                auto powerSpot = static_cast<PowerSpot *>(powerSpotList[0]);

                // パワースポットに触れる
                powerSpot->PowerCharge(myObj, data);
            }
        }
    }

    return 0;
}

int OtherPlayerController::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    return 0;
}

int OtherPlayerController::LateUpdate()
{
    return 0;
}

int OtherPlayerController::Draw()
{
    return 0;
}
