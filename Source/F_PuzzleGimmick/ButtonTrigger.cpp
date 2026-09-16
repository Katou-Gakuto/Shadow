#include "ButtonTrigger.h"

#include "../C_Component/ButtonObjectController.h"

// 
ButtonTrigger::ButtonTrigger() :
    mlFlagButtonBox(),
    BaseGimmickTrigger()
{
}

// 
ButtonTrigger::~ButtonTrigger()
{
    // 
    for (auto &currentButton : this->mlFlagButtonBox)
    {
        // 
        currentButton = nullptr;
    }

    // 
    this->mlFlagButtonBox.clear();
}

// 
bool ButtonTrigger::GetSignal()
{
    // 
    for (auto currentButton : this->mlFlagButtonBox)
    {
        // 
        if (!currentButton->CheckOnPlayer())
        {
            // 
            return false;
        }
    }

    // 
    return true;
}

// 
bool ButtonTrigger::AddButton(ButtonObjectController *button)
{
    // 
    this->mlFlagButtonBox.push_back(button);

    // 
    return true;
}
