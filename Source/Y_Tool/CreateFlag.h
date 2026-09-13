#pragma once

class CreateFlag
{
private:
    bool mbFlag;
public:
    CreateFlag() : mbFlag(true) {}
    ~CreateFlag() {}
    void CreateComplate() { this->mbFlag = false; }
    bool GetFlag() { return this->mbFlag; }
};