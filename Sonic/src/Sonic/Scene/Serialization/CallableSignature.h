#pragma once
#include "Sonic/Base.h"
#include "Sonic/Util/Callable.h"

namespace Sonic
{
	class CallableSignature
	{
	public:
		template<typename ReturnType, typename... Args>
		CallableSignature(Callable<ReturnType(Args...)>* callable)
		{
			Init<ReturnType, Args...>();
		}

		template<typename ReturnType, typename... Args>
		CallableSignature(Ref<Callable<ReturnType(Args...)>> callable)
			: CallableSignature(callable.get())
		{
		}

		template<typename ReturnType, typename Class, typename... Args>
		CallableSignature(ReturnType(Class::* method)(Args...))
		{
			Init<ReturnType, Args...>();
		}

		template<typename ReturnType, typename... Args>
		CallableSignature(ReturnType(* function)(Args...))
		{
			Init<ReturnType, Args...>();
		}

		bool operator==(const CallableSignature& other) const;
		bool operator!=(const CallableSignature& other) const;

	private:
		template<typename Arg>
		void InitArgs()
		{
			m_ArgTypeHashes.push_back(typeid(Arg).hash_code());
		}

		template<typename Arg0, typename Arg1, typename... Args>
		void InitArgs()
		{
			InitArgs<Arg0>();
			InitArgs<Arg1, Args...>();
		}

		template<typename ReturnType>
		void Init()
		{
			m_ReturnTypeHash = typeid(ReturnType).hash_code();
		}

		template<typename ReturnType, typename Arg, typename... Args>
		void Init()
		{
			Init<ReturnType>();
			InitArgs<Arg, Args...>();
		}

	private:
		size_t m_ReturnTypeHash;
		std::vector<size_t> m_ArgTypeHashes;
	};

	template<typename Signature>
	inline CallableSignature getSignature()
	{
		Callable<Signature>* tmp = nullptr;
		return CallableSignature(tmp);
	}

}
