#pragma once
#include "Sonic/Scene/Serialization/SceneSerializer.h"
#include "Sonic/Scene/Serialization/CallableSignature.h"


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


namespace Sonic {

	template<typename ReturnType, typename ScriptClass, typename... Args>
	Result<uintptr_t, CallableDeserializationError>
		assureSignatureAndCreateMethod(Script* baseScript, ReturnType(ScriptClass::* method)(Args...), const CallableSignature& signature)
	{
		ScriptClass* script = dynamic_cast<ScriptClass*>(baseScript);
		if (script == nullptr)
			return CallableDeserializationError::InvalidScriptClass;

		if (signature != method)
			return CallableDeserializationError::InvalidSignature;


		return (uintptr_t)new Method<ScriptClass, ReturnType(Args...)>(script, method);
	}
	
	SONIC_CLIENT_SERIALIZATION Script* createClientScript(const String& scriptClass);

	SONIC_CLIENT_SERIALIZATION Optional<SerializedCallable> serializeClientMethod(const BaseCallable& method);

	SONIC_CLIENT_SERIALIZATION Result<uintptr_t, CallableDeserializationError>
		deserializeClientMethod(Script* script, const SerializedCallable& method, const CallableSignature& signature);
}
