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
		: i(-1), a(nullptr)
	{
		n++;
		std::cout << "Created empty" << std::endl;
	}

	Test(int i)
		: i(i), a(new char[2])
	{
		n++;
		std::cout << "Created: " << i << std::endl;
	}

	Test(const Test& t)
		: i(t.i), a(new char[2])
	{
		n++;
		std::cout << "Copied " << i << std::endl;
	}

	Test(Test&& t) noexcept
		: i(t.i), a(t.a)
	{
		n++;
		t.a = nullptr;
		std::cout << "Moved " << i << std::endl;
	}

	Test& operator=(const Test& t)
	{
		i = t.i;
		delete[] a;
		a = new char[2];
		std::cout << "Copied operator= " << i << std::endl;
		return *this;
	}

	Test& operator=(Test&& t) noexcept
	{
		delete[] a;
		a = t.a;
		t.a = nullptr;
		i = t.i;
		std::cout << "Moved operator= " << i << std::endl;
		return *this;
	}

	bool operator==(const Test& other) const { return other.i == i; }

	~Test()
	{
		delete[] a;
		std::cout << "Destroyed: " << this->i << std::endl;
		i = 0;
		n--;
	}

	int i;
	char* a;
};

Test test(int a, double b)
{
	return { };
}

int main()
{
	Log::init("C:/dev/Sonic/Sandbox/log/test_log.log", &std::cout, Log::TRACE, Log::TRACE);

	for (auto& s : String("........This..is..a.very..lo.ng..sentence.   ..").split(".."))
		Log::log(Log::TRACE, s);

	Log::writeToFile();
}
