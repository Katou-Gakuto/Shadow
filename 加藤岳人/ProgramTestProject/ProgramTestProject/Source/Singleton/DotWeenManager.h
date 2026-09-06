#pragma once
#include <vector>

#include "DotWeenData.h"
#include "FSMDotWeen.h"

class DotWeenManager
{
private:
    std::vector<DOT_WEEN_DATA> mstDotWeenDatas;

    int mnIdMax;

    static constexpr int DOT_WEEN_ID_MAX = 2'000'000'000;

    // DotWeen処理 有限状態マシン
    FSMDotWeen* mpFsm;

public:
    DotWeenManager();
    ~DotWeenManager();

    /// <summary>初期化</summary>
    void Initilize();
    /// <summary>終了</summary>
    void Finalize();
    /// <summary>更新</summary>
	void Update();

    /// <summary>DotWenn情報設定</summary>
    int SetDotWeen(DOT_WEEN_DATA dotWeenData);
    
    /// <summary>DotWeen情報削除</summary>
    void DeleteDotWeenData(int id);
};