#include <Sonic.h>

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
};

int main()
{
	Log::init("C:/dev/Sonic/Sandbox/log/test_log.log", &std::cout, Log::ALL, Log::ALL);

	StaticArray<int, 3> array = {
		3, 4, 5
	};

	array.forEach([](int i) {
		Log::log(Log::DEBUG, i);
	});

	DynamicArray<int> dynamic = {
		5, 6, 7
	};

	dynamic.forEach([](int i) {
		Log::log(Log::DEBUG, i);
	});

	Map<int, double> map = {
		{ 4, 5.0 },
		{ 3, 6.0 },
		{ 8, 189.0 }
	};

	map.forEach([](auto& pair) {
		Log::log(Log::DEBUG, pair.key, ": ", pair.value);
	});
}
