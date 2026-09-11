#pragma once

#include "Animation2D.h"

class Animation2DList
{
private:
    Animation2D *mpFirstAnimation;
    Animation2DTag mnAnimationOldTag;
    Animation2DTag mnAnimationNowTag;
    Animation2DTag mnReserveTag;
    unsigned short mnAnimationOldIndex;
    unsigned short mnAnimationNowIndex;
    unsigned short mnAnimationReserveIndex;
    bool mbTagMode;
    bool mbReserveLoopFlag;
    bool mbFirstFrame;

public:
    Animation2DList(bool tagMode);
    ~Animation2DList();

    int ChangeAnimationTag(Animation2DTag animTag, bool loopFlag);
    int ChangeAnimationIndex(unsigned short index, bool loopFlag);
    int ChangeAnimationResarve();

    int Initialize();
    int Finalize();
    int Update();
    int Draw();

    int Add(Animation2D *target);
    int DeleteTarget(Animation2D *target);
    int DeleteAll();
    int ConnectTarget(Animation2D *prev, Animation2D *target, Animation2D *next);
    int IsolateTarget(Animation2D *target);
    Animation2D *SearchAnimation2DTag(Animation2DTag tag) const;
    Animation2D *SearchAnimation2DIndex(unsigned short index) const;
    Animation2D *GetNowAnimation2D() const;
    Animation2D *GetOldAnimation2D() const;
    bool GetNowLoopFlag() const;

    void SetReserveAnimationParam(Animation2DTag tag, bool loopFlag) { this->mnReserveTag = tag; this->mbReserveLoopFlag = loopFlag; }

    void SetAnimationOld(Animation2DTag tag) { this->mnAnimationOldTag = tag; }

    Animation2DTag GetAnimationOldTag()const { return this->mnAnimationOldTag; }
    Animation2DTag GetAnimationNowTag()const { return this->mnAnimationNowTag; }
    Animation2DTag GetReserveAnimationTag()const { return this->mnReserveTag; }
    bool GetReserveAnimationLoopFlag()const { return this->mbReserveLoopFlag; }
    bool GetFirstFrame()const { return this->mbFirstFrame; }
};