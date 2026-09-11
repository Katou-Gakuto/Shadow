#include "PowerSpotController.h"

#include "../A_GameObject/PowerSpot.h"
#include "../S_Collision/BaseCollision.h"
#include "../W_Network/TCPData.h"
#include "../W_Network/NetworkHome.h"
#include "../Z_Except/Master.h"

PowerSpot *PowerSpotController::GetPowerSpot() const
{
    return static_cast<PowerSpot *>(this->GetMyObject());
}

PowerSpotController::PowerSpotController(GameObject *myObject) :
    repopACK(0),
    BaseComponent(myObject, ComponentTagAndOrder::CTAT_PowerSpotController)
{

}

PowerSpotController::~PowerSpotController()
{

}

int PowerSpotController::Create()
{
    return 0;
}

int PowerSpotController::Initialize()
{
    // 自身のオブジェクトを取得する
    auto myObj = GetPowerSpot();

    unsigned long myNumber = Master::mpNetworkHome->GetMyNumber();
    if (myNumber == 0)
    {
        myObj->SetState(0);
    }
    else
    {
        if (myNumber == 1)
        {
            myObj->SetState(0);
        }
        else
        {
            myObj->SetState(1);
        }
    }
    return 0;
}

int PowerSpotController::Finalize()
{
    return 0;
}

int PowerSpotController::EarlyUpdate()
{
    return 0;
}

int PowerSpotController::Update()
{
    // このPCのプレイヤー識別番号を取得する
    unsigned long myNumber = Master::mpNetworkHome->GetMyNumber();

    // 自身のオブジェクトを取得する
    auto myObj = GetPowerSpot();

    // オフライン状態か、ホストか、ゲストかで処理を変える
    if (myNumber == 0)
    {
        // 現在の状態によって処理を変える
        switch (myObj->GetState())
        {
        case 0:
            // 再出現の座標を取得
            myObj->SetRepopPos();

            // 再出現までの準備が完了したので
            myObj->SetState(2);

            // 再出現までの更新を行う(ここでstateが3になる可能性も一応)
            myObj->IntervalUpdate();
            break;
        case 1:
            // 再出現位置の情報を取得
            break;
        case 2:
            // 再出現までの準備が完了した状態でのステルス更新処理
            // 再出現までの更新を行う(ここでstateが3になる)
            myObj->IntervalUpdate();
            break;
        case 3:
            // 再出現後のチャージ処理を行う
            myObj->ChargeUpdate(myNumber, nullptr);
            break;
        case 4:
            // repopPosの計算を行うため、stateを0に変える
            myObj->SetState(0);

            // 再出現までの更新を行う(ここでstateが3になる可能性も一応)
            myObj->IntervalUpdate();
            break;
        default:
            // エラーキャッチのための意味不明コード
            myNumber = myNumber;
            break;
        }
    }
    else
    {
        // 現在の状態によって処理を変える
        switch (myObj->GetState())
        {
        case 0:
            // 再出現座標を送っていない場合と送っている場合で処理を変える
            if (myObj->GetRepopACK() == 0)
            {
                // 再出現の座標を取得
                myObj->SetRepopPos();

                // 再出現位置のデータを送信する
                myObj->SendRepop();
            }
            else
            {
                // 送信したデータが再送信リストから消えていることを確認する
                if (Master::mpNetworkHome->CheckReliablePacketNull(myObj->GetRepopACK()))
                {
                    // 再出現までの準備が完了したので
                    myObj->SetState(2);

                    // 保存しているACKを0に戻しておく
                    myObj->ResetRepopACK();
                }
            }

            // 再出現までの更新を行う(ここでstateが3になる可能性も一応)
            myObj->IntervalUpdate();
            break;
        case 1:
        {
            // 再出現位置の情報を取得
            auto repopDataList = Master::mpNetworkHome->PullGameDataUDP(UDPDataType::PDT_ITR_PowerSpotRepop);
            AvailableDataUDP *data = Master::mpNetworkHome->GetLastGameData(repopDataList);
            if (data != nullptr)
            {
                // 再出現位置の設定を行う
                UDPData_PowerSpotRepop *repop = (UDPData_PowerSpotRepop *)data->GetData();
                myObj->SetRepopPos(repop);

                // 再出現までの準備が完了したので
                myObj->SetState(2);
            }

            // 再出現までの更新を行う(ここでstateが3になる可能性も一応)
            myObj->IntervalUpdate();
        }
        break;
        case 2:
            // 再出現までの準備が完了した状態でのステルス更新処理
            // 再出現までの更新を行う(ここでstateが3になる)
            myObj->IntervalUpdate();
            break;
        case 3:
        {
            // 再出現をし、チャージ処理を行う
            // 
            // この状態で通信相手が触れるかどうかの処理はOtherPlayer側で行う。
            auto list = Master::mpNetworkHome->PullGameDataUDP(UDPDataType::PDT_ITR_PowerSpotResult);
            auto data = Master::mpNetworkHome->GetLastGameData(list);
            UDPData_PowerSpotResult *resultData = nullptr;
            if (data != nullptr)
            {
                resultData = (UDPData_PowerSpotResult *)data;
            }
            // 通信相手側が送信してきたデータがnullptrの可能性もある
            // このPCが先に結果を出す可能性もある(その場合は内部で結果を送信する)
            // そして結果が定まった場合は、内部でstateの値を変更する(こっちで結果を出した場合は4に、向こうが結果を出した場合は1に)
            myObj->ChargeUpdate(myNumber, resultData);
        }
            break;
        case 4:
            // こっちが先に結果を出し、向こうのPCがそれを受け取ったかを確認する
            if (Master::mpNetworkHome->CheckReliablePacketNull(myObj->GetResultACK()))
            {
                // 向こうのPCが受け取ったことを確認したので、stateを0に変える
                myObj->SetState(0);
            }
            break;
        default :
            // エラーキャッチのための意味不明コード
            myNumber = myNumber;
            break;
        }
    }

    // 関数が正常終了したことを返す
    return 0;
}

int PowerSpotController::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    // パワースポットによる攻撃に当たった時の計算をする
    if (myCollision->GetCollisionTag() == CollisionTag::CollisionTag_Attack)
    {
        // 自身のオブジェクトを取得する
        auto myObj = this->GetPowerSpot();
        if (myObj != nullptr &&
            myObj != hitCollision->GetMyObject())
        {
            // 相手のHPを一つ減らす
            myObj->Hit(hitCollision);

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

    // 関数が正常終了したことを返す
    return 0;
}

int PowerSpotController::LateUpdate()
{
    // 自身のオブジェクトを取得する
    auto powerSpot = GetPowerSpot();

    // パワースポットが出現中なら、次のフレームでの処理を行うためにフラグを整える
    if (powerSpot->CheckStealthFlag() == false)
    {
        powerSpot->ClearFlag();
    }

    return 0;
}

int PowerSpotController::Draw()
{
    return 0;
}