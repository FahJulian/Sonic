#pragma once

#include "sonic/util/String.h"

#include "sonic/core/scene/Scene.h"

#if defined(SN_CLIENT_LINK_STATIC)
	#if defined(SN_CLIENT_LINK_IMPORT)
		#define SN_CLIENT_API extern
	#elif defined(SN_CLIENT_LINK_EXPORT)
		#define SN_CLIENT_API
	#else
		#error Client linking method must be specified.
	#endif
#elif defined (SN_CLIENT_LINK_DYNAMIC)
	#if defined(SN_CLIENT_LINK_IMPORT)
		#define SN_CLIENT_API __declspec(dllimport)
	#elif defined(SN_CLIENT_LINK_EXPORT)
		#define SN_CLIENT_API __declspec(dllexport)
	#else
		#error Client linking method must be specified.
	#endif
#else
	#error Client linking method must be specified.
#endif

namespace sonic::client
{
	SN_CLIENT_API Scene* createScene(String sceneName);
}
