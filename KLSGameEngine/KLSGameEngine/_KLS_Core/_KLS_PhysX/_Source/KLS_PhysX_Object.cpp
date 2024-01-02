// include the needed header files
#include "KLS_PhysX_Object.h"
#include "KLS_Logger.h"

// unclutter the global namespace
namespace KLS
{
	// do nothing constuctor - use initialize, create and cleanup instead
	KLS_PhysX_Object::KLS_PhysX_Object()
	{
		// log this activity
		KLS_TRACE("KLS_PhysX_Object::KLS_PhysX_Object()");
	}

	// do nothing destuctor - use initialize, create and cleanup instead
	KLS_PhysX_Object::~KLS_PhysX_Object() 
	{
		// log this activity
		KLS_TRACE("KLS_PhysX_Object::~KLS_PhysX_Object()");
	}

	// set all variables to a known value
	void KLS_PhysX_Object::initialize()
	{ 
		// log this activity
		KLS_TRACE("KLS_PhysX_Object::intialize()");

		// forget this
		m_World = nullptr; 
	}

	// dual creation allows for better error handling
	bool KLS_PhysX_Object::create(KLS_PhysX_World* world, KLS_PhysX_ObjectCreationParams params)
	{ 
		// log this activity
		KLS_TRACE("KLS_PhysX_Object::create()");

		// remember this
		m_World = world; 
		m_Params = params;

		// everything went fine
		return true; 
	}

	// cleanup whatever memory mess we made (always return false from a cleanup function)
	bool KLS_PhysX_Object::cleanup() 
	{
		// log this activity
		KLS_TRACE("KLS_PhysX_Object::cleanup()");

		// forget this
		m_World = 0; 

		// always return flase from a cleanup function to allow for cascading errors
		return KLS_FAILURE; 
	}

	// called before beginscene each frame
	void KLS_PhysX_Object::preFrame(const float& elapsedtime)
	{	
	}

	// rotate the object relative to it's current rotation
	void KLS_PhysX_Object::rotate(glm::quat rotOffset)
	{ 
		setRotation(getRotation() * rotOffset); 
	}

	// get both the positiona dn the rotation of the obejct
	void KLS_PhysX_Object::getPositionAndRotation(glm::vec3& pos, glm::quat& rot)
	{ 
		pos = glm::vec3(0, 0, 0); 
		rot = glm::quat(1,0, 0, 0);
	}

	physx::PxTransform KLS_PhysX_Object::getGlobalPose() 
	{ 
		return physx::PxTransform();
	}

	glm::vec3 KLS_PhysX_Object::getRotationFromPxMat33(const physx::PxMat33& rotationMatrix)
	{
		// Get the inverse of the rotation matrix (Transpose is used because it's an orthogonal matrix)
		physx::PxMat33 inverseRotationMatrix = rotationMatrix.getInverse();

		// Get the transpose of the inverse to obtain the original rotation matrix
		physx::PxMat33 extractedRotationMatrix = inverseRotationMatrix.getTranspose();

		// Extract the rotation angles around the X, Y, and Z axes
		float rotationX = atan2f(extractedRotationMatrix.column1.z, extractedRotationMatrix.column2.z);
		float rotationY = asinf(-extractedRotationMatrix.column0.z);
		float rotationZ = atan2f(extractedRotationMatrix.column0.y, extractedRotationMatrix.column0.x);

		// Convert rotations to degrees
		float rotationX_deg = rotationX * 180.0f / physx::PxPi;
		float rotationY_deg = rotationY * 180.0f / physx::PxPi;
		float rotationZ_deg = rotationZ * 180.0f / physx::PxPi;

		return glm::vec3(rotationX_deg, rotationY_deg, rotationZ_deg);
	}

	glm::vec3 KLS_PhysX_Object::getRotationFromPxTransform(const physx::PxTransform& transform)
	{
		// Get the quaternion representing the rotation from the PxTransform
		physx::PxQuat rotationQuaternion = transform.q;

		// Convert the quaternion to a glm::quat
		glm::quat glmQuaternion(rotationQuaternion.w, rotationQuaternion.x, rotationQuaternion.y, rotationQuaternion.z);

		// Convert the quaternion to Euler angles (pitch, yaw, roll)
		glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(glmQuaternion));

		return eulerRotation;
	}

} // end namespace
