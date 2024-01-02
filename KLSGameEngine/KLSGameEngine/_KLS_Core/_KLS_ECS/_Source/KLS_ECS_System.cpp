// include the needed header files
#include "KLS_Application.h"
#include "KLS_Logger.h"
#include "KLS_Entity.h"
#include "KLS_ECS_System.h"
#include "KLS_ECS.h"

// unclutter the global namespace
namespace KLS
{
	// class constructor / destructor (use create / cleanup instead)
	KLS_ECS_System::KLS_ECS_System(KLS_ECS* ecs) : m_ECS(ecs)
	{
	}

	KLS_ECS_System::~KLS_ECS_System()
	{
		m_ECS = nullptr;
	}

	void KLS_ECS_System::tick(const double& elapsedtime)
	{
	}

} // end namespace