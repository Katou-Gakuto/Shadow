#pragma once

#include <functional>

// MyTicketに破棄関数を持たせておくことで、オブジェクトが動的に登録・解除しても問題ないようにする仕組み
// 勝手にReleaseしてくれるHandleみたいなイメージで良いかな
class MyTicket
{
public:
    // コンストラクタ
    MyTicket();

    // コンストラクタ
    MyTicket(std::function<void()> cleanup);

    // デストラクタ
    ~MyTicket();

    // コピー禁止
    MyTicket(const MyTicket &) = delete;
    MyTicket &operator=(const MyTicket &) = delete;

    // ムーブコンストラクタ
    MyTicket(MyTicket &&other) noexcept;

    // ムーブ
    MyTicket &operator=(MyTicket &&other) noexcept;

    // ムーブを行う関数
    MyTicket &Move(MyTicket &&other) noexcept;

    // 持っている破棄関数を使用する関数
    void Cleanup();

    // このチケットが有効なのか
    bool CheckActive() const;

private:
    // このMyRegistrationとHas関係にあるオブジェクトの破棄関数
    std::function<void()> mpCleanup;
};