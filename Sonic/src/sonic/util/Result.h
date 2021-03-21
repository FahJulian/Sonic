#pragma once

namespace sonic
{
	template<typename R, typename E>
	struct Result
	{
		Result(R result)
			: hasError(false), result(result)
		{
		}

		Result(E error)
			: hasError(true), error(error)
		{
		}

		~Result()
		{
			if (hasError)
				error.~E();
			else
				result.~R();
		}

		bool hasError;

		union
		{
			R result;
			E error;
		};
	};

} // namespace sonic
