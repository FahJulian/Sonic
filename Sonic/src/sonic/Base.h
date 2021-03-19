#pragma once

#include <stdexcept>

#ifdef SN_DEBUG
	#define SN_ASSERT(b, message) if (!(b)) throw std::runtime_error(message);
#else
	#define SN_ASSERT(b, ...)
#endif
