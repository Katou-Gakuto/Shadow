#pragma once
#include <map>
#include <string>
#include <vector>

#include "Master.h"

#include "EndManager.h"
#include "TemplateType_Equal.h"

#ifdef _DEBUG
#include "DebugLogs/DebugLog.h"
#endif

enum class HANDLE_FLAG
{
    NONE = 0,
    ZERO_LOOK,
    ZERO_EXCEPT_LOOK,
};

template<typename QUOTE_SOURCE, typename HANDLE_TYPE/* = int*/, typename = typename std::enable_if<std::is_convertible<HANDLE_TYPE, int>::value && TemplateType_Equal<HANDLE_TYPE>::value>::type>
class HandleContainer
{
private:
    // ハンドル
    std::map<QUOTE_SOURCE, std::vector<HANDLE_TYPE>> mmHandles;

    // カウント
    std::map<int, int> mmHandleCounts;

    // 次のハンドル追加ファイル
    QUOTE_SOURCE msRegisterFileName;

    // 次のハンドル追加ファイルのイテレーター
    typename std::map<QUOTE_SOURCE, std::vector<HANDLE_TYPE>>::iterator mitRegisterFileIterator;

    // ハンドルフラグ
    HANDLE_FLAG meHandleFlag;

public:
    HandleContainer()
    : msRegisterFileName()
    , meHandleFlag(HANDLE_FLAG::NONE)
    {
        mmHandles.clear();
        mmHandleCounts.clear();
        mitRegisterFileIterator = mmHandles.end();
    }

    ~HandleContainer()
    {
        mmHandles.clear();
        mmHandleCounts.clear();
    }

    /*--------*/
    /*【取得】*/
    /*--------*/
    /// <summary>ハンドルファイル名取得</summary>
    QUOTE_SOURCE GetHandleQuoteSource(HANDLE_TYPE handle)
    {
        for (std::pair<QUOTE_SOURCE, std::vector<HANDLE_TYPE>> chaeckHandle : mmHandles)
        {
            for (int i = 0; i < chaeckHandle.second.size(); i++)
            {
                if (chaeckHandle.second[i] == handle)
                {
                    return chaeckHandle.first;
                }
            }
        }

        QUOTE_SOURCE null;
        return null;
    }

    /// <summary>ハンドル取得</summary>
    std::vector<HANDLE_TYPE> GetHandles(QUOTE_SOURCE fileName) { return mmHandles[fileName]; }

    /// <summary>ハンドルマップ取得</summary>
    std::map<QUOTE_SOURCE, std::vector<HANDLE_TYPE>> GetHandleMap() { return mmHandles; }

    /// <summary>ハンドルマップポインタ取得</summary>
    std::map<QUOTE_SOURCE, std::vector<HANDLE_TYPE>>* GetHandleMapPointer() { return &mmHandles; }

    /// <summary>ハンドルカウント取得</summary>
    int GetHandleCount(HANDLE_TYPE handle) { return mmHandleCounts[static_cast<int>(handle)]; }
    
    /// <summary>ハンドルカウントマップ取得</summary>
    std::map<int, int> GetHandleCountMap() const { return mmHandleCounts; }

    /*--------*/
    /*【設定】*/
    /*--------*/
    /// <summary>ハンドルフラグ設定</summary>
    inline void SetHandleFlag(HANDLE_FLAG handleFlag) { meHandleFlag = handleFlag; }

    /*--------*/
    /*【追加】*/
    /*--------*/
    /// <summary>同名のファイルがあるかを確認する</summary>
    bool CheckFileName(QUOTE_SOURCE fileName)
    {
        msRegisterFileName = fileName;
        mitRegisterFileIterator = mmHandles.find(fileName);

        return mitRegisterFileIterator != mmHandles.end();
    }

