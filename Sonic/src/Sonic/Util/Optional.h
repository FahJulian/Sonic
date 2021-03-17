#pragma once

namespace Sonic {
	
	template<typename ValueType>
	class Optional
	{
		Optional()
			: hasValue(false), value(ValueType())
		{
		}

		Optional(const ValueType& value)
			: hasValue(true), value(value)
		{
		}

		bool hasValue;
		ValueType value;
	};

}
