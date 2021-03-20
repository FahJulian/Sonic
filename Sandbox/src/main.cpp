#include <Sonic/util/DynamicArray.h>
#include <Sonic/util/Map.h>
#include "sonic/debug/Log.h"
#include "sonic/util/String.h"
#include "sonic/util/Function.h"

#include <functional>

using namespace sonic;

struct Test
{
	Test() { Log::log(Log::INFO, "Created ", this); }
	Test(const Test& t) { Log::log(Log::INFO, "Copied ", &t, " into new object ", this); }
	Test(Test&& t) noexcept { Log::log(Log::INFO, "Moved ", &t, " into new object ", this); }
	Test& operator=(const Test& t) { Log::log(Log::INFO, "Copied ", &t, " into ", this); }
	Test& operator=(Test&& t) noexcept { Log::log(Log::INFO, "Moved ", &t, " into ", this); return *this; }
	Test memberFunction(Test t) { return { }; }
	~Test() { Log::log(Log::INFO, "Destroyed ", this); }

	void test0(Test&& t) { }
	void test1(Test t) {}
	void test2(Test* t) {}
	void test3(Test& t) {}
	void test4(const Test& t) {}
	void test5(int a, Test&& t) { }
	void test6(int&& a, Test t) {}
	void test7(int a, Test* t) {}
	void test8(int a, Test& t) {}
	void test9(int a, const Test& t) {}
};

Test test0(Test&& t) { return { }; }
Test test1(Test t) {return { }; }
Test test2(Test* t) {return { }; }
Test test3(Test& t) {return { }; }
Test test4(const Test& t) {return { }; }
Test test5(int a, Test&& t) { return { }; }
Test test6(int&& a, Test t) {return { }; }
Test test7(int a, Test* t) {return { }; }
Test test8(int a, Test& t) {return { }; }
Test test9(int a, const Test& t) {return { }; }

int main()
{
	Log::init("C:/dev/Sonic/Sandbox/log/test_log.log", &std::cout, Log::ALL, Log::ALL);

	Test t;
	int i = 0;

	Function<Test(Test&&)> func0 = test0;
	Function<Test(Test)> func1 = test1;
	Function<Test(Test*)> func2 = test2;
	Function<Test(Test&)> func3 = test3;
	Function<Test(const Test&)> func4 = test4;
	Function<Test(int, Test&&)> func5 = test5;
	Function<Test(int&&, Test)> func6 = test6;
	Function<Test(int, Test*)> func7 = test7;
	Function<Test(int, Test&)> func8 = test8;
	Function<Test(int, const Test&)> func9 = test9;

	Function<void(Test&&)> memberfunc0 = { &t, &Test::test0 };
	Function<void(Test)> memberfunc1 = { &t, &Test::test1 };
	Function<void(Test*)> memberfunc2 = { &t, &Test::test2 };
	Function<void(Test&)> memberfunc3 = { &t, &Test::test3 };
	Function<void(const Test&)> memberfunc4 = { &t, &Test::test4 };
	Function<void(int, Test&&)> memberfunc5 = { &t, &Test::test5 };
	Function<void(int&&, Test)> memberfunc6 = { &t, &Test::test6 };
	Function<void(int, Test*)> memberfunc7 = { &t, &Test::test7 };
	Function<void(int, Test&)> memberfunc8 = { &t, &Test::test8 };
	Function<void(int, const Test&)> memberfunc9 = { &t, &Test::test9 };

	t = func0(std::move(t));
	t = func1(t);
	t = func2(&t);
	t = func3(t);
	t = func4(t);
	t = func5(i, std::move(t));
	t = func6(std::move(i), t);
	t = func7(i, &t);
	t = func8(i, t);
	t = func9(i, t);

  	memberfunc0(std::move(t));
	memberfunc1(t);
	memberfunc2(&t);
	memberfunc3(t);
	memberfunc4(t);
	memberfunc5(i, std::move(t));
	memberfunc6(std::move(i), t);
	memberfunc7(i, &t);
	memberfunc8(i, t);
	memberfunc9(i, t);
}
