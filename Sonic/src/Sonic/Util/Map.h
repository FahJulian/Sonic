#pragma once
#include "DynamicArray.h"

namespace Sonic {
	
	template<typename KeyType, typename ValueType>
	class Map
	{
		struct Pair
		{
			KeyType key;
			ValueType value;
		};

	private:
		DynamicArray<Pair> data;
	};

}
