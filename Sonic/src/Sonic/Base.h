#pragma once
#include "Sonic/Debug/Log.h"

#ifdef SN_DEBUG
#define SN_ASSERT(b, ...) if (!(b)) { Sonic::Log::log(Sonic::Log::FATAL, __VA_ARGS__); exit(-1); }	// TODO: Stop the app
#else
#define SN_ASSERT(b, ...)
#endif
