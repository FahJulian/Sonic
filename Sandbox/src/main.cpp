#include <iostream>
#include <Sonic/Util/DynamicArray.h>
#include <Sonic/Util/Map.h>
#include <vector>

#include "sonic/util/String.h"
#include "sonic/debug/Log.h"

#include <string>

using namespace sonic;

int n = 0;

struct Test
{
	Test()
		: i(-1)
	{
		n++;
		std::cout << "Created empty" << std::endl;
	}

	Test(int i)
		: i(i)
	{
		n++;
		std::cout << "Created: " << i << std::endl;
	}

	Test(const Test& t)
		: i(t.i)
	{
		n++;
		std::cout << "Copied " << i << std::endl;
	}

	Test(Test&& t) noexcept
		: i(t.i)
	{
		n++;
		std::cout << "Moved " << i << std::endl;
	}

	Test& operator=(const Test& t)
	{
		i = t.i;
		std::cout << "Copied operator= " << i << std::endl;
		return *this;
	}

	Test& operator=(Test&& t) noexcept
	{
		i = t.i;
		std::cout << "Moved operator= " << i << std::endl;
		return *this;
	}

	bool operator==(const Test& other) const { return other.i == i; }

	~Test()
	{
		std::cout << "Destroyed: " << this->i << std::endl;
		i = 0;
		n--;
	}

	int i;
};

namespace sn
{
	struct Test1
	{

	};

	void test();
}

namespace sonic
{
	struct Test1
	{

	};

	void test();
}

namespace Sonic
{
	void test();

	struct Test1
	{

	};
}

int main()
{
	Log::init("C:/dev/Sonic/Sandbox/log/test_log.log", &std::cout, Log::TRACE, Log::TRACE);

	DynamicArray<Test> data = DynamicArray<Test>(4);

	data.setCapacity(0);

	data.setSize(4);

	data[0] = 0;
	data[0] = 0;
	data[0] = 0;
	data[0] = 0;
	data[1] = 1;

	Log::log(Log::INFO, "Constructing data2");

	DynamicArray<Test> data2 = DynamicArray<Test>();
	data2.add(1);
	data2.add(2);
	data2.add(3);

	Log::log(Log::INFO, "Copying...");

	data2 = data;

	Log::log(Log::INFO, "Done.");

	data2.add(8);

	Log::log(Log::INFO, "Constructing data3");

	DynamicArray<Test> data3 = DynamicArray<Test>();
	data3.add(1);
	data3.add(2);
	data3.add(3);

	Log::log(Log::INFO, "Moving...");

	data3 = std::move(data);

	Log::log(Log::INFO, "Done");

	data3.add(8);

	data.setSize(1);

	data.add(4);

	data.insert(0, 8);

	if (data.contains(1))
		data.remove(data.indexOf(1));

	data += 9;
	data += 9;
	data += 7;
	data += 9;

	std::cout << std::endl << n << std::endl;

	Map<int, float> map;
	Map<int, float> map2 = {
		Map<int, float>::Pair{ 3, 4.0f },
		Map<int, float>::Pair{ 2, 1.0f }
	};

	map = Map<int, float>();
	map = Map<int, float>(100);
	map = map2;
	map = std::move(map);

	map.insert({ 3, 4 });
	map.insert(3, 4);
	map.insert(2, 5);
	map.insert({ 2, 5 });

	bool b0 = map.containsKey(3);
	bool b1 = map.containsKey(100);
	bool b2 = map.containsValue(5);
	bool b3 = map.containsValue(100);

	map.remove(3);

	map.setSize(50);
	map.setCapacity(100);
	map.shrink();

	for (auto& [key, value] : map)
		Log::log(Log::TRACE, "(", key, ", ", value, ")");

	map.get(2) = 4;
	map[2] = 3;

	Log::writeToFile();

	Log::log(Log::TRACE, "TEST");

	Log::writeToFile();

	Log::log(Log::TRACE, String::valueOf(354348.9989f, true));
	Log::log(Log::TRACE, String::valueOf(85394.435, true));
	Log::log(Log::TRACE, String::valueOf(static_cast<long double>(85394.435), true));
}
