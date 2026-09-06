#pragma once
#include <iostream>
#include <map>
#include <type_traits>
#include <vector>

#include "StateData.h"

#include "StateBase.h"

enum class CAMERA_MODE;
enum class DOT_WEEN_TYPE;

struct DOT_WEEN_DATA;

class CameraManager;

/*----------*/
/*【継承用有限状態マシン】*/
/*----------*/
template<typename state, typename subscript, typename stateData, typename = typename std::enable_if<std::is_base_of<StateBase<subscript, stateData>, state>::value>::type>
class FSMBase
{
protected:
	// 登録した状態リスト
	std::map<subscript, state*> mmStateMap;
	// 現在実行中ステート
	subscript mCurrentState;
	// 次の実行ステート
	subscript mNextState;
	// 前の実行ステート
	subscript mPreState;

public:
	FSMBase()
	: mCurrentState((subscript) - 1)
	, mNextState((subscript)0)
	, mPreState((subscript) - 1)
	{
		mmStateMap.clear();
	}
	
	~FSMBase()
	{
		//mapコンテナの解放
		for (const auto& pair : mmStateMap)
			delete (pair.second);//登録されたStateのインスタンスを削除する

		mmStateMap.clear();
	}

	// TODO: 所持しているオブジェクトでシーン変更後この関数を呼び出すようにする
    /// <summary>ステート初期化</summary>
    virtual void StateInitilize(stateData* initilizeStateData)
	{
		for (auto& initilizeState : mmStateMap)
		{
			initilizeState.second->Initilize(initilizeStateData);
		}
	}

	/*ステート登録(ステートナンバー変更される)*/
	inline void RegisterState(const subscript id, state* State)
	{
		State->SetStatenumber(id);
		mmStateMap[id] = State;
	}

	/*ステート登録*/
	virtual void RegisterState(state* State)
	{
		mmStateMap[State->GetStateNumber()] = State;
	}

	/*実行中状態をセットする*/
	virtual void SetCurrentState(const subscript id, stateData* StateData, stateData* preStateData = nullptr)
	{
		mPreState = mCurrentState;
		mCurrentState = id;
		if ((subscript)-1 != mPreState)
		{
			if (preStateData == nullptr)
			{
				preStateData = StateData;
			}
			mmStateMap[mPreState]->OnExit(preStateData, mCurrentState);
		}
		mmStateMap[mCurrentState]->OnEnter(StateData, mPreState);
	}

	/*ステート変更確認*/
	void CheckChangeState(stateData* checkStateData)
	{
		// 変更確認
		subscript changeState = mmStateMap[mCurrentState]->GetNextState(checkStateData);
		if (changeState != mCurrentState)
		{
			mPreState = mCurrentState;
			mCurrentState = changeState;

			// 変更処理	
			mmStateMap[mPreState]->OnExit(checkStateData, mCurrentState);
			mmStateMap[mCurrentState]->OnEnter(checkStateData, mPreState);
		}
	}

	/// <summary>現在の実行状態取得</summary>
	inline subscript GetCurrentState() const { return mCurrentState; }
	/// <summary>前の実行状態取得</summary>
	inline subscript GetPreState() const { return mPreState; }
	/// <summary>次の実行状態設定</summary>
	inline void SetNextState(subscript nextState) { mNextState = nextState; }
};