    /// <summary>ハンドルを登録する</summary>
    HANDLE_TYPE RegisterHandle(HANDLE_TYPE handle, bool countFlag = true)
    {
        // ハンドルが-1なら実行を終了させる
        if (static_cast<int>(handle) == (-1))
        {
            Master::mpEndManager->SetEndFlag(true, END_FLAG_NUMBER::HANDLE_FLAG);
            return handle;
        }

        // 設定されたファイル名が使われていないなら新しく設定する
        if (mitRegisterFileIterator == mmHandles.end())
        {
            std::vector<HANDLE_TYPE> enptyHandleList;
            enptyHandleList.clear();
            mmHandles[msRegisterFileName] = enptyHandleList;
            mitRegisterFileIterator = mmHandles.find(msRegisterFileName);
            
            mmHandleCounts[static_cast<int>(handle)] = 0;
        }
        switch (meHandleFlag)
        {
        case HANDLE_FLAG::ZERO_LOOK:
            // 返すハンドルを設定する
            if (mitRegisterFileIterator->second.size() <= 0)
            {
                mitRegisterFileIterator->second.push_back(handle);
            }
            else
            {
                handle = mitRegisterFileIterator->second[0];
            }
            break;

        case HANDLE_FLAG::ZERO_EXCEPT_LOOK:
        default:
            // ハンドルを追加する
            mitRegisterFileIterator->second.push_back(handle);
        break;
        }

        // カウントフラグが「true」ならカウントを増やす
        if (countFlag)
        {
            mmHandleCounts[static_cast<int>(mitRegisterFileIterator->second[0])] += 1;
        }

        return handle;
    }

    /// <summary>ハンドルを登録する</summary>
    HANDLE_TYPE RegisterHandle(HANDLE_TYPE handle, QUOTE_SOURCE fileName, bool countFlag = true)
    {
        msRegisterFileName = fileName;
        mitRegisterFileIterator = mmHandles.find(fileName);

        return RegisterHandle(handle, countFlag);
    }

