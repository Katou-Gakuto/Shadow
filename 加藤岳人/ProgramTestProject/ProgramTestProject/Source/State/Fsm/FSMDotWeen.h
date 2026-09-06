#pragma once

#include "FSM.h"
#include "StateDotWeenBase.h"

/*-------------------------*/
/*【DotWeen有限状態マシン】*/
/*-------------------------*/
class FSMDotWeen : public FSMBase<IStateDotWeen, DOT_WEEN_TYPE, std::vector<DOT_WEEN_DATA>>
{
public:
	FSMDotWeen();

	/// <summary>初期化</summary>
	void Initilize();

	/// <summary>更新</summary>
	void Update(std::vector<DOT_WEEN_DATA>& dotWeenData);
};
