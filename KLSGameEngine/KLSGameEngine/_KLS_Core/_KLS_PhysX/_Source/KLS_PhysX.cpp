// include the needed header files
#include "_KLS_PhysX.h"
#include "KLS_Logger.h"
#include "KLS_Application.h"
#include "KLS_Level.h"
#include "KLS_PhysX_World.h"

// unclutter the global namespace
namespace KLS
{
	void KLS_PhysX::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
	{
		KLS_ERROR("PhysX : {}", message);
	}

	// global variable
	physx::PxDefaultAllocator gDefaultAllocatorCallback;

	// do nothing class constructor / destructor [use initialize(), create() and cleanup() instead]
	KLS_PhysX::KLS_PhysX()
	{
		// log this activity
		KLS_TRACE("KLS_PhysX::KLS_PhysX()");
	}

	KLS_PhysX::~KLS_PhysX()
	{
		// log this activity
		KLS_TRACE("KLS_PhysX::~KLS_PhysX()");
	}

	// set all variables to a known value
	void KLS_PhysX::initialize()
	{
		// log this activity
		KLS_TRACE("KLS_PhysX::initialize()");

		// set all variables to a known value
		m_Foundation = 0;
		m_Physics = 0;
		m_CpuDispatcher = 0;
		m_Pvd = 0;
		m_Transport = 0;
		m_GPUEnabled = true;
		m_CreateCudaCtxManager = true;
		m_CudaContextManager = 0;
	}

	// dual creation allows for better error handling
	// calls postCreate() for final creation
	bool KLS_PhysX::create(bool gpuenabled, bool usedebuginfo, bool usepvd)
	{
		// log this activity
		KLS_INFO("--------------------------");
		KLS_INFO("KLS_PhysX::create()");

		// remember this
		m_GPUEnabled = gpuenabled;
		m_UseDebugInfo = usedebuginfo;
		m_UsePVD = usepvd;

		// create the foundation
		physx::PxAllocatorCallback* allocator = &gDefaultAllocatorCallback;
		m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *allocator, *this);
		if (!m_Foundation)
		{
			// if creation failed, lte the user know and then bail
			KLS_ERROR("KLS_PhysX::create() Foundation not valid");
			return false;
		}

		// create the cuda connection
		createCuda();

		// connect the pvd
		createPvdConnection();

		// set up our scale factor
		physx::PxTolerancesScale scale;

