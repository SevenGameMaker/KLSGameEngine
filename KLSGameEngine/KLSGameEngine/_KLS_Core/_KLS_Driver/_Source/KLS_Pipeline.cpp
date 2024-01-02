// include the needed header files
#include "KLS_Pipeline.h"
#include "KLS_Logger.h"
#include "KLS_File.h"
#include "_KLS_Driver.h"

// unclutter the global namespace
namespace KLS
{
	// default method does nothing since this requires driver specific code
	void KLS_Pipeline::setShaderParameters(KLS_Material& material, glm::mat4x4 worldmatrix)
	{
	}

	// do nothing constructor / destructor (use initialize, create and cleanup instead)
	KLS_Pipeline::KLS_Pipeline()
	{
		// log this activity
		KLS_TRACE("  KLS_Pipeline::KLS_Pipeline()");
	}

	KLS_Pipeline::~KLS_Pipeline()
	{
		// log this activity
		KLS_TRACE("  KLS_Pipeline::~KLS_Pipeline({})", getName().c_str());
	}

	// set all variables to a known value
	void KLS_Pipeline::initialize()
	{
		// log this activity
		KLS_TRACE("  KLS_Pipeline::initialize()");

		m_Driver = nullptr;
	}

	// dual creation allows for better error handling
	bool KLS_Pipeline::create(KLS_Driver* driver, std::string name)
	{
		// log this activity
		KLS_TRACE("  KLS_Pipeline::create({})", getName().c_str());

		m_Driver = driver;
		m_Name = name;

		return KLS_SUCCESS;
	}

	// dual creation allows for better error handling
	bool KLS_Pipeline::create(KLS_Driver* driver, std::string name,
		std::string vertexshaderfilename, std::string fragmentshaderfilename,
		KLS_ShaderCallback* callback)
	{
		// log this activity
		KLS_TRACE("  KLS_Pipeline::create({})", getName().c_str());

		// rememebr these
		m_Driver = driver;
		m_Name = name;
		m_Callback = callback;

		// everything went fine
		return KLS_SUCCESS;
	}

	// dual creation allows for better error handling
	bool KLS_Pipeline::create(KLS_Driver* driver, std::string name,
		std::string vertexshaderfilename, std::string geomshaderfilename, std::string fragmentshaderfilename,
		KLS_ShaderCallback* callback)
	{
		// log this activity
		KLS_TRACE("  KLS_Pipeline::create({})", getName().c_str());

		// rememebr these
		m_Driver = driver;
		m_Name = name;
		m_Callback = callback;

		// everything went fine
		return KLS_SUCCESS;
	}

	// After all the shaders have been added to the program call this function
	// to link and validate the program.
	bool KLS_Pipeline::postCreate()
	{
		// log this activity
		KLS_TRACE("  KLS_Pipeline::postCreate({})", getName().c_str());

		return KLS_SUCCESS;
	}

	// cleanup all memory issues
	bool KLS_Pipeline::cleanup()
	{
		// log this activity
		KLS_TRACE("  KLS_Pipeline::cleanup({})", getName().c_str());

		// safely delete the callback
		if (m_Callback) delete(m_Callback); m_Callback = nullptr;

		// forget these
		m_Driver = nullptr;

		return KLS_FAILURE;
	}

	bool KLS_Pipeline::addShader(KLS_SHADER_TYPE ShaderType, std::string filepath)
	{
		KLS_TRACE("KLS_Pipeline::addShader({})", filepath.c_str());

		return KLS_SUCCESS;
	}

	void KLS_Pipeline::bind()
	{
	}

	void KLS_Pipeline::unbind()
	{
	}

	unsigned int KLS_Pipeline::uniformLocation(std::string name)
	{
		return -1;
	}

	void KLS_Pipeline::setInt(const std::string& name, int value)
	{
	}

	void KLS_Pipeline::setIntArray(const std::string& name, int* values, uint32_t count)
	{
	}

	void KLS_Pipeline::setFloat(const std::string& name, float value)
	{
	}

	void KLS_Pipeline::setFloat2(const std::string& name, const glm::vec2& value)
	{
	}

	void KLS_Pipeline::setFloat3(const std::string& name, const glm::vec3& value)
	{
	}

	void KLS_Pipeline::setFloat4(const std::string& name, const glm::vec4& value)
	{
	}

	void KLS_Pipeline::setMat4(const std::string& name, const glm::mat4& value)
	{
	}


} // end namespace
