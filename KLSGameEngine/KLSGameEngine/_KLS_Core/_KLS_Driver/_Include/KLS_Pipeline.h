// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Material.h"

// unclutter the global namespace
namespace KLS
{
	typedef class KLS_Driver KLS_Driver;
	typedef class KLS_Pipeline KLS_Pipeline;

	class KLS_API KLS_ShaderCallback
	{
	public:
		virtual void setShaderParameters(KLS_Pipeline* pipeline, KLS_Material& material, glm::mat4x4 worldMatrix)
		{
		}
	};

	class KLS_API KLS_ShaderCallback_Default : public KLS_ShaderCallback
	{
	public:
		virtual void setShaderParameters(KLS_Pipeline* pipeline, KLS_Material& material, glm::mat4x4 worldMatrix)
		{
		}
	};

	// the pipeline class
	// responsible for loading and compiling all assocaited shaders
	// responsible for setting up the rendering pipeline for the driver
	class KLS_API KLS_Pipeline
	{
	private:
		KLS_VARIABLE_GET(KLS_Driver*, Driver, nullptr);				// pointer to the driver
		KLS_VARIABLE_SETGET(int, Id, 0);							// this pipeline id
		KLS_VARIABLE_GET(std::string, Name, KLS_DEFAULT_STRING);	// this pipeline name
		KLS_VARIABLE_GET(KLS_ShaderCallback*, Callback, nullptr);	// pointer to the callback fnction

	public:
		// do nothing constructor / destructor (use initialize, create and cleanup instead)
		KLS_Pipeline();
		virtual ~KLS_Pipeline();

		// set all variables to a known value
		virtual void initialize();

		// dual creation allows for better error handling
		virtual bool create(KLS_Driver* driver, std::string name);

		// dual creation allows for better error handling
		virtual bool create(KLS_Driver* driver, std::string name,
			std::string vertexshaderfilename, std::string fragmentshaderfilename,
			KLS_ShaderCallback* callback);

		// dual creation allows for better error handling
		virtual bool create(KLS_Driver* driver, std::string name,
			std::string vertexshaderfilename, std::string geomshaderfilename, std::string fragmentshaderfilename,
			KLS_ShaderCallback* callback);

		// final creation method
		virtual bool postCreate();

		// cleanup all memory issues
		virtual bool cleanup();

		//
		// all of these must be overriden by derived classes to provide driver specific methods
		//

		// the main method that is useful to the end user. pass the material data to the shaders uniforms
		virtual void setShaderParameters(KLS_Material& material, glm::mat4x4 worldMatrix);

		// add a shader to the technique
		virtual bool addShader(KLS_SHADER_TYPE ShaderType, std::string filepath);

		virtual void bind();
		virtual void unbind();

		// useful in opengl for setting shader uniforms (might have to rethink this for vulkan)
		virtual unsigned int uniformLocation(std::string name);
			virtual void setInt(const std::string& name, int value);
			virtual void setIntArray(const std::string& name, int* values, uint32_t count);
			virtual void setFloat(const std::string& name, float value);
			virtual void setFloat2(const std::string& name, const glm::vec2& value);
			virtual void setFloat3(const std::string& name, const glm::vec3& value);
			virtual void setFloat4(const std::string& name, const glm::vec4& value);
			virtual void setMat4(const std::string& name, const glm::mat4& value);

	};

} // end namespace
