#include <iostream>
#include <Sonic/Util/DynamicArray.h>
#include <vector>

#include "Sonic/Debug/Log.h"

using namespace Sonic;

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

	Test(const Test&& t) noexcept
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

int main()
{
	Log::init("C:/dev/log/test_log.log", &std::cout, Log::TRACE, Log::TRACE);

	DynamicArray<Test>* pdata = new DynamicArray<Test>(4);
	auto& data = *pdata;

	data.setCapacity(0);

	data.setSize(4);

	data[0] = 0;
	data[1] = 1;

	data.add(4);

	data.insert(0, 8);

	if (data.contains(1))
		data.remove(data.indexOf(1));

	data += 9;
	data += 9;
	data += 7;
	data += 9;

	delete pdata;

	std::cout << std::endl << n << std::endl;
}
