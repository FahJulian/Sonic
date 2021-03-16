#pragma once
#include "Sonic/Util/Result.h"
#include "Sonic/Util/Optional.h"
#include "Sonic/Scene/Serialization/CallableSignature.h"
#include "Sonic/Scene/Serialization/SerializedCallable.h"


#ifdef SONIC_CLIENT_SERIALIZATION_RELEASE
	#ifdef SONIC_CLIENT_SERIALIZATION_IMPORT
	#define SONIC_CLIENT_SERIALIZATION extern
	#else
	#define SONIC_CLIENT_SERIALIZATION 
	#endif
#else
	#ifdef SONIC_CLIENT_SERIALIZATION_IMPORT
	#define SONIC_CLIENT_SERIALIZATION __declspec(dllimport)
	#else
	#define SONIC_CLIENT_SERIALIZATION __declspec(dllexport)
	#endif
#endif


namespace Sonic::Serialization {
	
	SONIC_CLIENT_SERIALIZATION Optional<Script*> createClientScript(const String& scriptClass,
		const std::unordered_map<String, String>& data);

	SONIC_CLIENT_SERIALIZATION Optional<SerializedCallable> serializeClientMethod(const BaseCallable& method);

	SONIC_CLIENT_SERIALIZATION Result<uintptr_t, CallableDeserializationError>
		deserializeClientMethod(Script* script, const SerializedCallable& method, const CallableSignature& signature);

}
