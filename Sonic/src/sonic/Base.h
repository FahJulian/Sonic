#pragma once
#include "sonic/debug/Log.h"

#ifdef SN_DEBUG
#define SN_ASSERT(b, ...) if (!(b)) { sonic::Log::log(sonic::Log::FATAL, __VA_ARGS__); exit(-1); }	// TODO: Stop the app
#else
#define SN_ASSERT(b, ...)
#endif
