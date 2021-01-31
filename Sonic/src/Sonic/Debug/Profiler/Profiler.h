#pragma once
#include <iostream>
#include "Sonic/Window/Window.h"

//#define SONIC_PROFILE

namespace Sonic
{
	class Profiler
	{
	private:
		struct Function
		{
			int runs;
			double duration;
		};

	public:
		class FunctionProfiler
		{
			FunctionProfiler(Function& function)
				: function(function), startTime(Window::getTime())
			{
			}

		public:
			~FunctionProfiler()
			{
				function.runs++;
				function.duration += Window::getTime() - startTime;
			}

		private:
			Function& function;
			double startTime;

			friend class Profiler;
		};

	private:
		Profiler()
		{
		}

		static Profiler* get()
		{
			static Profiler profiler;
			return &profiler;
		}

		~Profiler()
		{
			std::cout << "Profiler results:" << std::endl << std::endl;

			for (auto& [name, function] : functions)
			{
				std::cout << "Function " << name << "():" << std::endl;
				std::cout << "    Ran: " << function.runs << (function.runs == 1 ? " time" : " times") << std::endl;
				std::cout << "    Total duration: " << function.duration * 1000 << "ms" << std::endl;
				std::cout << "    Average duration: " << function.duration / function.runs * 1000 << "ms" << std::endl;
				std::cout << std::endl;
			}
		}

	public:
		static FunctionProfiler CreateFunctionProfiler(const std::string& functionName)
		{
			return FunctionProfiler(get()->functions[functionName]);
		}

	private:

		std::unordered_map<std::string, Function> functions;
	};
}

#ifdef SONIC_PROFILE
#define SONIC_PROFILE_FUNCTION(functionName) Sonic::Profiler::FunctionProfiler __functionProfiler = Sonic::Profiler::CreateFunctionProfiler(functionName)
#else
#define SONIC_PROFILE_FUNCTION(functionName)
#endif
