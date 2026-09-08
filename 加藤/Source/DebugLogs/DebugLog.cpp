#include <windows.h>

#include <dbghelp.h>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#ifdef _DEBUG
#include "DebugLog.h"
#endif

#pragma comment(lib, "dbghelp.lib")

#ifdef _DEBUG

// デバッグ初期化
void DEBUG::DebugInitialization(bool debugOutputFlag)
{
    DEBUG::DebugOutputFileFlag = debugOutputFlag;

    DEBUG::DebugProcessHandle = GetCurrentProcess();
    SymInitialize(DEBUG::DebugProcessHandle, NULL, TRUE);
    SymSetOptions(SYMOPT_LOAD_LINES);

    PlusLogFileData.clear();

    std::filesystem::create_directory("LogDebug");

    DEBUG::LogFileString = "LogDebug/debug_0.txt";
    DEBUG::LogFileProcess.clear();
}

// デバッグ出力先を新しいファイルにする
void DEBUG::DebugCreateLogFileName(DEBUG_MAP_TYPE debugMapType, std::vector<std::string> plusFileName)
{
    switch (debugMapType)
    {
    case DEBUG_MAP_TYPE::DEBUG_BASE:
    {
        int count = 1;
        std::string filename;
        while (true)
        {
            filename = "LogDebug/debug_" + std::to_string(count) + ".txt";
            if (!std::filesystem::exists(filename)) {
                break;
            }
            count++;
        }
        DEBUG::LogFileString = filename;
    }
        break;

    default:
    {
        DEBUG_FILE_DATA debugFileData;
        debugFileData.debugType.clear();
        debugFileData.plusFileName = plusFileName;
        DEBUG::PlusLogFileData[debugMapType] = debugFileData;
    }
        break;
    }
}

// デバッグ出力情報追加
void DEBUG::DebugLogAddData(DEBUG_PROCESS_TYPE debugProcessType, DEBUG_MAP_TYPE debugMapType)
{
    switch (debugMapType)
    {
    case DEBUG_MAP_TYPE::DEBUG_BASE:
        DEBUG::LogFileProcess.push_back(debugProcessType);
        break;

    default:
        if (DEBUG::PlusLogFileData.find(debugMapType) != DEBUG::PlusLogFileData.end())
        {
            DEBUG::PlusLogFileData[debugMapType].debugType.push_back(debugProcessType);
        }
        break;
    }
}

// デバッグ出力情報削除
void DEBUG::DebugLogSubData(DEBUG_PROCESS_TYPE debugProcessType, DEBUG_MAP_TYPE debugMapType)
{
    switch (debugMapType)
    {
    case DEBUG_MAP_TYPE::DEBUG_BASE:
    {
        auto& baseLogProcessType = DEBUG::LogFileProcess;
        auto baseIterator = std::find(baseLogProcessType.begin(), baseLogProcessType.end(), debugProcessType);
        if (baseIterator != baseLogProcessType.end())
        {
            baseLogProcessType.erase(baseIterator);
        }
        break;
    }

    default:
        if (DEBUG::PlusLogFileData.find(debugMapType) != DEBUG::PlusLogFileData.end())
        {
            auto& logProcessType = DEBUG::PlusLogFileData[debugMapType].debugType;
            auto plusIterator = std::find(logProcessType.begin(), logProcessType.end(), debugProcessType);
            if (plusIterator != logProcessType.end())
            {
                logProcessType.erase(plusIterator);
            }
        }
        break;
    }
}

// 文字列をファイルに追加する
void DEBUG::SaveText(std::string logString, DEBUG_MAP_TYPE debugMapType)
{
    if (!DEBUG::DebugOutputFileFlag)
    {
        return;
    }

    // 共通情報設定
    DEBUG_SAVE_TEXT_FUNCTION_DATA debugSaveTextFunctionData;
    {
        // 時間
        debugSaveTextFunctionData.timeString = DEBUG::TimeToString();
        debugSaveTextFunctionData.logString = logString;
        debugSaveTextFunctionData.debugMapType = debugMapType;
    }

    // 基本ファイル出力
    {
        std::ofstream baseFile(DEBUG::LogFileString, std::ios::app);

        if (!baseFile) {
            return;
        }
    
        baseFile << logString;
        for (int i = 0; i < DEBUG::LogFileProcess.size(); i++)
        {
            ProcessByDebugType(&baseFile, DEBUG::LogFileProcess[i], debugSaveTextFunctionData);
        }
    }
    
    if (DEBUG::PlusLogFileData.find(debugMapType) != DEBUG::PlusLogFileData.end())
    {
        for (std::string& plusFileName : DEBUG::PlusLogFileData[debugMapType].plusFileName)
        {
            std::ofstream plusFile(DEBUG::LogFileString.substr(0, DEBUG::LogFileString.size() - 4) + plusFileName + ".txt", std::ios::app);

            plusFile << logString;
            for (int i = 0; i < DEBUG::PlusLogFileData[debugMapType].debugType.size(); i++)
            {
                if (DEBUG::PlusLogFileData[debugMapType].debugType[i] == DEBUG::DEBUG_PROCESS_TYPE::ALL_FILE_OUTPUT)
                {
                    if (plusFileName != DEBUG::PlusLogFileData[debugMapType].plusFileName[0])
                    {
                        continue;
                    }
                }
                ProcessByDebugType(&plusFile, DEBUG::PlusLogFileData[debugMapType].debugType[i], debugSaveTextFunctionData);
            }
        }
    }
}

