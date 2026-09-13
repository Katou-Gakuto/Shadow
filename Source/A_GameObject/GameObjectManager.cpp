#include "GameObjectManager.h"
#include "GameObject.h"
#include "BaseCamera.h"

#include "FPSCamera.h"
#include "LineMap.h"
#include "Obstacle.h"
#include "Fence.h"

#include "TestEmitter.h"
#include "Player.h"
#include "Enemy.h"

#include "../S_Collision/BaseCollisionList.h"
#include "../H_PlayerSide/PlayerSideComand2D.h"
#include "../Z_Except/Master.h"

// GameObjectManager::Draw()で関数の始めに呼び出す、カメラのアップデートから行列を更新する関数
int GameObjectManager::UpdateMatrix()
{
    // 新しいカメラに設定した時点で行列の更新も行っているため、処理を飛ばす
    if (this->mbUpdateMatrixFlag == true)
    {
        // 行列の最新のものである事が確定しているのでフラグをfalseに設定する
        this->mbUpdateMatrixFlag = false;

        // 行列が最新であることを返す
        return 0;
    }
    // 自身に設定されているカメラが無効なものなら処理を行わない
    if (this->mnNowCameraNumber == UseCameraNumber::UCN_Max)
    {
        return -1;
    }

    // 返り値
    int temp = -1;

    // 目的のカメラがnullptrでなければ処理を行う
    if (this->mlCameraBox[(unsigned char)(this->mnNowCameraNumber)] != nullptr)
    {
        // カメラへキャストするのが成功していたら処理を行う
        BaseCamera *nextCamera = dynamic_cast<BaseCamera *>(this->mlCameraBox[(unsigned char)(this->mnNowCameraNumber)]);
        if (nextCamera != nullptr)
        {
            // 二つの行列を更新する
            int result = nextCamera->UpdateMatrix(false);
            if (result == 0)
            {
                // 行列が最新であることを記憶しておく
                temp = 0;
            }
        }
    }

    // 結果を返す
    return temp;
}

GameObjectManager::GameObjectManager() :
    mpFirstGameObject(nullptr),
    mbCreateFlag(),
    mnNowCameraNumber(UseCameraNumber::UCN_Max),
    mlCameraBox(),
    mbUpdateMatrixFlag(true)
{

}

GameObjectManager::~GameObjectManager()
{
    // カメラもリストの中にいるのでこの関数のみで大丈夫です
    this->DeleteAll();
}

int GameObjectManager::SetNowCamera(UseCameraNumber nextUseCamera)
{
    // 既に設定しているカメラは再度設定する必要がないので
    if (this->mnNowCameraNumber == nextUseCamera)
    {
        return 0;
    }
    // 引数で無効な値を渡された場合は処理を行わない
    if (nextUseCamera == UseCameraNumber::UCN_Max)
    {
        return -1;
    }

    // 返り値(初期化の値は失敗を意味する値)
    int temp = -1;

    // 目的のカメラがnullptrでなければ処理を行う
    if (this->mlCameraBox[(unsigned char)(nextUseCamera)] != nullptr)
    {
        // カメラへキャストするのが成功していたら処理を行う
        BaseCamera *nextCamera = dynamic_cast<BaseCamera *>(this->mlCameraBox[(unsigned char)(nextUseCamera)]);
        if (nextCamera != nullptr)
        {
            // 二つの行列を更新する
            int result = nextCamera->UpdateMatrix(true);
            if (result == 0)
            {
                // 新しいカメラに設定する
                this->mnNowCameraNumber = nextUseCamera;

                // 新しいカメラに設定したことを覚えておく
                this->mbUpdateMatrixFlag = true;

                // 成功したことを記憶させておく
                temp = 0;
            }
        }
    }

    // 結果を返す
    return temp;
}

BaseCamera *GameObjectManager::GetCameraNum(UseCameraNumber cameraNum)
{
    // そんなカメラはありません
    if (cameraNum == UseCameraNumber::UCN_Max)
    {
        return nullptr;
    }

    // カメラへキャストするのが成功していたら処理を行う
    return this->mlCameraBox[(unsigned char)(cameraNum)];
}

