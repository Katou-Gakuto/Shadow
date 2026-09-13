#pragma once

#include <limits.h>

// Charのビットが8ビット前提のコードなので
#if CHAR_BIT != 8
#error "This program requires 8-bit char"
#endif

// 
union Num_Bit16
{
    char mcChar[2];
    unsigned short num;
};

