#define _CRT_SECURE_NO_WARNINGS

#include "CSVOperater.h"

#include <sstream>
#include <string>

// string→?にするやつ
#include "MyFunctions.h"

// 書き読み可能なクラスのinclude
#include "VECTOR.h"

// 作業用空間。関数のたびに確保するのめんどくさいので
char CSVOperater::smlByte1000[1000];

// ファイル書き込み関数
CSVNumResult CSVOperater::Write(FILE *filePtr, CSVNumType typeNum, const void *src)
{
    // ソースがnullptrの場合は処理を行う
    if (src == nullptr)
    {
        // ソースがnullptrであることを報告する
        return CSVNumResult::CSVResult_Error_Null_Source;
    }

    // 
    switch (typeNum)
    {


    default:
        break;
    }

    // 
    return CSVNumResult::CSVResult_Complate;
}

// ファイル読み込み関数(dstがnullptrの場合はシークを次の行に進めるだけ)
CSVNumResult CSVOperater::Read(CSVNumType dstType, void *dst)
{
    // 1000Byteあれば、いくら何でも流石に改行されるでしょ。
    char *result = fgets(CSVOperater::smlByte1000, 1000, this->mpFile);

    // 既にファイルシークが終点だった場合は処理を行う
    if (result == NULL)
    {
        // コンテンツシークが配列の大きさと同じ(配列の一つ外)にあることを記録しておく
        this->mnCurrent = this->mlContents.size();

        // ファイルシークが終点だったことを返す
        return CSVNumResult::CSVResult_Error_EOF;
    }

    // コンテンツシークを更新する
    this->mnCurrent++;

    // std::string型を生成する
    std::string str = CSVOperater::smlByte1000;

    // dstがnullptrでない場合は特別な処理を行う
    if (dst != nullptr)
    {
        // 各データの入れ物
        std::vector<std::string> paramBox;

        // 各データを入れ物に格納する
        CSVNumType type = CSVOperater::InterpretCSV(str, paramBox);

        // dstのTypeとCSVから取得したTypeが同じであるかを確認する
        if (type == dstType)
        {
            // 各データをdstに格納する
            this->SetDest(type, paramBox, dst);
        }
        else
        {
            // 予めコンテンツテーブルを確認済みなので、間違ってるわけがない
            // よって、再度コンテンツテーブルを読み込みなおす
            this->ResetTableAndOffest();

            // 一致していなかったため処理を終える
            return CSVNumResult::CSVResult_Error_Read_DestType;
        }
    }

    // 成功したことを返す
    return CSVNumResult::CSVResult_Complate;
}

// dstに実データを入れる処理
CSVNumResult CSVOperater::SetDest(CSVNumType type, const std::vector<std::string> &paramBox, void *dst)
{
    // type毎に処理を変える
    switch (type)
    {
    case CSVNumType::CSVType_Null:
        break;
    case CSVNumType::CSVType_Vector2D:
    {
        VECTOR2D *vec = (VECTOR2D *)dst;
        vec->SetX(MyFunctions::str2float(paramBox[0]));
        vec->SetY(MyFunctions::str2float(paramBox[1]));
        break;
    }
    default:
        break;
    }

    // 
    return CSVNumResult::CSVResult_Complate;
}

// ファイルを確認し、プログラム上のコンテンツテーブルとoffsetを直す関数
// ※この関数を呼ぶと、シークはコンテンツシーク0になります
CSVNumResult CSVOperater::ResetTableAndOffest()
{
    // 現在のコンテンツシークの位置を記憶しておく
    unsigned long current = this->mnCurrent;

    // ファイルの開始地点に移動する
    this->SeekStartFile();

    // 現在の行を取得する
    char *result = fgets(CSVOperater::smlByte1000, 1000, this->mpFile);
    if (result == NULL)
    {
        return CSVNumResult::CSVResult_Error_Read_File;
    }

    // 今回取得したデータをstd::string型に変換する
    std::string str = CSVOperater::smlByte1000;

    // ストリームに文字列を設定する
    std::stringstream ss(str);

    // 作業用の文字列
    std::string paramStr;

    // enumとenumにただの整数を格納するためのポインタ
    CSVNumType tempMemory;
    unsigned long *tempPtr = (unsigned long *)(&tempMemory);

    // 読み終わるまで終わらない
    while (std::getline(ss, paramStr, ','))
    {
        // 先頭に半角スペースがある場合は処理を行う
        if (!paramStr.empty() && paramStr[0] == ' ')
        {
            // 半角スペースを削除する
            paramStr.erase(0, 1);
        }

        // 先頭の半角スペースを消した後でもまだ残っていた場合は処理を行う
        if (!paramStr.empty())
        {
            // unsigned long *を使って無理やりenumの中にファイルから取得した符号なし整数を代入する
            *tempPtr = std::stoul(paramStr);

            // データを格納
            this->mlContents.push_back(tempMemory);
        }
    }

    // 空の行を読み飛ばす
    result = fgets(CSVOperater::smlByte1000, 1000, this->mpFile);
    if (result == NULL)
    {
        return CSVNumResult::CSVResult_Error_Read_File;
    }

    // offsetを更新する
    this->mnOffset = ftell(this->mpFile);

    // ここで初めてコンテンツシーク0の場所にファイルシークが到達したので
    this->mnCurrent = 0;

    // コンテンツシークを関数開始時点の位置に戻す
    return CSVNumResult::CSVResult_Complate;
}

