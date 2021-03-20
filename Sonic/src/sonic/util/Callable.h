#pragma once
#include <cstdint>

namespace sonic
{
	template<typename R, typename... Args>
	class Callable
	{
		static constexpr bool _alwaysFalse = false;
		static_assert(_alwaysFalse, "Callable can only be used with function typenames");
	};


	template<typename R, typename... Args>
	class Callable<R(Args...)>
	{
	private:
		template<typename T>
		using MemberFunction = R(T::*)(Args...);

		using Function = R(*)(Args...);

		using FunctionCaller = R(*)(uintptr_t, uintptr_t, Args...);

	public:
		Callable()
			: mTypePunnedObject(0), mTypePunnedFunction(0), mFunctionCaller(0)
		{
		}

		Callable(Function function)
			: mTypePunnedObject(0), mTypePunnedFunction(*reinterpret_cast<uintptr_t*>(&function)),
			mFunctionCaller(toFunctionCaller(function))
		{
		}

		template<typename T>
		Callable(T* object, MemberFunction<T> memberFunction)
			: mTypePunnedObject(reinterpret_cast<uintptr_t>(object)), mTypePunnedFunction(*reinterpret_cast<uintptr_t*>(&memberFunction)),
			mFunctionCaller(toFunctionCaller(object, memberFunction))
		{
		}

		R operator()(Args... args)
		{
			return mFunctionCaller(mTypePunnedObject, mTypePunnedFunction, (args)...);
		}

		operator bool()
		{
			return isValid();
		}

		bool isValid()
		{
			return mFunctionCaller != nullptr;
		}

	private:
		static FunctionCaller toFunctionCaller(Function function)
		{
			if (function == nullptr)
				return nullptr;

			FunctionCaller functionCaller = [](uintptr_t _, uintptr_t typePunnedFunction, Args... args)
			{
				Function function = *reinterpret_cast<Function*>(&typePunnedFunction);

				return function((args)...);
			};

			return functionCaller;
		}

		template<typename T>
		static FunctionCaller toFunctionCaller(T* object, MemberFunction<T> method)
		{
			if (object == nullptr || method == nullptr)
				return nullptr;

			FunctionCaller functionCaller = [](uintptr_t typePunnedObject, uintptr_t typePunnedFunction, Args... args)
			{
				T* object = reinterpret_cast<T*>(typePunnedObject);
				MemberFunction<T> memberFunction = *reinterpret_cast<MemberFunction<T>*>(&typePunnedFunction);

				return (object->*memberFunction)((args)...);
			};

			return functionCaller;
		}

		uintptr_t mTypePunnedObject;
		uintptr_t mTypePunnedFunction;

		union
		{
			uintptr_t mTypePunnedFunction;
			FunctionCaller mFunctionCaller;
		};
	};

} // namespace sonic
