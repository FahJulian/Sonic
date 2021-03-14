#pragma once

namespace Sonic {

	template<typename ValueType>
	struct Optional
	{
	private:
		bool hasValue;
		ValueType value;

	public:
		Optional()
			: hasValue(false)
		{
		}

		template<typename Arg, typename... Args>
		Optional(Arg&& arg, Args&&... args)
			: Optional(ValueType(std::forward<Arg>(arg), std::forward<Args>(args)...))
		{
		}

		Optional(ValueType value)
			: hasValue(true), value(value)
		{
		}

		bool operator==(const ValueType& other) const { return HasValue() && (value == other); }

		bool HasValue() const { return hasValue; }
		ValueType& GetValue() { return value; }
		const ValueType& GetValue() const { return value; }
	};

}
