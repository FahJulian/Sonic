#pragma once
#include "Sonic/Base.h"
#include "Sonic/Util/Result.h"
#include "Sonic/Util/Optional.h"
#include "Sonic/Scene/Components/Scripting/Script.h"
#include "Sonic/Scene/Scene.h"

namespace Sonic {

	enum class CallableDeserializationError : uint8_t
	{
		InvalidSignature,
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
		getMethod(ScriptClass* object, ResultType(ScriptClass::* method)(Args...))
	{
		if (typeid(Signature) != typeid(ResultType(Args...)))
			return CallableDeserializationError::InvalidSignature;

		auto it = new Method<ScriptClass, ResultType(Args...)>(object, method);
		return Ref<Callable<Signature>>(reinterpret_cast<Method<ScriptClass, Signature>*>(it));
	}

	class SceneSerializer
	{
	public:
		static void deserialize(Scene* scene, const String& relativeFilePath);
		static void serialize(Scene* scene, const String& filePath);
	};

}
