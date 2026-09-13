#pragma once

#include "BaseComponent.h"
#include "../Y_Tool/CountDownSystem.h"

class CharacterLife : public BaseComponent
{
private:
    CountDownSystem mnLifeCountDown;

public:
    CharacterLife(GameObject *myObject);
    ~CharacterLife() override;

    int Hit();
    int GetLifeCount() const;
};