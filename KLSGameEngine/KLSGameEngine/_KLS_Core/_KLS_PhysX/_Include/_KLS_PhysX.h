// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "PxPhysicsAPI.h"
#include <cooking/PxCooking.h>

// unclutter the global namespace
namespace KLS
{
	// forward declarations

	// pvd connection string info
	struct KLS_API PvdParameters
	{
		std::string						ip;
		physx::PxU32					port;
		physx::PxU32					timeout;
		bool							useFullPvdConnection;
		PvdParameters() : port(5425), timeout(10), useFullPvdConnection(true) { ip = "127.0.0.1"; }
	};

	// a wrapper for the material structure (allows us to create at program start and then retrieve as needed furing runtime)
	class KLS_API KLS_PhysXMaterial
	{
	public:
		int					m_Id;		 // simple id to find this material with
		physx::PxMaterial*	m_Material;	 // the actual PxMaterial
		KLS_PhysXMaterial(physx::PxMaterial* mat, int id) { m_Material = mat; m_Id = id; }
	};

	/*
	*/
	class KLS_API KLS_PhysX : public physx::PxDeletionListener, public physx::PxErrorCallback
	{
		// class variables
	private:
		KLS_VARIABLE_GET(physx::PxFoundation*, Foundation, nullptr);
		KLS_VARIABLE_GET(physx::PxPhysics*, Physics, nullptr);
		KLS_VARIABLE_GET(physx::PxDefaultCpuDispatcher*, CpuDispatcher, nullptr);
		KLS_VARIABLE_GET(physx::PxPvd*, Pvd, nullptr);
		KLS_VARIABLE_GET(physx::PxPvdTransport*, Transport, nullptr);

		physx::PxDefaultAllocator			m_DefaultAllocator;
		physx::PxPvdInstrumentationFlags	m_PvdFlags;
		PvdParameters						m_PvdParams;

		KLS_VARIABLE_GET(bool, SupportGpuPhysX, true);
		KLS_VARIABLE_GET(bool, UsePVD, true);

		// CUDA related variables
		KLS_VARIABLE_GET(bool, GPUEnabled, false);
		KLS_VARIABLE_GET(bool, CreateCudaCtxManager, true);
		KLS_VARIABLE_GET(physx::PxCudaContextManager*, CudaContextManager, nullptr);

		// array of materias we can access at runtime
		std::vector<KLS_PhysXMaterial*>	m_Materials;

		// must be set at create method to be useful
		KLS_VARIABLE_GET(bool, UseDebugInfo, false);

		// do nothing class constructor / destructor [use initialize(), create() and cleanup() instead]
	public:
		KLS_PhysX();
		virtual ~KLS_PhysX();

		// class creation / destruction methods
	public:
		// set all variables to a known value
		virtual void initialize();

		// dual creation allows for better error handling
		virtual bool create(bool gpuenabled, bool usedebuginfo, bool usepvd);

		// cleanup whatever memory mess we made
		virtual bool cleanup();

		// exposed class methods
	public:
		// get a material from our array
		physx::PxMaterial* getDefaultMaterial(int id);

		// add a material to the array
		int addDefaultMaterial(physx::PxReal staticFriction, physx::PxReal dynamicFriction, physx::PxReal restitution);

		// private class methods
	private:
		virtual	bool createCuda();

		// listener
		virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);
		virtual	void onRelease(const physx::PxBase* observed, void* userData, physx::PxDeletionEventFlag::Enum deletionEvent);

		// create the pvd copnnection to visualize physx seperately
		void createPvdConnection();
	};

} // end namespace
