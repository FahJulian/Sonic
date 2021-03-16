#pragma once
#include "Sonic/Base.h"
#include "Sonic/Util/Result.h"
#include "Sonic/Util/Optional.h"
#include "Sonic/Util/Callable.h"

namespace Sonic::Serialization {

	enum class CallableDeserializationError : uint8_t
	{
		InvalidSignature,
		InvalidScriptClass,
		NotFound
	};

	struct SerializedCallable
	{
		SerializedCallable() = default;

		SerializedCallable(const char* scriptClass, const char* method)
			: scriptClass(scriptClass), callable(method)
		{
		}

		SerializedCallable(const String& scriptClass, const String& method)
			: scriptClass(scriptClass), callable(method)
		{
		}

		SerializedCallable(const char* function)
			: callable(function)
		{
		}

		SerializedCallable(const String& function)
			: callable(function)
		{
		}

		Optional<String> scriptClass;
		String callable;

		bool IsMethod()
		{
			return scriptClass.HasValue();
		}
	};

	template<typename Signature, typename ScriptClass, typename ResultType, typename... Args>
	Result<Ref<Callable<Signature>>, CallableDeserializationError>
		assureSignature(ScriptClass* object, ResultType(ScriptClass::* method)(Args...))
	{
		if (typeid(Signature) != typeid(ResultType(Args...)))
			return CallableDeserializationError::InvalidSignature;

		auto it = new Method<ScriptClass, ResultType(Args...)>(object, method);
		return Ref<Callable<Signature>>(reinterpret_cast<Method<ScriptClass, Signature>*>(it));
	}

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

}
