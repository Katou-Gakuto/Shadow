#include "MyTicket.h"

// コンストラクタ
MyTicket::MyTicket() :
    mpCleanup(nullptr)
{
}

// コンストラクタ
MyTicket::MyTicket(std::function<void()> cleanup) :
    mpCleanup(std::move(cleanup))
{
}

// デストラクタ
MyTicket::~MyTicket()
{
    if (this->CheckActive())
    {
        // 持っている破棄関数を使用する
        Cleanup();
    }
}

// ムーブコンストラクタ
MyTicket::MyTicket(MyTicket &&other) noexcept
{
    this->mpCleanup = std::move(other.mpCleanup);
}

// ムーブ
MyTicket &MyTicket::operator=(MyTicket &&other) noexcept
{
    this->mpCleanup = std::move(other.mpCleanup);
    return *this;
}

// 
MyTicket &MyTicket::Move(MyTicket &&other) noexcept
{
    this->mpCleanup = std::move(other.mpCleanup);
    return *this;
}

// 持っている破棄関数を使用する関数
void MyTicket::Cleanup()
{
    // 関数を持っている場合は処理を行う
    if (this->mpCleanup)
    {
        // 破棄関数を行う
        this->mpCleanup();

        // 関数を使用したので明示的にnullptrにして関数を呼べないようにする
        this->mpCleanup = nullptr;
    }
}

// 
bool MyTicket::CheckActive() const
{
    return this->mpCleanup != nullptr;
}
