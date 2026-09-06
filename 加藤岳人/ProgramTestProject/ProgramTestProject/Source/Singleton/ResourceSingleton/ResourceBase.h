#pragma once
#include <map>
#include <string>

#include "HandleContainer.h"

#ifdef _DEBUG
#include "Master.h"
#include "EndManager.h"
#endif

template<typename HANDLE_TYPE, typename QUOTE_SOURCE = std::string, typename PLUS_DATA = int/*voidだと引数でエラー起こすから*/, typename = typename std::enable_if<std::is_convertible<HANDLE_TYPE, int>::value && TemplateType_Equal<HANDLE_TYPE>::value>::type>
class ResourceBase
{
#ifdef _DEBUG
private:
    // ハンドル設定確認用変数
    unsigned char mucHandleSetConfirmation = 0;
#endif

protected:
    enum RESOURCE_BASE_HANDLE_FLAG_SETTING_TYPE
    {
        REDUCE_RESOURCE = 0,                // 減らす
        GENERATION_RESOURCE,                // 生成
        DUPLICATION_RESOURCE,               // 複製
        RESOURCE_BASE_HANDLE_FLAG_TYPE_MAX  // 最大
    };

    // ハンドルコンテナ
    HandleContainer<QUOTE_SOURCE, HANDLE_TYPE> mclHandleContainer;

    // 処理別設定ハンドルフラグ
    std::map<int, HANDLE_FLAG> mmSettingHandleFlagByProcess;

public:
    ResourceBase() = default;
    ~ResourceBase() = default;

    /// <summary>終了</summary>
    virtual void Finalize() = 0;

    /// <summary>リソースハンドル取得</summary>
	virtual HANDLE_TYPE GetResourceHandle(QUOTE_SOURCE fileName, PLUS_DATA* plusData = nullptr)
    {
        if (mclHandleContainer.CheckFileName(fileName))
        {
            return mclHandleContainer.RegisterHandle(ResourceDuplication(fileName, plusData));
        }

        return ResourceGeneration(fileName, plusData);
    }

	/// <summary>リソースカウントを減らす</summary>
	virtual void ReduceResourceHandle(HANDLE_TYPE handle)
    {
        SetHandleFlag(RESOURCE_BASE_HANDLE_FLAG_SETTING_TYPE::REDUCE_RESOURCE);

        std::vector<HANDLE_TYPE> deleteHandles = mclHandleContainer.DeleteHandle(handle);

        ResourceDelete(deleteHandles);
    }

protected:
    /// <summary>リソース生成</summary>
    virtual HANDLE_TYPE ResourceGeneration(QUOTE_SOURCE fileName, PLUS_DATA* plusData)
    {
        SetHandleFlag(RESOURCE_BASE_HANDLE_FLAG_SETTING_TYPE::GENERATION_RESOURCE);
        mclHandleContainer.RegisterHandle(CreateResource(fileName, plusData), false);
        
#ifdef _DEBUG
        unsigned char preHandleSetConfirmation = mucHandleSetConfirmation;
        HANDLE_TYPE result = mclHandleContainer.RegisterHandle(ResourceDuplication(fileName, plusData));
        if (preHandleSetConfirmation == mucHandleSetConfirmation)
        {
            Master::mpEndManager->SetEndFlag(true, END_FLAG_NUMBER::RESOURCE_BASE_FLAG);
        }
        return result;
#endif
        return mclHandleContainer.RegisterHandle(ResourceDuplication(fileName, plusData));
        
    }

    /// <summary>リソース本体作成</summary>
    virtual HANDLE_TYPE CreateResource(const QUOTE_SOURCE& fileName, PLUS_DATA* plusData) = 0;
    /// <summary>リソース複製</summary>
    virtual HANDLE_TYPE ResourceDuplication(const QUOTE_SOURCE& fileName, PLUS_DATA* plusData) = 0;
    /// <summary>リソース削除</summary>
    virtual void ResourceDelete(const std::vector<HANDLE_TYPE>& handles) = 0;

    void SetHandleFlag(int settingType)
    {
#ifdef _DEBUG
        mucHandleSetConfirmation += 1;
#endif
        auto settingHandleIt = mmSettingHandleFlagByProcess.find(settingType);
        if (settingHandleIt != mmSettingHandleFlagByProcess.end())
        {
            mclHandleContainer.SetHandleFlag(settingHandleIt->second);
        }
    }
};