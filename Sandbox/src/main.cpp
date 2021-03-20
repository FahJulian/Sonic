#include <iostream>
#include <Sonic/Util/DynamicArray.h>
#include <Sonic/Util/Map.h>
#include <vector>

#include "sonic/debug/Log.h"
#include "sonic/util/String.h"
#include "sonic/util/Function.h"

#include <functional>
#include <string>
#include <type_traits>
#include <ctime>
#include <chrono>

using namespace sonic;

int n = 0;

struct Test
{
	Test()
		: i(-1), a(nullptr)
	{
		n++;
		//std::cout << "Created empty" << std::endl;
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

	Test memberFunction(int a, double b)
	{
		Log::log(Log::INFO, this, " calling function: ", a, ", ", b);
		return { };
	}

	Test memberFunction2(int a, double b)
	{
		Log::log(Log::INFO, this, " calling function: ", a, ", ", b);
		return { };
	}

	~Test()
	{
		delete[] a;
		//std::cout << "Destroyed: " << this->i << std::endl;
		i = 0;
		n--;
	}

	int i;
	char* a;
};

Test test(int a, double b)
{
	//Log::log(Log::INFO, a, ", ", b);

	Test t1 = { };
	if (t1.a == nullptr)
		t1.i = 3;

	if (t1.i == 5)
		std::cout << "TEST" << std::endl;

	return { };
}

Test test2(int a, double b)
{
	return { };
}

int main()
{
	typedef std::chrono::high_resolution_clock Time;
	typedef std::chrono::milliseconds ms;
	typedef std::chrono::duration<float> fsec;

	Log::init("C:/dev/Sonic/Sandbox/log/test_log.log", &std::cout, Log::TRACE, Log::TRACE);

	Function<Test(int, double)> f1 = test;
	auto start1 = Time::now();

	for (int i = 0; i < 1000000; i++)
		f1(3, 4.0);

	auto end1 = Time::now();

	std::function<Test(int, double)> f = test;
	auto start = Time::now();

	for (int i = 0; i < 1000000; i++)
		f(3, 4.0);

	auto end = Time::now();

	auto d = end - start;
	auto d1 = end1 - start1;

	std::cout << "std::function duration: " << d.count() << std::endl;
	std::cout << "sonic::Function duration: " << d1.count() << std::endl;

	std::cout << "std::function took " << static_cast<double>(d.count()) / static_cast<double>(d1.count()) << " times as long as sonic::Function" << std::endl;
}
