#pragma once

enum ModelType : unsigned char
{
    ModelType_Model2D,
    ModelType_Model3D,
    ModelType_ModelPartsBox,
    ModelType_ModelSoundLine,
    ModelType_Max,
};

enum ScreenNumber : unsigned char;
class GameObject;
class BaseModel
{
private:
    typedef enum BaseModelBitFlag : unsigned char
    {
        BMBF_DrawFlag = 1 << 0,
        BMBF_1 = 1 << 1,
        BMBF_2 = 1 << 2,
        BMBF_3 = 1 << 3,
        BMBF_4 = 1 << 4,
        BMBF_5 = 1 << 5,
        BMBF_6 = 1 << 6,
        BMBF_7 = 1 << 7,
    } BMBF;

    ModelType mnModelType;
    ScreenNumber mnDrawScreen;
    unsigned char mcBitFlag;
    unsigned long mnModelNumber;

    GameObject *mpUp;

    BaseModel *mpPrev;
    BaseModel *mpNext;

    void SetDrawBit(bool flag);

public:
    BaseModel(GameObject *up, ModelType modelType, ScreenNumber drawScreen);
    virtual ~BaseModel();

    virtual int Initialize();
    virtual int Finalize();
    virtual int Update();
    virtual int Draw();

    int BaseDraw();

    void SetDrawFlag(bool flag);
    void SetModelNumber(unsigned long number);

    ModelType GetModeltype() const;
    bool GetDrawFlag() const;
    unsigned long GetModelNumber()const;
    GameObject *GetUp() const;

    void SetPrevNext(BaseModel *prev, BaseModel *next);
    BaseModel *GetPrev() const;
    BaseModel *GetNext() const;
};