#pragma once

#include <cstdint>

namespace sonic
{
	template<typename R, typename... Args>
	class Function
	{
		static constexpr bool alwaysFalse = false;
		static_assert(alwaysFalse, "sonic::Function can only be used with function typenames");
	};


	template<typename R, typename... Args>
	class Function<R(Args...)>
	{
	private:
		template<typename T>
		using MemberFunction = R(T::*)(Args...);

		using NonMemberFunction = R(*)(Args...);

		using FunctionCaller = R(*)(uintptr_t, uintptr_t, Args&&...);

	public:
		constexpr Function()
			: mTypePunnedObject(0), mTypePunnedFunction(0), mFunctionCaller(0)
		{
		}

		Function(NonMemberFunction function)
			: mTypePunnedObject(0), mTypePunnedFunction(*reinterpret_cast<uintptr_t*>(&function)),
			mFunctionCaller(function != nullptr ? callFunction : nullptr)
		{
		}

		template<typename T>
		Function(T* object, MemberFunction<T> memberFunction)
			: mTypePunnedObject(reinterpret_cast<uintptr_t>(object)), mTypePunnedFunction(*reinterpret_cast<uintptr_t*>(&memberFunction)),
			mFunctionCaller((object != nullptr && memberFunction != nullptr) ? callMemberFunction<T> : nullptr)
		{
		}

		R operator()(Args... args) const
		{
			return mFunctionCaller(mTypePunnedObject, mTypePunnedFunction, std::forward<Args>(args)...);
		}

		operator bool() const
		{
			return isValid();
		}

		bool isValid() const
		{
			return mFunctionCaller != nullptr;
		}

		bool isFunction() const
		{
			return mFunctionCaller == callFunction;
		}

		bool isMemberFunction() const
		{
			return isValid() && !isFunction();
		}

		template<typename R1, typename... Args1>
		bool isFunction(R1(*function)(Args...)) const
		{
			return function != nullptr && mTypePunnedFunction == *reinterpret_cast<uintptr_t*>(&function);
		}

		template<typename T, typename R1, typename... Args1>
		bool isMemberFunction(R1(T::*memberFunction)(Args1...)) const
		{
			return memberFunction != nullptr && mTypePunnedFunction == *reinterpret_cast<uintptr_t*>(&memberFunction);
		}

		template<typename T>
		bool isMemberFunctionOfObject(T* object) const
		{
			return object != nullptr && mTypePunnedObject == reinterpret_cast<uintptr_t>(object);
		}

	private:
		static R callFunction(uintptr_t typePunnedObject, uintptr_t typePunnedFunction, Args&&... args)
		{
			NonMemberFunction function = *reinterpret_cast<NonMemberFunction*>(&typePunnedFunction);
			return function(std::forward<Args>(args)...);
		}

		template<typename T>
		static R callMemberFunction(uintptr_t typePunnedObject, uintptr_t typePunnedFunction, Args&&... args)
		{
			T* object = reinterpret_cast<T*>(typePunnedObject);
			MemberFunction<T> memberFunction = *reinterpret_cast<MemberFunction<T>*>(&typePunnedFunction);

			return (object->*memberFunction)(std::forward<Args>(args)...);
		}

		uintptr_t mTypePunnedObject;
		uintptr_t mTypePunnedFunction;
		FunctionCaller mFunctionCaller;
	};

} // namespace sonic
