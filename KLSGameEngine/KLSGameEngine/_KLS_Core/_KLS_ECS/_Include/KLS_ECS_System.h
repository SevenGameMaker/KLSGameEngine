// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

// unclutter the global namespace
namespace KLS
{
	typedef class KLS_ECS KLS_ECS;

	class KLS_API KLS_ECS_System
	{
	private:
		KLS_ECS* m_ECS = nullptr;
		bool m_Active = true;

	public:
		KLS_ECS* getECS() { return m_ECS; };

	public:
		KLS_ECS_System(KLS_ECS* ecs);
		virtual ~KLS_ECS_System();
		virtual void tick(const double& elapsedtime);
		void setActive(bool value) { m_Active = value; }
		bool isActive() { return m_Active; }
	};

} // end namespace