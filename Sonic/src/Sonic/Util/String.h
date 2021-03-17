#pragma once


namespace Sonic
{
	class String
	{
	public:
		String()
			: mData(nullptr)
		{

		}

		String(const char* data)
			: mData(data)
		{
		}

		const char* getData() const
		{
			return mData;
		}

	private:
		const char* mData;
	};
}
