#pragma once

#include <string>
#include <vector>
#include <stdio.h>

// 自身のCSVファイルに格納できるクラスを列挙したもの
enum CSVNumType : unsigned long
{
    CSVType_Null = 0,
    CSVType_Vector2D,
    CSVType_Vector3D,
};

// 自身のCSVファイルのCurrentを移動させる方法を列挙したもの
enum CSVNumCurrent : unsigned char
{
    CSVCurrent_Start,
    CSVCurrent_Now,
    CSVCurrent_End,
};

// CSVファイルの操作を行った際に起こったエラー
enum CSVNumResult : unsigned short
{
    CSVResult_Complate = 0,

    CSVResult_Error_NotOpen,

    CSVResult_Error_Create_NewFile,
    CSVResult_Error_Create_NotNullFile,
    CSVResult_Error_Destroy,

    CSVResult_Error_Null_File,
    CSVResult_Error_Null_Dest,
    CSVResult_Error_Null_Source,

    CSVResult_Error_Over_Start,
    CSVResult_Error_Over_End,

    CSVResult_Error_Write_File,
    CSVResult_Error_Read_File,
    CSVResult_Error_Read_DestType,

    CSVResult_Error_Seek_ContentsStart,
    CSVResult_Error_Rename,

    CSVResult_Error_EOF,
};

// ここで言うCSVとは
// 「拡張子が.txt且つ、データ一つ一つが" " + ","で区切られている」
// ものに限ります
// 
// .txtを開いてメモ帳などから直接変更することもできることを前提としています

// シークはType単位で行います
// また、Typeは改行によって区切られています
// 
// たとえば
// Objectクラスがあり、ObjectクラスはメンバーにVECTOR3Dクラスを持ってるとする
// その場合はObjectクラスが終わると改行をします
// また、VECTORの終わりには改行をしません
// なのでシークを進める場合は内部的にはputsを進めるだけ。

// このCSVファイルはファイルの先頭に「Contents」というものを書き込んでいます
// このContentsはどのTypeがファイルに書かれているかという「目次」の役割をしています
// また、ContentsもTypeと同様に一行で書かれていますが、ContentsとTypeの境には空の行が一行挿入されています
// 

// コンテンツの頭にはコンテンツタイプを書くこと。
// また、コンテンツを追加、削除した場合はテーブルにも同様の結果を反映させること。

// CSVファイルに対して読み書きを行うクラス
// 
// コンストラクタにはファイルパス(拡張子のみナシ)を渡してください
// なお、新しいCSVファイルを作る際はコンストラクタには""を渡してください
// その上でCreate()にファイルパスを渡してください
class CSVOperater
{
private:
    FILE *mpFile;                                   // 実際のファイル
    std::string msPath;                             // このオペレータが操作するファイルパスを記憶しておく変数
    std::vector<CSVNumType> mlContents;             // このファイルに書かれているオブジェクトを簡単管理するための変数

    unsigned long mnCurrent;                        // コンテンツシーク。現在の何個目のコンテンツを指しているのかを記憶しておく変数
    unsigned long mnOffset;                         // 最初のコンテンツのファイルシークを記憶しておく変数

    static char smlByte1000[1000];                  // 作業用空間。関数のたびに確保するのめんどくさいので

    // ファイル書き込み関数
    static CSVNumResult Write(FILE *filePtr, CSVNumType typeNum, const void *src);

    // ファイル読み込み関数(dstがnullptrの場合はシークを次の行に進めるだけ)
    CSVNumResult Read(CSVNumType dstType, void *dst);

    // dstに実データを入れる処理
    CSVNumResult SetDest(CSVNumType type, const std::vector<std::string> &paramBox, void *dst);

    // ファイルを確認し、プログラム上のコンテンツテーブルとoffsetを直す関数
    // ※この関数を呼ぶと、シークはコンテンツシーク0になります
    CSVNumResult ResetTableAndOffest();

    // 目的のIndexを取得する関数
    unsigned long GetTargetContenes(CSVNumCurrent currentNum, const signed long long &moveContents);

    // CurrentをファイルのStart地点に戻す関数
    CSVNumResult SeekStartFile();

    // CurrentをコンテンツのStart地点に戻す関数
    CSVNumResult SeekStartContents();

    // CurrentをStartからEnd方向に移動させる関数
    CSVNumResult Front(unsigned long moveContents);

    // Insertを行った際、新しいファイルに記述する最新版のコンテンツテーブルの文字列を取得する関数
    std::string CreateNewTableInsert(CSVNumType typeNum);

    // Deleteを行った際、新しいファイルに記述する最新版のコンテンツテーブルの文字列を取得する関数
    std::string CreateNewTableDelete();

    // ファイルの動的生成関数
    static CSVNumResult CreateHiddenFile(std::string newFilePath);

    // ファイルの動的破棄関数
    static CSVNumResult DestroyHiddenFile(FILE *fp);

    // CSV解釈関数
    // 「値, 」が繰り返された文字列からそれぞれのデータを抽出する関数
    static CSVNumType InterpretCSV(std::string &str, std::vector<std::string> &paramBox);

    // dstFileにあるコンテンツを指定の個数分だけsrcFileに書き写す関数
    // ※dstFile、srcFile共にファイルシークがコンテンツシーク単位にしたうえで渡してください
    static CSVNumResult ContentsCopy(FILE *dstFile, FILE *srcFile, unsigned long copyCount);

public:
    // コンストラクタ
    CSVOperater(const std::string &path);

    // デストラクタ
    ~CSVOperater();

    // ファイルの生成関数
    CSVNumResult Create(const std::string &newFilePath);

    // ファイルの破棄関数
    CSVNumResult Destroy();

    // ファイル操作開始関数
    CSVNumResult Open();

    // ファイル操作終了関数
    CSVNumResult Close();

    // データ挿入関数(現在位置に挿入)
    // コンテンツシークは現在Index + 1
    CSVNumResult Insert(CSVNumType typeNum, const void *src);

    // データ更新関数(現在位置を更新)
    // コンテンツシークは現在Index + 1
    CSVNumResult Update(CSVNumType typeNum, const void *src);

    // データ削除関数(現在位置を削除)
    // コンテンツシークは現在Index
    CSVNumResult Delete();

    // データ取得関数(現在位置を取得)
    // コンテンツシークは現在Index + 1
    CSVNumResult Load(CSVNumType dstType, void *dst);

    // Current変更関数()
    CSVNumResult SetCurrent(CSVNumCurrent currentNum, const signed long long &moveContents);

    // ファイル操作が行えるかどうか
    bool CheckFileOperation() const;

    // 自身が管理しているファイルをバイナリファイルに変換する
    // ※結構重い処理になりそう
    CSVNumResult ToBin();
};