// 目的のIndexを取得する関数
unsigned long CSVOperater::GetTargetContenes(CSVNumCurrent currentNum, const signed long long &moveContents)
{
    // 返り値
    unsigned long temp = 0;
    switch (currentNum)
    {
    case CSVNumCurrent::CSVCurrent_Start:
        // Startからなのでそのまま代入
        temp = moveContents;
        break;
    case CSVNumCurrent::CSVCurrent_Now:
        // moveContentsに現在位置を考慮する
        temp = (signed long long)this->mnCurrent + moveContents;
        break;
    case CSVNumCurrent::CSVCurrent_End:
        // moveContentsに終了位置を考慮する
        temp = this->mlContents.size() + moveContents;
        break;
    default:
        break;
    }

    // 結果を返す
    return temp;
}

// CurrentをStart地点に戻す関数
CSVNumResult CSVOperater::SeekStartFile()
{
    // ファイルシークを先頭に戻す
    rewind(this->mpFile);

    // 成功したことを返す
    return CSVNumResult::CSVResult_Complate;
}

// CurrentをコンテンツのStart地点に戻す関数
CSVNumResult CSVOperater::SeekStartContents()
{
    // fgets * 2を使うこともできるけど、自身のmnOffsetを信用します
    int result = fseek(this->mpFile, SEEK_SET, this->mnOffset);

    // seekの移動に失敗した場合は処理を行う
    if (result != 0)
    {
        // 失敗したことを返す
        return CSVNumResult::CSVResult_Error_Seek_ContentsStart;
    }

    // コンテンツシークを0に変更する
    this->mnCurrent = 0;

    // 成功したことを返す
    return CSVNumResult::CSVResult_Complate;
}

// CurrentをStartからEnd方向に移動させる
CSVNumResult CSVOperater::Front(unsigned long moveContents)
{
    // 移動するコンテンツの分だけループする
    for (unsigned long i = 0; i < moveContents; i++)
    {
        // ファイルを読み進める
        this->Read(CSVNumType::CSVType_Null, nullptr);
    }

    // 処理に成功したことを記憶しておく
    return CSVNumResult::CSVResult_Complate;
}

// Insertを行った際、新しいファイルに記述する最新版のコンテンツテーブルの文字列を取得する関数
std::string CSVOperater::CreateNewTableInsert(CSVNumType typeNum)
{
    // 返り値
    std::string newTable;

    // コンテンツシーク0～現在着目位置までのループ
    for (unsigned long i = 0; i < this->mnCurrent; i++)
    {
        // コンテンツのTypeNumを追加する
        newTable += this->mlContents[i];

        // 区切り文字である','と可読性を高める' 'を追加する
        newTable += ", ";
    }

    // 挿入するデータのTypeNumを追加する
    newTable += typeNum;

    // 区切り文字である','と可読性を高める' 'を追加する
    newTable += ", ";

    // 現在着目位置～コンテンツ終了地点までのループ
    for (unsigned long i = this->mnCurrent; i < this->mlContents.size(); i++)
    {
        // コンテンツのTypeNumを追加する
        newTable += this->mlContents[i];

        // 区切り文字である','と可読性を高める' 'を追加する
        newTable += ", ";
    }

    // 返り値を返す
    return newTable;
}

