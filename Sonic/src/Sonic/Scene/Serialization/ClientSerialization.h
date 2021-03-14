#pragma once
#include "Sonic.h"
#include "Sonic/Scene/Serialization/SceneSerializer.h"

#ifdef SONIC_CLIENT_SERIALIZATION_IMPORT  
#define SONIC_CLIENT_SERIALIZATION __declspec(dllimport)
#else
#define SONIC_CLIENT_SERIALIZATION __declspec(dllexport)
#endif 


namespace Sonic {
	
	SONIC_CLIENT_SERIALIZATION Script* createClientScript(const String& scriptClass);

	SONIC_CLIENT_SERIALIZATION Optional<SerializedCallable> serializeClientMethod(const BaseCallable& method);

	template<typename Signature>
	SONIC_CLIENT_SERIALIZATION Result<Ref<Callable<Signature>>, CallableDeserializationError>
		deserializeClientMethod(Script* script, const SerializedCallable& method);

	template SONIC_CLIENT_SERIALIZATION Result<Ref<Callable<void(int)>>, CallableDeserializationError>
		deserializeClientMethod(Script* script, const SerializedCallable& method);
	template SONIC_CLIENT_SERIALIZATION Result<Ref<Callable<void(const UIEntityClickedEvent&)>>, CallableDeserializationError>
		deserializeClientMethod(Script* script, const SerializedCallable& method);
}