// デバッグ種類別の処理
void DEBUG::ProcessByDebugType(std::ofstream *file, DEBUG_PROCESS_TYPE debugProcessType, DEBUG_SAVE_TEXT_FUNCTION_DATA debugSaveTextFunctionData)
{
    switch (debugProcessType)
    {
    case DEBUG_PROCESS_TYPE::FUNCTION_CALL:
        *file << DEBUG::FunctionCallHistoryAcquisition(2);
        break;
    case DEBUG_PROCESS_TYPE::TIME:
        *file << debugSaveTextFunctionData.timeString;
        break;

    case DEBUG_PROCESS_TYPE::ALL_FILE_OUTPUT:
    {
        std::vector<std::string> outPutFiles;
        outPutFiles.clear();
        std::string nextFileName;
        for (auto pludFileData : DEBUG::PlusLogFileData)
        {
            if (pludFileData.first == debugSaveTextFunctionData.debugMapType)
            {
                continue;
            }

            for (std::string& plusFileName : pludFileData.second.plusFileName)
            {
                nextFileName = (DEBUG::LogFileString.substr(0, DEBUG::LogFileString.size() - 4) + plusFileName + ".txt");

                bool outPutFileFlag = false;
                for (int i = 0; i < outPutFiles.size(); i++)
                {
                    if (outPutFiles[i] == nextFileName)
                    {
                        outPutFileFlag = true;
                        break;
                    }
                }
                if (outPutFileFlag)
                {
                    continue;
                }

                // ファイル出力
                std::ofstream plusFile(nextFileName, std::ios::app);
                plusFile << debugSaveTextFunctionData.logString;

                // ファイル名保存
                outPutFiles.push_back(nextFileName);
            }
        }
    }
        break;
    }
}

// 関数名を取得
std::string DEBUG::FunctionCallHistoryAcquisition(int deleteNumber)
{
    // スタックフレームから関数名取得
    void* stack[50];
    USHORT stackFrames = CaptureStackBackTrace(0, 50, stack, NULL);

    /*---------- 変数宣言 ----------*/
    SYMBOL_INFO* symbol =
        (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256, 1);

    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    IMAGEHLP_LINE64 fileDataLine;
    DWORD displacement = 0;
    fileDataLine.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

    // 文字列取得用宣言
    std::string functionCallHistoryString;
    /*------------------------------*/

    // 関数名などに変換(この関数を抜いて処理する)
    for (USHORT i = deleteNumber; i < stackFrames; i++)
    {
        DWORD64 address = (DWORD64)stack[i];
        std::ostringstream oneFunctionCallHistoryString;

        // 関数名
        if (SymFromAddr(DEBUG::DebugProcessHandle, address, 0, symbol))
        {
            oneFunctionCallHistoryString << symbol->Name << "(";
        }
        else
        {
            oneFunctionCallHistoryString << "unknown(";
        }

        // ファイル名&行数
        if (SymGetLineFromAddr64(DEBUG::DebugProcessHandle, address, &displacement, &fileDataLine))
        {
            std::string fileName = fileDataLine.FileName;
            size_t fileNamePos = fileName.find_last_of("\\/");

            if (fileNamePos != std::string::npos)
            {
                fileName = fileName.substr(fileNamePos + 1);
            }

            oneFunctionCallHistoryString << fileName << ":" << fileDataLine.LineNumber << ") -> ";
        }
        else
        {
            oneFunctionCallHistoryString << ":) -> ";
        }
        functionCallHistoryString = oneFunctionCallHistoryString.str() + functionCallHistoryString;

        // メインまで行ったら終了
        if (!std::strcmp(symbol->Name, "WinMain") || !std::strcmp(symbol->Name, "main"))
        {
            break;
        }
    }

    // リソース解放
    free(symbol);

    // 関数名たちを返す
    return "    " + functionCallHistoryString + '\n';
}

// 時間文字列取得
std::string DEBUG::TimeToString()
{
    return "    TIME : " + std::to_string(timeGetTime()) + '\n';
}

#endif