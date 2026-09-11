#pragma once

// コンパイル時点で静的な文字列を定義する際のマクロです
// class T
// {
// public:
//     static MYMACRO_CONST_NAME = "";
// };
// の形で使用してください
#define MYMACRO_CLASS_NAME constexpr const char *

// 