// 現在使用しているカメラを取得する
BaseCamera *GameObjectManager::GetCameraNow()
{
    return this->mlCameraBox[(unsigned char)(this->mnNowCameraNumber)];
}

int GameObjectManager::Create()
{
    if (this->mbCreateFlag.GetFlag())
    {
        this->mbCreateFlag.CreateComplate();
    }

    return 0;
}

int GameObjectManager::Initialize()
{
    this->Create();
    this->SetNowCamera(UseCameraNumber::UCN_MainCamera);
    GameObject *current = this->mpFirstGameObject;
    while (current != nullptr)
    {
        if (current->GetDeleteFlag() == false)
        {
            current->Initialize();
        }
        current = current->GetNext();
    }

    return 0;
}

int GameObjectManager::Finalize()
{
    GameObject *current = this->mpFirstGameObject;
    while (current != nullptr)
    {
        if (current->GetDeleteFlag() == false)
        {
            current->Finalize();
        }
        current = current->GetNext();
    }

    return 0;
}

int GameObjectManager::ObjectCollisionActivate()
{
    GameObject *current = this->mpFirstGameObject;
    while (current != nullptr)
    {
        if (current->GetDeleteFlag() == false)
        {
            current->CollisionActivate();
        }
        current = current->GetNext();
    }
    return 0;
}

int GameObjectManager::ObjectCollisionDeactivate()
{
    GameObject *current = this->mpFirstGameObject;
    while (current != nullptr)
    {
        if (current->GetDeleteFlag() == false)
        {
            current->CollisionDeactivate();
        }
        current = current->GetNext();
    }
    return 0;
}

// 移動ベクトルを0ベクトルに設定する
int GameObjectManager::ResetMoveVec()
{
    GameObject *current = this->mpFirstGameObject;
    while (current != nullptr)
    {
        if (current->GetDeleteFlag() == false)
        {
            current->ResetMoveVec();
        }
        current = current->GetNext();
    }

    return 0;
}

// 通常更新の前にしなければいけない何か
int GameObjectManager::EarlyUpdate()
{
    GameObject *current = this->mpFirstGameObject;
    while (current != nullptr)
    {
        if (current->GetDeleteFlag() == false)
        {
            current->EarlyUpdate();
        }
        current = current->GetNext();
    }

    return 0;
}

// 通常更新
int GameObjectManager::Update()
{
    GameObject *current = this->mpFirstGameObject;
    while (current != nullptr)
    {
        if (current->GetDeleteFlag() == false)
        {
            current->Update();
        }
        current = current->GetNext();
    }

    return 0;
}

// 当たり判定計算後のオブジェクトの座標の確定
int GameObjectManager::SetPositionToMoveVec()
{
    GameObject *current = this->mpFirstGameObject;
    while (current != nullptr)
    {
        if (current->GetDeleteFlag() == false)
        {
            current->SetPositionToMoveVec();
        }
        current = current->GetNext();
    }

    return 0;
}

// 当たり判定計算後の当たり判定の座標の確定
int GameObjectManager::SetCollisionPosToCollisionMoveVec()
{
    GameObject *current = this->mpFirstGameObject;
    while (current != nullptr)
    {
        if (current->GetDeleteFlag() == false)
        {
            current->SetCollisionPosToCollisionMoveVec();
        }
        current = current->GetNext();
    }

    return 0;
}

// 当たり判定計算後の更新
int GameObjectManager::LateUpdate()
{
    GameObject *current = this->mpFirstGameObject;
    while (current != nullptr)
    {
        if (current->GetDeleteFlag() == false)
        {
            current->LateUpdate();
        }
        current = current->GetNext();
    }

    return 0;
}

// 削除フラグの立っている当たり判定などを削除する
int GameObjectManager::DeleteToFlagMember()
{
    GameObject *current = this->mpFirstGameObject;
    while (current != nullptr)
    {
        current->DeleteToFlagMember();
        current = current->GetNext();
    }

    return 0;
}

// 削除フラグの立っているオブジェクトを削除する
int GameObjectManager::DeleteToFlag()
{
    if (this->mpFirstGameObject == nullptr)
    {
        return 0;
    }

    GameObject *current = this->mpFirstGameObject;
    GameObject *next = current->GetNext();
    while (true)
    {
        if (current->GetDeleteFlag())
        {
            DeleteTarget(current);
        }
        current = next;
        if (current != nullptr)
        {
            next = current->GetNext();
        }
        else
        {
            break;
        }
    }

    return 0;
}