		// create the main physx structure
		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, scale, false, m_Pvd);
		if (!m_Physics)
		{
			// if creation failed, lte the user know and then bail
			KLS_ERROR("KLS_PhysX::create() m_Physics not valid");
			return false;
		}

		// initialize the extensions
		if (!PxInitExtensions(*m_Physics, m_Pvd))
		{
			// if creation failed, lte the user know and then bail
			KLS_ERROR("KLS_PhysX::create() initExtensions not valid");
			return false;
		}

		// register for deletion events
		m_Physics->registerDeletionListener(*this, physx::PxDeletionEventFlag::eUSER_RELEASE);

		// create the default material
		addDefaultMaterial(0.5f, 0.5f, 0.0f);

		// create a cpu dispatcher for scenes
		m_CpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
		if (!m_CpuDispatcher)
		{
			// if creation failed, lte the user know and then bail
			KLS_ERROR("KLS_PhysX::create() m_CpuDispatcher not valid");
			return false;
		}

		// log this activity
		KLS_INFO("KLS_PhysX::created successfully");
		KLS_INFO("--------------------------------------");

		// everything went fine
		return KLS_SUCCESS;
	}

	// cleanup whatever memory mess we made
	bool KLS_PhysX::cleanup()
	{
		// log this activity
		KLS_TRACE("KLS_PhysX::cleanup()");

		// safely shutdown physx
		KLS_SAFE_RELEASE(m_CpuDispatcher);
		PxCloseExtensions();
		KLS_SAFE_RELEASE(m_Physics);
		KLS_SAFE_RELEASE(m_Pvd);
		KLS_SAFE_RELEASE(m_Transport);
		KLS_SAFE_RELEASE(m_Foundation);

		// always return false from a cleanup function to allow for cascading errors
		return KLS_FAILURE;
	}

	physx::PxMaterial* KLS_PhysX::getDefaultMaterial(int id)
	{
		if (id > (int)m_Materials.size() - 1) id = 1;
		physx::PxMaterial* mat = m_Materials[id]->m_Material;
		if (!mat) getPhysics()->getMaterials(&mat, 1);

		return mat;
	}

	int KLS_PhysX::addDefaultMaterial(physx::PxReal staticFriction, physx::PxReal dynamicFriction, physx::PxReal restitution)
	{
		m_Materials.push_back(new KLS_PhysXMaterial(getPhysics()->createMaterial(staticFriction, dynamicFriction, restitution), (int)m_Materials.size()));
		return (int)m_Materials.size();
	}

	void KLS_PhysX::onRelease(const physx::PxBase* observed, void* userData, physx::PxDeletionEventFlag::Enum deletionEvent)
	{
		// log this activity
		KLS_TRACE("KLS_PhysX::onRelease()");

		PX_UNUSED(userData);
		PX_UNUSED(deletionEvent);

		// do what you want with this actor before is it fully destroyed
		const physx::PxRigidActor* actor = static_cast<const physx::PxRigidActor*>(observed);
	}

	void KLS_PhysX::createPvdConnection()
	{
		// log this activity
		KLS_INFO("KLS_PhysX::createPVDConnection()");

		if (!m_UsePVD)
		{
			KLS_WARN("UsePVD is false. No PVD connection will be available");
			return;
		}

		//Create a pvd connection that writes data straight to the filesystem.  This is
		//the fastest connection on windows for various reasons.  First, the transport is quite fast as
		//pvd writes data in blocks and filesystems work well with that abstraction.
		//Second, you don't have the PVD application parsing data and using CPU and memory bandwidth
		//while your application is running.
		//physx::PxPvdTransport* transport = physx::PxDefaultPvdFileTransportCreate( "c:\\mywork\\sample.pxd2" );

		//The normal way to connect to pvd.  PVD needs to be running at the time this function is called.
		//We don't worry about the return value because we are already registered as a listener for connections
		//and thus our onPvdConnected call will take care of setting up our basic connection state.
		m_Transport = physx::PxDefaultPvdSocketTransportCreate(m_PvdParams.ip.c_str(), m_PvdParams.port, m_PvdParams.timeout);
		if (m_Transport == NULL)
		{
			// if creation failed, lte the user know and then bail
			KLS_WARN("KLS_PhysX::createPvdConnection() m_Transport not valid");
			return;
		}

		//The connection flags state overall what data is to be sent to PVD.  Currently
		//the Debug connection flag requires support from the implementation (don't send
		//the data when debug isn't set) but the other two flags, profile and memory
		//are taken care of by the PVD SDK.

		//Use these flags for a clean profile trace with minimal overhead
		m_PvdFlags = physx::PxPvdInstrumentationFlag::eALL;
		if (!m_PvdParams.useFullPvdConnection)
		{
			m_PvdFlags = physx::PxPvdInstrumentationFlag::ePROFILE;
		}

		// create the Pvd 
		m_Pvd = physx::PxCreatePvd(*m_Foundation);

		// conenct if we have a valid Pvd
		if (m_Pvd)
		{
			m_Pvd->connect(*m_Transport, m_PvdFlags);
			KLS_INFO("  PVD connection will be available");
		}
		else
			KLS_WARN("Pvd->connect() failed. No PVD connection will be available");
	}

	bool KLS_PhysX::createCuda()
	{
		// log this activity
		KLS_INFO("KLS_PhysX::createCUDA()");

		// assume failure
		m_GPUEnabled = false;

		if (m_SupportGpuPhysX)
		{
			KLS_INFO("  attempting to createCudaContext");
			physx::PxCudaContextManagerDesc cudaContextManagerDesc;
			m_CudaContextManager = PxCreateCudaContextManager(*getFoundation(), cudaContextManagerDesc, PxGetProfilerCallback());
			if (m_CudaContextManager)
			{
				if (!m_CudaContextManager->contextIsValid())
				{
					KLS_WARN("  m_CudaContextManager created but is not valid!!");
					KLS_WARN("  are you using a Cuda enabled graphics driver?");
					m_CudaContextManager->release();
					m_CudaContextManager = NULL;
				}
				else
				{
					KLS_INFO("  Cuda creation successful. PhysX will be done on the GPU");
					m_GPUEnabled = true;
					return true;
				}
			}
			else
			{
				KLS_WARN("  unable to create the Cuda Context Manager");
				KLS_WARN("  are you using a Cuda enabled graphics driver?");
				return false;
			}
		}
		else
		{
			KLS_WARN("KLS_SUPPORT_GPU_PHYSX is not defined. No Cuda context creation is available");
			KLS_WARN("PhysX will be done on the CPU not the GPU");
		}

		// something went wrong
		return false;
	}
} // end namespace