// Deleteを行った際、新しいファイルに記述する最新版のコンテンツテーブルの文字列を取得する関数
std::string CSVOperater::CreateNewTableDelete()
{
    // コンテンツテーブル文字列生成
    std::string newTable;

    // コンテンツテーブル全てを走査するループ
    for (unsigned long i = 0; i < this->mlContents.size(); i++)
    {
        // 着目タイプ以外は処理を行う
        if (i != this->mnCurrent)
        {
            // コンテンツのTypeNumを追加する
            newTable += this->mlContents[i];

            // 区切り文字である','と可読性を高める' 'を追加する
            newTable += ", ";
        }
    }

    // 返り値を返す
    return newTable;
}

// ファイルの動的生成関数
CSVNumResult CSVOperater::CreateHiddenFile(std::string newFilePath)
{
    // まず開くことができるかを確認する
    FILE *fp = fopen(newFilePath.c_str(), "r");
    if (fp != nullptr)
    {
        // ファイルを閉じる
        fclose(fp);

        // 既にファイルが存在していたことを報告する
        return CSVNumResult::CSVResult_Error_Create_NotNullFile;
    }

    // ファイルを生成する
    fp = fopen(newFilePath.c_str(), "w");
    if (fp == nullptr)
    {
        return CSVNumResult::CSVResult_Error_Create_NewFile;
    }

    // ファイルを閉じる
    fclose(fp);

    // 既にファイルが存在していたことを報告する
    return CSVNumResult::CSVResult_Complate;
}

// ファイルの動的破棄関数
CSVNumResult CSVOperater::DestroyHiddenFile(FILE *fp)
{
    // とりあえず返り値だけね
    return CSVNumResult::CSVResult_Complate;
}

// CSV解釈関数
// 「値, 」が繰り返された文字列からそれぞれのデータを抽出する関数
CSVNumType CSVOperater::InterpretCSV(std::string &str, std::vector<std::string> &paramBox)
{
    // ストリームに文字列を設定する
    std::stringstream ss(str);

    // 作業用変数
    std::string paramStr;

    // 返り値
    CSVNumType returnNum = CSVType_Null;

    // 最初のデータ(Type部分)を読み込む
    if (std::getline(ss, paramStr, ','))
    {
        // 力技で数字を代入する
        *(unsigned long *)(&returnNum) = std::stoul(paramStr);

        // 読み終わるまで終わらない
        while (std::getline(ss, paramStr, ','))
        {
            // 先頭の半角スペースを削除
            if (!paramStr.empty() && paramStr[0] == ' ')
            {
                paramStr.erase(0, 1);
            }

            // 先頭の半角スペースを消した後でもまだ残っていた場合は処理を行う
            if (!paramStr.empty())
            {
                // データを返り値に格納
                paramBox.push_back(paramStr);
            }
        }
    }

    // 値を返す
    return returnNum;
}

// dstFileにあるコンテンツを指定の個数分だけsrcFileに書き写す関数
// ※dstFile、srcFile共にファイルシークがコンテンツシーク単位にしたうえで渡してください
CSVNumResult CSVOperater::ContentsCopy(FILE *dstFile, FILE *srcFile, unsigned long copyCount)
{
    // ファイルの読み込みの実行結果
    char *readResult = nullptr;

    // ファイルの書き込みの実行結果
    int writeResult = 0;

    // コンテンツ(挿入される前の部分)を走査するループ
    for (unsigned long i = 0; i < copyCount; i++)
    {
        // srcFileからコンテンツについて記載されている文字列を読み込む
        readResult = fgets(CSVOperater::smlByte1000, 1000, srcFile);
        if (readResult == NULL)
        {
            return CSVNumResult::CSVResult_Error_Null_File;
        }
        // 読み込んだコンテンツ文字列をdstFileにそのまま書き込む
        writeResult = fprintf(dstFile, "%s", CSVOperater::smlByte1000);
        if (writeResult != 0)
        {
            return CSVNumResult::CSVResult_Error_Write_File;
        }
    }

    // 成功したことを返す
    return CSVNumResult::CSVResult_Complate;
}

// コンストラクタ
CSVOperater::CSVOperater(const std::string &path) :
    mpFile(nullptr),
    msPath(path),
    mlContents(),
    mnCurrent(),
    mnOffset()
{

}

// デストラクタ
CSVOperater::~CSVOperater()
{

}

// ファイルの生成関数
CSVNumResult CSVOperater::Create(const std::string &newFilePath)
{
    // 
    CSVNumResult temp = CSVOperater::CreateHiddenFile(newFilePath);
    this->mnCurrent = 0;
    this->mnOffset = 0;


    // とりあえず返り値だけね
    return CSVNumResult::CSVResult_Complate;
}

