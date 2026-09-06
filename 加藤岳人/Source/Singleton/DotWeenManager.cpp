#include "DotWeenEnum.h"
#include "DotWeenData.h"

#include "DotWeenManager.h"
#include "FSMDotWeen.h"

// コンストラクタ
DotWeenManager::DotWeenManager()
: mnIdMax(0)
, mpFsm(nullptr)
{
    mstDotWeenDatas.clear();
}

// デストラクタ
DotWeenManager::~DotWeenManager()
{
    delete mpFsm;
}

// 初期化
void DotWeenManager::Initilize()
{
    mpFsm = new FSMDotWeen();
    mpFsm->Initilize();
}

// 終了
void DotWeenManager::Finalize()
{
    mstDotWeenDatas.clear();
}

// 更新
void DotWeenManager::Update()
{
    // DotWeen情報の更新処理
    mpFsm->Update(mstDotWeenDatas);
}

// DotWenn情報設定
int DotWeenManager::SetDotWeen(DOT_WEEN_DATA dotWeenData)
{
    dotWeenData.Id = mnIdMax;
    mnIdMax += 1;
    if (mnIdMax > DOT_WEEN_ID_MAX)
    {
        mnIdMax = 0;
    }

    mstDotWeenDatas.push_back(dotWeenData);

    return dotWeenData.Id;
}

// DotWeen情報削除
void DotWeenManager::DeleteDotWeenData(int id)
{
    // IDに対応するDotWeen情報を削除
    for (int i = 0; i < mstDotWeenDatas.size(); ++i)
    {
        if (mstDotWeenDatas[i].Id == id)
        {
            mstDotWeenDatas.erase(mstDotWeenDatas.begin() + i);
            return;
        }
    }
}