#pragma once

#include "Pose2D.h"

enum Animation2DTag : unsigned short
{
    Animation2DTag_Default = 0,
    
    Animation2DTag_Stop_Left,
    Animation2DTag_Stop_Right,
    Animation2DTag_Stop_Up,
    Animation2DTag_Stop_Down,
    Animation2DTag_Walk_Left,
    Animation2DTag_Walk_Right,
    Animation2DTag_Walk_Up,
    Animation2DTag_Walk_Down,
    
    Animation2DTag_Battle,

    Animation2DTag_Max,
};

// Animation2DA‚Ü‚½‚Ì–¼‚ðPoseList
class Animation2D
{
private:
    Pose2D *mpFirstPose;
    Animation2DTag mnAnimation2DTag;
    unsigned char mcCurrentPose;
    unsigned char mcPoseMax;
    unsigned char mcChangeInterval;
    unsigned char mcChangeCount;
    bool mbLoopFlag;
    bool mbEndTimingNow;

    Animation2D *mpPrev;
    Animation2D *mpNext;

public:
    Animation2D();
    Animation2D(Animation2DTag tag, unsigned char poseChangeInterval, bool loopFlag);
    ~Animation2D();

    int UpdatePoseChangeCount();

    int Initialize();
    int Finalize();
    int Update();
    int Draw();

    int Add(Pose2D *target);
    int DeleteTarget(Pose2D *target);
    int DeleteAll();
    int ConnectTarget(Pose2D *prev, Pose2D *target, Pose2D *next);
    int IsolateTarget(Pose2D *target);
    Pose2D *SearchPose2D(unsigned char index);
    Pose2D *GetCurrentPose2D() { return this->SearchPose2D(this->mcCurrentPose); }

    Animation2DTag GetTag() const { return this->mnAnimation2DTag; }
    bool GetLoopFlag() const { return this->mbLoopFlag; }
    bool CheckEndTimingNow() const { return this->mbEndTimingNow; }
    void SetPoseChangeInterval(char interval) { this->mcChangeInterval = interval; }
    void SetPoseChangeCount(char count) { this->mcChangeCount = count; }
    void SetLoopFlag(bool flag) { this->mbLoopFlag = flag; }

    void SetPrev(Animation2D *prev) { this->mpPrev = prev; }
    void SetNext(Animation2D *next) { this->mpNext = next; }
    Animation2D *GetPrev() { return this->mpPrev; }
    Animation2D *GetNext() { return this->mpNext; }
};