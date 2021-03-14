#pragma once
#include <cstdint>
#include "Sonic/Scene/ECS/Entity.h"
#include "Sonic/Scene/Components/Scripting/Script.h"

template<typename> inline constexpr bool _false = false;

namespace Sonic {
	
	struct BaseCallable
	{
	private:
		uintptr_t object;
		uintptr_t function;
		Entity entity;

		BaseCallable(uintptr_t object, uintptr_t function, Entity entity)
			: object(object), function(function), entity(entity)
		{
		}

		BaseCallable(uintptr_t function)
			: object(0), function(function), entity(0)
		{
		}

	public:
		friend bool operator==(const BaseCallable& bc1, const BaseCallable& bc2)
		{
			return bc1.object == bc2.object && bc1.function == bc2.function;
		}

		template<typename T>
		bool IsMethodOrFunction(T method) const
		{
			return function == *((uintptr_t*)&method);
		}

		template<typename, typename...> friend class Function;
		template<typename, typename, typename...> friend class Method;
		friend class CallableSerializer;
	};


	template<typename ReturnType, typename... Args>
	class Callable
	{
		static_assert(_false<ReturnType>, "Callable has to be used with function types.");
	};


	template<typename ReturnType, typename... Args>
	class Function : public Callable<ReturnType, Args...>
	{
	};


	template<typename Class, typename ReturnType, typename... Args>
	class Method : public Callable<ReturnType, Args...>
	{
	};


	template<typename ReturnType, typename... Args>
	class Callable<ReturnType(Args...)>
	{
	public:
		virtual ReturnType operator()(Args... args) const = 0;
		virtual operator const BaseCallable() const = 0;

		bool operator==(const BaseCallable& other) { return (BaseCallable)*this == other; }
	};


	template<typename ReturnType, typename... Args>
	class Function<ReturnType(Args...)> : public Callable<ReturnType(Args...)>
	{
	public:
		Function(ReturnType(*function)(Args...))
			: function(function)
		{
		}

		ReturnType operator()(Args... args) const override
		{
			return function((args)...);
		}

		operator const BaseCallable() const override
		{
			return BaseCallable(*((uintptr_t*)&function));
		}

	private:
		ReturnType(*function)(Args...);
	};


	template<typename Class, typename ReturnType, typename... Args>
	class Method<Class, ReturnType(Args...)> : public Callable<ReturnType(Args...)>
	{
	public:
		Method(Class* object, ReturnType(Class::* function)(Args...))
			: object(object), function(function)
		{
			if (std::is_base_of<Script, Class>::value)
			{
				entity = ((Script*)object)->m_Entity;
			}
		}

		ReturnType operator()(Args... args) const override
		{
			return (object->*function)((args)...);
		}

		operator const BaseCallable() const override
		{
			return BaseCallable((uintptr_t)object, *((uintptr_t*)&function), entity);
		}

	private:
		Class* object;
		ReturnType(Class::* function)(Args...);
		Entity entity;
	};

}
