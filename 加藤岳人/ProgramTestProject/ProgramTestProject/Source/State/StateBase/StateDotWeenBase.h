#pragma once
#include <vector>

#include "StateBase.h"

enum class DOT_WEEN_TYPE;

struct DOT_WEEN_DATA;


/*-------------------------*/
/*【DotWeenステートベース】*/
/*-------------------------*/
class IStateDotWeen : public StateBase<DOT_WEEN_TYPE, std::vector<DOT_WEEN_DATA>>
{
public:
	IStateDotWeen()
	: StateBase(std::vector<STATE_CHANGE_CRITERIA_DATA<DOT_WEEN_TYPE, std::vector<DOT_WEEN_DATA>>>{}, DOT_WEEN_TYPE::NONE)
	{
	}
	/// <summary>初期化</summary>
	virtual void Initilize(std::vector<DOT_WEEN_DATA>* nullData = nullptr){}
	/*この状態に入った時の処理*/
	virtual void OnEnter(std::vector<DOT_WEEN_DATA>* voidData, DOT_WEEN_TYPE preState) override
	{
	}
	/*この状態を出る時の処理*/
	virtual void OnExit(std::vector<DOT_WEEN_DATA>* voidData, DOT_WEEN_TYPE nextState) override
	{
	}

	virtual void Update(DOT_WEEN_DATA dotWeenData) = 0;
};