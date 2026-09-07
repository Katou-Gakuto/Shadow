#include <vector>

#include "DotWeenData.h"

#include "Master.h"

#include "FSM.h"
#include "FSMDotWeen.h"
#include "StateDotWeen.h"
#include "StateDotWeenBase.h"
#include "TimeManager.h"


/*-------------------------*/
/*【DotWeen有限状態マシン】*/
/*-------------------------*/
FSMDotWeen::FSMDotWeen()
: FSMBase()
{
}

// 初期化
void FSMDotWeen::Initilize()
{
	RegisterState(DOT_WEEN_TYPE::OUT_BOUNCE, new StateOutBounce());
}

// 更新
void FSMDotWeen::Update(std::vector<DOT_WEEN_DATA>& dotWeenData)
{
	for (int i  = 0; i < dotWeenData.size(); i++)
	{
		dotWeenData[i].DotWeenElapsedTime += Master::mpTimeManager->GetDeltaTime();
		if (dotWeenData[i].DotWeenElapsedTime >= dotWeenData[i].DotWeenTotalTime)
		{
			*dotWeenData[i].ChangeData = dotWeenData[i].EndData;
			dotWeenData.erase(dotWeenData.begin() + i);
			i -= 1;
			continue;
		}

		mmStateMap[dotWeenData[i].DotWeenType]->Update(dotWeenData[i]);

	}
}