// モデルの更新
int GameObjectManager::UpdateModel()
{
    GameObject *current = this->mpFirstGameObject;
    while (current != nullptr)
    {
        current->UpdateModel();
        current = current->GetNext();
    }

    return 0;
}

// モデルの描画
int GameObjectManager::Draw()
{
    // カメラが無効なものなら処理を行わない
    if (this->mnNowCameraNumber == UseCameraNumber::UCN_Max)
    {
        return -1;
    }

    // 現在のカメラの更新を行う
    int result = UpdateMatrix();
    
    // 更新に失敗していた場合は関数を終了する
    if (result != 0)
    {
        return result;
    }

    // オブジェクトごとの描画を行う
    GameObject *current = this->mpFirstGameObject;
    while (current != nullptr)
    {
        current->ObjectDraw();
        current = current->GetNext();
    }

    // 成功したことを返す
    return 0;
}

int GameObjectManager::Add(GameObject *target)
{
    if (target == nullptr)
    {
        return -1;
    }

    if (this->mpFirstGameObject == nullptr)
    {
        this->mpFirstGameObject = target;
        return 0;
    }

    GameObject *current = this->mpFirstGameObject;
    while (current->GetNext() != nullptr)
    {
        current = current->GetNext();
    }

    return this->ConnectTarget(current, target, nullptr);
}

int GameObjectManager::AddCamera(BaseCamera *target, UseCameraNumber cameraNum)
{
    this->mlCameraBox[cameraNum] = target;
    return 0;
}

int GameObjectManager::DeleteTarget(GameObject *target)
{
    if (target == nullptr)
    {
        return 0;
    }

    this->IsolateTarget(target);

    target->Finalize();

    delete target;

    return 0;
}

int GameObjectManager::DeleteAll()
{
    if (this->mpFirstGameObject == nullptr)
    {
        return 0;
    }

    GameObject *current = this->mpFirstGameObject;
    GameObject *next = current->GetNext();
    while (true)
    {
        DeleteTarget(current);
        current = next;
        if (current != nullptr)
        {
            next = current->GetNext();
        }
        else
        {
            break;
        }
    }

    return 0;
}

int GameObjectManager::ConnectTarget(GameObject *prev, GameObject *target, GameObject *next)
{
    if (prev != nullptr)
    {
        prev->SetNext(target);
    }
    if (next != nullptr)
    {
        next->SetPrev(target);
    }
    if (next == this->mpFirstGameObject)
    {
        this->mpFirstGameObject = target;
    }
    target->SetPrev(prev);
    target->SetNext(next);
    return 0;
}

int GameObjectManager::IsolateTarget(GameObject *target)
{
    GameObject *prev = target->GetPrev();
    GameObject *next = target->GetNext();

    if (prev != nullptr)
    {
        prev->SetNext(next);
    }
    if (next != nullptr)
    {
        next->SetPrev(prev);
    }
    if (target == this->mpFirstGameObject)
    {
        this->mpFirstGameObject = next;
    }
    target->SetPrev(nullptr);
    target->SetNext(nullptr);
    return 0;
}

MallocData<GameObject *> GameObjectManager::SearchGameObjectsTag(GameObjectTag tag)
{
    // 指定のタグを持ったオブジェクトがどれくらいいるか
    int counter = 0;
    GameObject *current = this->mpFirstGameObject;
    while (current != nullptr)
    {
        if (current->GetObjectTag() == tag)
        {
            counter++;
        }
        current = current->GetNext();
    }

    // ↓で(GameObject **)malloc(sizeof(GameObject *) * counter)をやっています
    MallocData <GameObject *>temp(counter);

    // 指定のタグを持ったオブジェクトを配列に移動させる
    counter = 0;
    current = this->mpFirstGameObject;
    while (current != nullptr)
    {
        if (current->GetObjectTag() == tag)
        {
            temp[counter] = current;
            counter++;
        }
        if (counter == temp.Count())
        {
            break;
        }
        else
        {
            current = current->GetNext();
        }
    }

    // 配列を返す
    return temp;
}