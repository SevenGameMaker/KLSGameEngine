// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_KeyCodes.h"

// unclutter the global namespace
namespace KLS
{
	class KLS_UUID
	{
	public:
		KLS_UUID();
		KLS_UUID(UUID_TYPE uuid);
		KLS_UUID(const KLS_UUID&) = default;
		KLS_UUID(KLS_UUID&) = default;

		operator UUID_TYPE() const { return m_UUID; }
	private:
		UUID_TYPE m_UUID;
	};

}

namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<KLS::KLS_UUID>
	{
		std::size_t operator()(const KLS::KLS_UUID& uuid) const
		{
			return (UUID_TYPE)uuid;
		}
	};

}
