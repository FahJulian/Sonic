#pragma once
#include <iostream>

#ifdef SONIC_DEBUG
    #define SONIC_LOG_DEBUG(x) std::cout << "DEBUG: " << x << std::endl;
    #define SONIC_LOG_DEBUG_ASSERT(b, x) if (!b) SONIC_LOG_DEBUG(x)
#else
    #define SONIC_LOG_DEBUG(x) ;
    #define SONIC_LOG_DEBUG_ASSERT(x) ;
#endif

