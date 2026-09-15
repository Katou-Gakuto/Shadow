#include "ButtonGimmickChecker.h"

#include "../C_Component/ButtonObjectController.h"

// 
ButtonSumilationChecker::ButtonSumilationChecker() :
    mlFlagButtonBox(),
    BaseGimmickChecker()
{
}

// 
ButtonSumilationChecker::~ButtonSumilationChecker()
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
bool ButtonSumilationChecker::GetSignalSumilate()
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
bool ButtonSumilationChecker::GetSignalExecute()
{
    // 
    for (auto currentButton : this->mlFlagButtonBox)
    {
        // 
        if (!currentButton->CheckButtonSignal())
        {
            // 
            return false;
        }
    }

    // 
    return true;
}

// 
bool ButtonSumilationChecker::AddButton(ButtonObjectController *button)
{
    // 
    this->mlFlagButtonBox.push_back(button);

    // 
    return true;
}
