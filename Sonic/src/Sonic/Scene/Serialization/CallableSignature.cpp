#include "CallableSignature.h"

using namespace Sonic::Serialization;


bool CallableSignature::operator==(const CallableSignature& other) const
{
	if (m_ReturnTypeHash != other.m_ReturnTypeHash)
		return false;
	if (m_ArgTypeHashes.size() != other.m_ArgTypeHashes.size())
		return false;

	for (size_t i = 0, size = m_ArgTypeHashes.size(); i < size; i++)
	{
		if (m_ArgTypeHashes.at(i) != other.m_ArgTypeHashes.at(i))
			return false;
	}

	return true;
}
