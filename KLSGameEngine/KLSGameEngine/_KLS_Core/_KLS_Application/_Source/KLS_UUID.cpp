#include "KLS_UUID.h"
#include <random>
#include <unordered_map>

namespace KLS 
{
	int uniqueId = 0;
	static std::random_device s_RandomDevice;
	static std::mt19937 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<UUID_TYPE> s_UniformDistribution;

	KLS_UUID::KLS_UUID()
		//: m_UUID(s_UniformDistribution(s_Engine))
		: m_UUID(++uniqueId)
	{
	}

	KLS_UUID::KLS_UUID(UUID_TYPE uuid)
		: m_UUID(uuid)
	{
	}

}
