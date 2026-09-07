#pragma once
#include "DotWeenData.h"

#include "StateDotWeenBase.h"

/*----------------*/
/*【共通ステート】*/
/*----------------*/
class DotWeenProcess
{
protected:
};

/*--------------------------*/
/*【バウンドアウトステート】*/
/*--------------------------*/
class StateOutBounce : public IStateDotWeen, public DotWeenProcess
{
private:
    /*更新*/
    void Update(DOT_WEEN_DATA dotWeenData) override;
};