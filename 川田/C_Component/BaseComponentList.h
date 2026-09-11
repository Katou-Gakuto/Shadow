#pragma once

#include "BaseComponent.h"

#include "../Y_Tool/VoidList.h"
#include <vector>

class BaseCollision;
class BaseComponentList
{
private:
    BaseComponent *mpFirstComponent;

    int DeleteAll();

    int ConnectTarget(BaseComponent *prev, BaseComponent *target, BaseComponent *next);
    int IsolateTarget(BaseComponent *target);

    int Sort();
    int Swap(BaseComponent *N1, BaseComponent *N2);

public:
    BaseComponentList();
    ~BaseComponentList();

    virtual int Initialize();
    virtual int Finalize();
    virtual int EarlyUpdate();
    virtual int Update();
    virtual int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision);
    virtual int LateUpdate();
    virtual int Draw();

    int Add(BaseComponent *target);
    int DeleteToFlag();

    std::vector<BaseComponent *> SearchComponent(ComponentTagAndOrder ctao) const;
};