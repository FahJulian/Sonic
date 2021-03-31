#pragma once

#include <cstdint>
#include <utility>

namespace sonic
{
	template<typename R, typename... Args>
	class Function
	{
		static constexpr bool ALWAYS_FALSE = false;
		static_assert(ALWAYS_FALSE, "sonic::Function can only be used with function typenames");
	};


	template<typename R, typename... Args>
	class Function<R(Args...)>
	{
	private:
		template<typename T>
		using MemberFunction = R(T::*)(Args...) const;

		using NonMemberFunction = R(* const)(Args...);

		using FunctionCaller = R(Function::*)(Args&&...) const;

	public:
		constexpr Function()
			: mTypePunnedObject(0), mTypePunnedFunction(0), mFunctionCaller(0)
		{
		}

		Function(NonMemberFunction function)
			: mTypePunnedObject(0), mTypePunnedFunction(*reinterpret_cast<uintptr_t*>(&function)),
			mFunctionCaller(function != nullptr ? &Function::_callFunction : nullptr)
		{
		}

		template<typename T>
		Function(T* object, MemberFunction<T> memberFunction)
			: mTypePunnedObject(reinterpret_cast<uintptr_t>(object)), mTypePunnedFunction(*reinterpret_cast<uintptr_t*>(&memberFunction)),
			mFunctionCaller((object != nullptr && memberFunction != nullptr) ? &Function::_callMemberFunction<T> : nullptr)
		{
		}

		template<typename L>
		Function(const L& lambda)
			: mTypePunnedObject(0), mTypePunnedFunction(_createLambda(lambda)), 
				mFunctionCaller(&Function::_callLambda<L>)
		{
		}

		~Function()
		{
			if (isLambda())
				_deleteLambda(mTypePunnedFunction);
		}

		R operator()(Args... args) const
		{
			return (this->*mFunctionCaller)(std::forward<Args>(args)...);
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
			return mFunctionCaller == &Function::_callFunction;
		}

		bool isMemberFunction() const
		{
			return isValid() && !isFunction() && mTypePunnedObject != 0;
		}

		bool isLambda() const
		{
			return !isFunction() && !isMemberFunction();
		}

		template<typename R1, typename... Args1>
		bool isFunction(R1(*function)(Args...)) const
		{
			return function != nullptr && mTypePunnedFunction == *reinterpret_cast<uintptr_t*>(&function);
		}

		template<typename T, typename R1, typename... Args1>
		bool isMemberFunction(R1(T::* memberFunction)(Args1...)) const
		{
			return memberFunction != nullptr && mTypePunnedFunction == *reinterpret_cast<uintptr_t*>(&memberFunction);
		}

		template<typename T>
		bool isMemberFunctionOfObject(T* object) const
		{
			return object != nullptr && mTypePunnedObject == reinterpret_cast<uintptr_t>(object);
		}

	private:
		template<typename L>
		static inline uintptr_t _createLambda(const L& lambda)
		{
			L* lambdaLocation = reinterpret_cast<L*>(operator new(sizeof(L*)));
			new(lambdaLocation) L(lambda);
			return reinterpret_cast<uintptr_t>(lambdaLocation);
		}

		static inline void _deleteLambda(uintptr_t lambda)
		{
			operator delete(reinterpret_cast<void*>(lambda));
		}

		R _callFunction(Args&&... args) const
		{
			NonMemberFunction function = *reinterpret_cast<NonMemberFunction*>(&mTypePunnedFunction);
			return function(std::forward<Args>(args)...);
		}

		template<typename T>
		R _callMemberFunction(Args&&... args) const
		{
			T* object = reinterpret_cast<T*>(mTypePunnedObject);
			MemberFunction<T> memberFunction = *reinterpret_cast<MemberFunction<T>*>(&mTypePunnedFunction);

			return (object->*memberFunction)(std::forward<Args>(args)...);
		}

		template<typename L>
		R _callLambda(Args&&... args) const
		{
			L* lambda = reinterpret_cast<L*>(mTypePunnedFunction);
			return (*lambda)(std::forward<Args>(args)...);
		}

		uintptr_t mTypePunnedObject;
		uintptr_t mTypePunnedFunction;
		FunctionCaller mFunctionCaller;
	};

} // namespace sonic