    /*--------*/
    /*【削除】*/
    /*--------*/
    /// <summary>ハンドル削除</summary>
    std::vector<HANDLE_TYPE> DeleteHandle(HANDLE_TYPE handle, bool countFlag = true)
    {
        if (static_cast<int>(handle) == -1)
        {
            return {};
        }

        for (auto& myHandle : mmHandles)
        {
            for (int i = 0; i < myHandle.second.size(); i++)
            {
                switch (meHandleFlag)
                {
                case HANDLE_FLAG::ZERO_LOOK:
                    // 0以外なら何もしない
                    if (i != 0)
                    {
                        i = myHandle.second.size();
                        break;
                    }
                    
                    // 指定のハンドルと比較する
                    if (myHandle.second[i] == handle)
                    { 
                        // カウント減少
                        int countHandle = static_cast<int>(mmHandles[myHandle.first][0]);
                        if (countFlag && (mmHandleCounts.find(countHandle) != mmHandleCounts.end()))
                        {
                            mmHandleCounts[countHandle] -= 1;
                        }

                        // 削除ハンドル
                        std::vector<HANDLE_TYPE> deleteHandles;
                        deleteHandles.clear();

                        if (mmHandleCounts[countHandle] <= 0)
                        {
                            // 削除予定のハンドルを全取得
                            deleteHandles = mmHandles[myHandle.first];

                            // カウントが0以下なためハンドルとカウント削除
                            mmHandles.erase(myHandle.first);
                            mmHandleCounts.erase(countHandle);
                        }

                        return deleteHandles;
                    }

                    break;

                case HANDLE_FLAG::ZERO_EXCEPT_LOOK:
                    // 0なら何もしない
                    if (i == 0)
                    {
                        break;
                    }

                    // 指定のハンドルと比較する
                    if (myHandle.second[i] == handle)
                    {
                        // カウント減少
                        int countHandle = static_cast<int>(mmHandles[myHandle.first][0]);
                        if (countFlag && (mmHandleCounts.find(countHandle) != mmHandleCounts.end()))
                        {
                            mmHandleCounts[countHandle] -= 1;
                        }

                        // 削除ハンドル
                        std::vector<HANDLE_TYPE> deleteHandles;
                        deleteHandles.clear();

                        if (mmHandleCounts[countHandle] <= 0)
                        {
                            // 削除予定のハンドルを全取得
                            deleteHandles = mmHandles[myHandle.first];

                            // カウントが0以下なためハンドルとカウント削除
                            mmHandles.erase(myHandle.first);
                            mmHandleCounts.erase(countHandle);
                        }
                        else
                        {
                            // 削除予定のハンドルを保存
                            deleteHandles.push_back(mmHandles[myHandle.first][i]);

                            // ハンドル削除					
                            mmHandles[myHandle.first].erase(mmHandles[myHandle.first].begin() + i);
                        }

                        return deleteHandles;
                    }
                    break;

                default:
                    // 指定のハンドルと比較する
                    if (myHandle.second[i] == handle)
                    {
                        // カウント減少
                        int countHandle = static_cast<int>(mmHandles[myHandle.first][0]);
                        if (countFlag && (mmHandleCounts.find(countHandle) != mmHandleCounts.end()))
                        {
                            mmHandleCounts[countHandle] -= 1;
                        }

                        // 削除ハンドル
                        std::vector<HANDLE_TYPE> deleteHandles;
                        deleteHandles.clear();

                        if (mmHandleCounts[countHandle] <= 0)
                        {
                            // 削除予定のハンドルを全取得
                            deleteHandles = mmHandles[myHandle.first];

                            // カウントが0以下なためハンドルとカウント削除
                            mmHandles.erase(myHandle.first);
                            mmHandleCounts.erase(countHandle);
                        }
                        else
                        {
                            // 削除予定のハンドルを保存
                            deleteHandles.push_back(mmHandles[myHandle.first][i]);

                            // ハンドル削除					
                            mmHandles[myHandle.first].erase(mmHandles[myHandle.first].begin() + i);

                            // カウントが参照しているハンドルなら入れ替える
                            if (i == 0)
                            {
                                mmHandleCounts[static_cast<int>(mmHandles[myHandle.first][0])] = mmHandleCounts[countHandle];
                                mmHandleCounts.erase(countHandle);
                            }
                        }

                        // 削除したハンドルを返す
                        return deleteHandles;
                    }
                    break;
                }
            }
        }

        
    Master::mpEndManager->SetEndFlag(true, END_FLAG_NUMBER::HANDLE_FLAG);

#ifdef _DEBUG
    DEBUG::SaveText("\nハンドル未発見 : " + std::to_string(static_cast<int>(handle)) + " <= ", DEBUG::DEBUG_MAP_TYPE::DEBUG_UNDISCOVERED);
#endif

        return {};
    }

    /*--------*/
    /*【交換】*/
    /*--------*/
    /// <summary>ハンドルの値を交換する</summary>
    bool SwapHandle(HANDLE_TYPE srcHandle, HANDLE_TYPE destHandle)
    {
        for (auto& myHandle : mmHandles)
        {
            for (int i = 0; i < myHandle.second.size(); i++)
            {
                switch (meHandleFlag)
                {
                case HANDLE_FLAG::ZERO_LOOK:
                    // 0以外なら何もしない
                    if (i != 0)
                    {
                        i = myHandle.second.size();
                        continue;
                    }
                    break;
                    
                case HANDLE_FLAG::ZERO_EXCEPT_LOOK:
                    // 0なら何もしない
                    if (i == 0)
                    {
                        continue;
                    }
                    break;

                default:
                break;
                }

                if (myHandle.second[i] == srcHandle)
                {
                    if (i == 0)
                    {
                        mmHandleCounts[static_cast<int>(destHandle)] = mmHandleCounts[static_cast<int>(srcHandle)];
                        mmHandleCounts.erase(static_cast<int>(srcHandle));
                    }
                    mmHandles[myHandle.first][i] = destHandle;
                    return true;
                }
            }
        }
        return false;
    }
};