#pragma once
#include <utility>

namespace Sonic {
	
	template<typename ResultType, typename ErrorType>
	struct Result
	{
	public:
		template<typename... Args>
		Result(Args&&... args)
			: Result(ResultType(std::forward<Args>(args)...))
		{
		}

		Result(ResultType result)
			: hasError(false), result(result)
		{
		}

		Result(ErrorType error)
			: hasError(true), error(error)
		{
		}

		~Result()
		{
			if (hasError)
				error.~ErrorType();
			else
				result.~ResultType();
		}

		bool HasError() const { return hasError; }
		ResultType& GetResult() { return result; }
		const ResultType& GetResult() const { return result; }
		ErrorType& GetError() { return error; }
		const ErrorType& GetError() const { return error; }

	private:
		bool hasError;

		union
		{
			ResultType result;
			ErrorType error;
		};
	};

}