// ファイルの破棄関数
CSVNumResult CSVOperater::Destroy()
{

    // とりあえず返り値だけね
    return CSVNumResult::CSVResult_Complate;
}

// ファイル操作開始関数
CSVNumResult CSVOperater::Open()
{
    // 既にファイルを開いていた場合は処理を行う
    if (this->mpFile != nullptr)
    {
        // ファイルが開けているので成功を返す
        return CSVNumResult::CSVResult_Complate;
    }

    // 完全なファイルのパスを生成する
    std::string path = this->msPath + ".txt";

    // 読み書き(追記)モードでファイルを開く
    this->mpFile = fopen(path.c_str(), "a+");

    // ファイルを開くのに失敗した場合は処理を行う
    if (this->mpFile == nullptr)
    {
        // ファイルを開くことに失敗したことを返す
        return CSVNumResult::CSVResult_Error_Null_File;
    }

    // 
    char *result = fgets(CSVOperater::smlByte1000, 1000, this->mpFile);




    // ファイルを開くことに成功したことを返す
    return CSVNumResult::CSVResult_Complate;
}

// ファイル操作終了関数
CSVNumResult CSVOperater::Close()
{
    // 既にファイルを閉じていた場合は処理を行う
    if (this->mpFile == nullptr)
    {
        // ファイルが閉じているので成功を返す
        return CSVNumResult::CSVResult_Complate;
    }

    // ファイルを閉じる
    fclose(this->mpFile);

    // ファイルを閉じることに成功したことを返す
    return CSVNumResult::CSVResult_Complate;
}

// データ挿入関数(現在位置に挿入)
// コンテンツシークは現在Index + 1
CSVNumResult CSVOperater::Insert(CSVNumType typeNum, const void *src)
{
    // 現在開いているファイルのディレクトリを保持する関数
    std::string directory;

    // パスをディレクトリとファイル名に分ける
    MyFunctions::GetDirectoryAndFileName(this->msPath, &directory, nullptr);

    // 新しいファイルのパスを取得
    std::string newFilePath = directory + "newFile.txt";

    // 新しいファイルのポインタを格納する変数
    FILE *newFilePtr = nullptr;

    // 新しいファイルを生成する
    CSVNumResult createResult = CSVOperater::CreateHiddenFile(newFilePath);

    // 作成に失敗した場合は処理を行う
    if (createResult != CSVResult_Complate ||
        newFilePtr == NULL)
    {
        // どのように作成に失敗したかを返す
        return createResult;
    }

    // ファイル操作関数の返り値を持っておく変数(read)
    char *readResult;

    // ファイル操作関数の返り値を持っておく変数(write)
    int writeResult;

    // 新しいコンテンツテーブル文字列生成
    std::string newTable = this->CreateNewTableInsert(typeNum);

    // 生成したコンテンツテーブル文字列を新しいファイルに書き込む
    writeResult = fprintf(newFilePtr, "%s", CSVOperater::smlByte1000);
    if (writeResult != 0)
    {
        return CSVNumResult::CSVResult_Error_Write_File;
    }

    // テーブルとコンテンツの間に位置する空の行を書き込む
    writeResult = fprintf(newFilePtr, "\n");
    if (writeResult != 0)
    {
        return CSVNumResult::CSVResult_Error_Write_File;
    }

    // 挿入する場所を記憶しておく
    unsigned long insertIndex = this->mnCurrent;

    // ファイルシークをコンテンツシークの頭に設定する
    this->SeekStartContents();

    // 挿入地点までのコンテンツをコピーする
    CSVOperater::ContentsCopy(newFilePtr, this->mpFile, insertIndex);

    // 挿入したいデータを新しいファイルに入れる
    this->Write(newFilePtr, typeNum, src);

    // 挿入地点からのコンテンツをコピーする
    CSVOperater::ContentsCopy(newFilePtr, this->mpFile, this->mlContents.size() - insertIndex);

    // 古いファイルを破棄する
    CSVNumResult destroyResult = CSVOperater::DestroyHiddenFile(this->mpFile);
    if (destroyResult != CSVNumResult::CSVResult_Complate)
    {
        return destroyResult;
    }

    // 新しく作ったファイルの名前を、古いファイルの名前と同じにする
    int renameResult = rename(newFilePath.c_str(), this->msPath.c_str());
    if (renameResult != 0)
    {
        return CSVNumResult::CSVResult_Error_Rename;
    }

    // ファイルのポインタを更新する
    this->mpFile = newFilePtr;

    // 自身の持つType配列とoffsetの値を更新する
    this->ResetTableAndOffest();

    // シークを現在と同じだけ進め、mnCurrentの値を更新する
    this->Front(insertIndex + 1);

    // rename()によって同じパスを持っているはずなので、msPathの更新はしない
    // this->msPath = newFilePath;

    // 成功したことを返す
    return CSVNumResult::CSVResult_Complate;
}

