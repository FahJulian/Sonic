#pragma once

namespace Sonic {

    class Log
    {
    public:
        static void debugAssert(bool b, const char* msg);
        static void debug(const char* msg);
    };

}

#ifdef SONIC_DEBUG
    #define SONIC_LOG_DEBUG(x) Sonic::Log::debug(x);
    #define SONIC_LOG_DEBUG_ASSERT(b, x) Sonic::Log::debugAssert(b, x);
#else
    #define SONIC_LOG_DEBUG(x) ;
    #define SONIC_LOG_DEBUG_ASSERT(x) ;
#endif

