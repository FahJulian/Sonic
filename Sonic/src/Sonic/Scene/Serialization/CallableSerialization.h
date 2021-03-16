#pragma once
#include "Sonic/Base.h"
#include "Sonic/App.h"
#include "Sonic/Util/Result.h"
#include "Sonic/Util/Optional.h"
#include "ClientSerialization.h"
#include "SerializedCallable.h"

namespace Sonic::Serialization {

	Optional<SerializedCallable> serializeCallable(const BaseCallable& callable)
	{
		if (callable.IsMethodOrFunction(App::stop))
			return "App::stop";

		return serializeClientMethod(callable);
	}

	template<typename Signature>
	Result<Ref<Callable<Signature>>, CallableDeserializationError>
		deserializeCallable(Script* script, const SerializedCallable& method)
	{
		if (Result<uintptr_t, CallableDeserializationError> result = deserializeClientMethod(script, method, getSignature<Signature>());
			result.HasError())
		{
			return result.GetError();
		}
		else
		{
			return Ref<Callable<Signature>>(reinterpret_cast<Callable<Signature>*>(result.GetResult()));
		}
	}

}