// データ更新関数(現在位置を更新)
// コンテンツシークは現在Index + 1
CSVNumResult CSVOperater::Update(CSVNumType typeNum, const void *src)
{
    // 自身のCSVファイルの操作が可能な状態かを確認する
    if (!this->CheckFileOperation())
    {
        // ファイル操作がまだできないということを記憶しておく
        return CSVNumResult::CSVResult_Error_NotOpen;
    }

    // 書き込みの結果を取得する
    CSVNumResult result = this->Write(this->mpFile, typeNum, src);

    // 書き込みに成功した場合は処理を行う
    if (result == CSVResult_Complate)
    {
        // コンテンツシークを進める
        this->mnCurrent++;
    }

    // 
    return result;
}

// データ削除関数(現在位置を削除)
// コンテンツシークは現在Index
CSVNumResult CSVOperater::Delete()
{
    // 自身のCSVファイルの操作が可能な状態かを確認する
    if (!this->CheckFileOperation())
    {
        // ファイル操作がまだできないということを記憶しておく
        return CSVNumResult::CSVResult_Error_NotOpen;
    }

    // 現在開いているファイルのディレクトリを保持する関数
    std::string directory;

    // パスをディレクトリとファイル名に分ける
    MyFunctions::GetDirectoryAndFileName(this->msPath, &directory, nullptr);

    // 新しいファイルのパスを取得
    std::string newFilePath = directory + "newFile.txt";

    // 新しいファイルのポインタを格納する変数
    FILE *newFilePtr = nullptr;

    // 新しいファイルを生成する
    CSVNumResult createResult = CSVOperater::CreateHiddenFile(newFilePath);

    // 作成に失敗した場合は処理を行う
    if (createResult != CSVResult_Complate ||
        newFilePtr == NULL)
    {
        // どのように作成に失敗したかを返す
        return createResult;
    }

    // ファイル操作関数の返り値を持っておく変数(read)
    char *readResult;

    // ファイル操作関数の返り値を持っておく変数(write)
    int writeResult;

    // 新しいコンテンツテーブル文字列生成
    std::string newTable = this->CreateNewTableDelete();

    // 生成したコンテンツテーブル文字列を新しいファイルに書き込む
    writeResult = fprintf(newFilePtr, "%s", CSVOperater::smlByte1000);
    if (writeResult != 0)
    {
        return CSVNumResult::CSVResult_Error_Write_File;
    }

    // テーブルとコンテンツの間に位置する空の行を書き込む
    writeResult = fprintf(newFilePtr, "\n");
    if (writeResult != 0)
    {
        return CSVNumResult::CSVResult_Error_Write_File;
    }

    // 削除する場所を記憶しておく
    unsigned long deleteIndex = this->mnCurrent;

    // ファイルシークをコンテンツシークの頭に設定する
    this->SeekStartContents();

    // 削除地点までのコンテンツをコピーする
    CSVOperater::ContentsCopy(newFilePtr, this->mpFile, deleteIndex);

    // 削除したいコンテンツを飛ばす
    this->Front(1);

    // 削除地点からのコンテンツをコピーする
    CSVOperater::ContentsCopy(newFilePtr, this->mpFile, this->mlContents.size() - this->mnCurrent);

    // 古いファイルを破棄する
    CSVNumResult destroyResult = CSVOperater::DestroyHiddenFile(this->mpFile);
    if (destroyResult != CSVNumResult::CSVResult_Complate)
    {
        return destroyResult;
    }

    // 新しく作ったファイルの名前を、古いファイルの名前と同じにする
    int renameResult = rename(newFilePath.c_str(), this->msPath.c_str());
    if (renameResult != 0)
    {
        return CSVNumResult::CSVResult_Error_Rename;
    }

    // ファイルのポインタを更新する
    this->mpFile = newFilePtr;

    // 自身の持つType配列とoffsetの値を更新する
    this->ResetTableAndOffest();

    // シークを現在と同じだけ進め、mnCurrentの値を更新する
    this->Front(deleteIndex);

    // rename()によって同じパスを持っているはずなので、msPathの更新はしない
    // this->msPath = newFilePath;

    // 成功したことを返す
    return CSVNumResult::CSVResult_Complate;
}

