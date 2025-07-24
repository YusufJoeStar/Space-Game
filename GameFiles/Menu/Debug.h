#pragma once
#include <iostream>

#define DEBUG_MODE true

#if DEBUG_MODE
    #define DEBUG_LOG(x) std::cout << x << std::endl
#else
    #define DEBUG_LOG(x)
#endif
