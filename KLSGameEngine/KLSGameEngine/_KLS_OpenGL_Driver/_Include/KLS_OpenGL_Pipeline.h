// parse this file only once
#pragma once

// include the needed header files
#include "KLS_Pipeline.h"
#include <glad/gl.h>

// unclutter the global namespace
namespace KLS
{
	class KLS_API KLS_OpenGL_Pipeline : public KLS_Pipeline
	{
	private:
		KLS_VARIABLE_SETGET(unsigned int, ProgramId, -1);
		std::unordered_map<std::string, GLint> m_UniformLocations;
		typedef std::list<GLuint> ShaderObjList;
		ShaderObjList m_shaderObjList;

	public:
		// !!!!
		// derived Pipelines should override this if they want to provide additional uniforms to the shader
		virtual void setShaderParameters(KLS_Material& material, glm::mat4x4 worldMatrix);

	public:
		// do nothing constructor / destructor (use initialize, create and cleanup instead)
		KLS_OpenGL_Pipeline();
		virtual ~KLS_OpenGL_Pipeline();

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
			std::string vertexshaderfilename, std::string geometryshaderfilename, std::string fragmentshaderfilename,
			KLS_ShaderCallback* callback);

		// final creation method
		virtual bool postCreate();

		// cleanup all memory issues
		virtual bool cleanup();

		// add a shader to the Pipeline
		virtual bool addShader(KLS_SHADER_TYPE ShaderType, std::string filepath);

		// bind / unbond the Pipeline
		virtual void bind();
		virtual void unbind();

		// get the uniform location based ont he name of the uniform
		virtual unsigned int uniformLocation(std::string name);

		// set the uniforms
		virtual void setInt(const std::string& name, int value);
		virtual void setIntArray(const std::string& name, int* values, uint32_t count);
		virtual void setFloat(const std::string& name, float value);
		virtual void setFloat2(const std::string& name, const glm::vec2& value);
		virtual void setFloat3(const std::string& name, const glm::vec3& value);
		virtual void setFloat4(const std::string& name, const glm::vec4& value);
		virtual void setMat4(const std::string& name, const glm::mat4& value);

	};

} // end namespace