// データ取得関数(現在位置を取得)
// コンテンツシークは現在Index + 1
CSVNumResult CSVOperater::Load(CSVNumType dstType, void *dst)
{
    // 自身のCSVファイルの操作が可能な状態かを確認する
    if (!this->CheckFileOperation())
    {
        // ファイル操作がまだできないということを記憶しておく
        return CSVNumResult::CSVResult_Error_NotOpen;
    }

    // 読み込んだデータを格納する変数がnullptrの場合は処理を行う
    if (dst == nullptr)
    {
        // 格納先がnullptrであることを返す
        return CSVNumResult::CSVResult_Error_Null_Dest;
    }

    // 現在のコンテンツシークがコンテンツテーブル内に収まっているかを確認する
    if (this->mlContents.size() <= this->mnCurrent)
    {
        // 改めて自身のコンテンツシークをコンテンツテーブルの一つ外にしておく
        this->mnCurrent = this->mlContents.size();

        // コンテンツシークがコンテンツテーブル外にあることを返す
        return CSVNumResult::CSVResult_Error_EOF;
    }

    // コンテンツテーブルの情報とdstTypeが一致しているかを確認する
    if (dstType == this->mlContents[this->mnCurrent])
    {
        // 一致していなかったため処理を終える
        return CSVNumResult::CSVResult_Error_Read_DestType;
    }

    // 読み込みを行う
    return this->Read(dstType, dst);
}

// Current変更関数
CSVNumResult CSVOperater::SetCurrent(CSVNumCurrent currentNum, const signed long long &moveContents)
{
    // 自身のCSVファイルの操作が可能な状態かを確認する
    if (!this->CheckFileOperation())
    {
        // ファイル操作がまだできないということを記憶しておく
        return CSVNumResult::CSVResult_Error_NotOpen;
    }

    // -方向に移動しようとしている場合 &&
    // (Start地点から移動しようとした場合 ||
    // End地点からsize以上の距離を移動しようとした場合 ||
    // Now地点からStart地点に戻る以上に移動しようとした場合 ||)
    if (moveContents < 0 &&
        ((currentNum == CSVNumCurrent::CSVCurrent_Start) ||
            (currentNum == CSVNumCurrent::CSVCurrent_End && this->mlContents.size() <= -moveContents) ||
            (currentNum == CSVNumCurrent::CSVCurrent_Now && this->mnCurrent < -moveContents)))
    {
        // Currentがファイル外(Start方向)に出ようとしたことを返す
        return CSVNumResult::CSVResult_Error_Over_Start;
    }

    // +方向に移動しようとしている場合 &&
    // End地点から移動しようとした場合 ||
    // Start地点からsize以上の距離を移動しようとした場合 ||
    // Now地点からEnd地点に移動する以上に移動しようとした場合 ||)
    if (0 < moveContents &&
        ((currentNum == CSVNumCurrent::CSVCurrent_End) ||
        (currentNum == CSVNumCurrent::CSVCurrent_Start && this->mlContents.size() <= moveContents) ||
        (currentNum == CSVNumCurrent::CSVCurrent_Now && this->mlContents.size() - this->mnCurrent <= moveContents)))
    {
        // Currentがファイル外(End方向)に出ようとしたことを返す
        return CSVNumResult::CSVResult_Error_Over_End;
    }

    // 目的のインデックス
    unsigned long target = this->GetTargetContenes(currentNum, moveContents);

    // プラス方向に移動するかどうか
    bool plus = target < this->mnCurrent;

    // End方向に移動する場合かで処理を変える
    if (plus)
    {
        // 移動量を計算し、その分だけEnd方向に進む
        this->Front(target - this->mnCurrent);
    }
    else
    {
        // CurrentをStart地点に戻す
        this->SeekStartContents();

        // 目的の位置までEnd方向に進む
        this->Front(target);
    }

    // 成功したことを返す
    return CSVNumResult::CSVResult_Complate;
}

// ファイル操作が行えるかどうか
bool CSVOperater::CheckFileOperation() const
{
    return this->mpFile != nullptr;
}
