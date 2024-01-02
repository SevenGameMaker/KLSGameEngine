// include the needed header files
#include "KLS_OpenGL_Pipeline.h"
#include "KLS_Logger.h"
#include "KLS_File.h"
#include "_KLS_Driver.h"
#include "KLS_Logger.h"
#include <glad/gl.h>

// unclutter the global namespace
namespace KLS
{
	GLenum KLS_ShaderTypeToGLShaderType(KLS_SHADER_TYPE t)
	{
		switch (t)
		{
		case KLSST_PIXEL: return GL_FRAGMENT_SHADER; break;
		case KLSST_VERTEX: return GL_VERTEX_SHADER; break;
		case KLSST_GEOM: return GL_GEOMETRY_SHADER; break;
		default: return 0;
		}
	}

	void KLS_OpenGL_Pipeline::setShaderParameters(KLS_Material& material, glm::mat4x4 worldmatrix)
	{
		// everything prior has been sent in uniform buffers
		setMat4("uWorldMatrix", worldmatrix);
		setFloat("uObjectId", (float)material.ObjectId);

		//if (material.MeshBuffer > 0) KLS_WARN("Setting meshbuffer to {}", (float)material.MeshBuffer / 100.0f);
		setFloat("uMeshBuffer", (float)material.MeshBuffer / 255.0f);

		setFloat("uHilite", material.Hilite == true ? 1.0f : 0.0f);

		if (m_Callback) m_Callback->setShaderParameters(this, material, worldmatrix);
	}

	// do nothing constructor / destructor (use initialize, create and cleanup instead)
	KLS_OpenGL_Pipeline::KLS_OpenGL_Pipeline() : KLS_Pipeline()
	{
		// log this activity
		KLS_TRACE("  KLS_OpenGL_Pipeline::KLS_OpenGL_Pipeline()");
	}

	KLS_OpenGL_Pipeline::~KLS_OpenGL_Pipeline()
	{
		// log this activity
		KLS_TRACE("  KLS_OpenGL_Pipeline::~KLS_OpenGL_Pipeline({})", getName().c_str());
	}

	// set all variables to a known value
	void KLS_OpenGL_Pipeline::initialize()
	{
		// log this activity
		KLS_TRACE("  KLS_OpenGL_Pipeline::initialize()");

		// call the base class
		KLS_Pipeline::initialize();
	}

	// dual creation allows for better error handling
	bool KLS_OpenGL_Pipeline::create(KLS_Driver* driver, std::string name)
	{
		// log this activity
		KLS_TRACE("  KLS_OpenGL_Pipeline::create({})", name.c_str());

		// call the base class
		KLS_CHECK(KLS_Pipeline::create(driver, name), "base class creation failed", KLS_FAILURE);

		m_ProgramId = glCreateProgram();

		if (m_ProgramId == 0)
		{
			fprintf(stderr, "Error creating shader program\n");
			return false;
		}

		return KLS_SUCCESS;
	}

	// dual creation allows for better error handling
	bool KLS_OpenGL_Pipeline::create(KLS_Driver* driver, std::string name,
		std::string vertexshaderfilename, std::string fragmentshaderfilename,
		KLS_ShaderCallback* callback)
	{
		// log this activity
		KLS_TRACE("  KLS_OpenGL_Pipeline::create({})", name.c_str());

		// call the base class
		KLS_CHECK(KLS_Pipeline::create(driver, name, vertexshaderfilename, fragmentshaderfilename, callback), "base class creation failed", KLS_FAILURE);

		m_ProgramId = glCreateProgram();

		if (m_ProgramId == 0)
		{
			fprintf(stderr, "Error creating shader program\n");
			return false;
		}

		addShader(KLSST_VERTEX, vertexshaderfilename);
		addShader(KLSST_PIXEL, fragmentshaderfilename);

		return postCreate();
	}

	// dual creation allows for better error handling
	bool KLS_OpenGL_Pipeline::create(KLS_Driver* driver, std::string name,
		std::string vertexshaderfilename, std::string geometryshaderfilename, std::string fragmentshaderfilename,
		KLS_ShaderCallback* callback)
	{
		// log this activity
		KLS_TRACE("  KLS_OpenGL_Pipeline::create({})", name.c_str());

		// call the base class
		KLS_CHECK(KLS_Pipeline::create(driver, name, vertexshaderfilename, geometryshaderfilename, fragmentshaderfilename, callback), "base class creation failed", KLS_FAILURE);

		m_ProgramId = glCreateProgram();

		if (m_ProgramId == 0)
		{
			fprintf(stderr, "Error creating shader program\n");
			return false;
		}

		addShader(KLSST_VERTEX, vertexshaderfilename);
		addShader(KLSST_GEOM, geometryshaderfilename);
		addShader(KLSST_PIXEL, fragmentshaderfilename);

		return postCreate();
	}

	// After all the shaders have been added to the program call this function
	// to link and validate the program.
	bool KLS_OpenGL_Pipeline::postCreate()
	{
		// log this activity
		KLS_TRACE("  KLS_OpenGL_Pipeline::PostCreate({})", getName().c_str());

		KLS_CHECK(KLS_Pipeline::postCreate(), "base class method failed", KLS_FAILURE);

		GLint Success = 0;
		GLchar ErrorLog[1024] = { 0 };

		glLinkProgram(m_ProgramId);

		glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &Success);
		if (Success == 0)
		{
			glGetProgramInfoLog(m_ProgramId, sizeof(ErrorLog), NULL, ErrorLog);
			fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
			return false;
		}

		glValidateProgram(m_ProgramId);
		glGetProgramiv(m_ProgramId, GL_VALIDATE_STATUS, &Success);
		if (!Success)
		{
			glGetProgramInfoLog(m_ProgramId, sizeof(ErrorLog), NULL, ErrorLog);
			fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
			//   return false;
		}

		glUseProgram(m_ProgramId);
		setInt("Texture0", 0);
		setInt("Texture1", 1);
		setInt("Texture2", 2);
		setInt("Texture3", 3);
		setInt("Texture4", 4);
		setInt("Texture5", 5);
		setInt("Texture6", 6);
		setInt("Texture7", 7);
		glUseProgram(0);

		// Delete the intermediate shader objects that have been added to the program
		for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
		{
			glDeleteShader(*it);
		}

		m_shaderObjList.clear();

		return KLS_SUCCESS; // GLCheckError();
	}

	// cleanup all memory issues
	bool KLS_OpenGL_Pipeline::cleanup()
	{
		// log this activity
		KLS_TRACE("  KLS_OpenGL_Pipeline::cleanup({})", getName().c_str());

		// Delete the intermediate shader objects that have been added to the program
		 // The list will only contain something if shaders were compiled but the object itself
		 // was destroyed prior to linking.
		for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
		{
			glDeleteShader(*it);
		}

		if (m_ProgramId != 0)
		{
			glDeleteProgram(m_ProgramId);
			m_ProgramId = 0;
		}

		return KLS_Pipeline::cleanup();
	}

	bool KLS_OpenGL_Pipeline::addShader(KLS_SHADER_TYPE ShaderType, std::string filepath)
	{
		KLS_TRACE("  KLS_OpenGL_Pipeline::addShader({})", filepath.c_str());
		KLS_TRACE("  {})", filepath.c_str());

		std::string s = readFile(filepath);

		GLuint ShaderObj = glCreateShader(KLS_ShaderTypeToGLShaderType(ShaderType));

		KLS_CHECK(ShaderObj, "glCreateShader failed", KLS_FAILURE);

		// Save the shader object - will be deleted in the destructor
		m_shaderObjList.push_back(ShaderObj);

		const GLchar* p[1];
		p[0] = s.c_str();
		GLint Lengths[1] = { (GLint)s.size() };

		glShaderSource(ShaderObj, 1, p, Lengths);

		glCompileShader(ShaderObj);

		GLint success;
		glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

		if (!success) {
			GLchar InfoLog[1024];
			glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
			KLS_WARN("Error compiling {} : {}", filepath.c_str(), (char*)InfoLog);
			return false;
		}

		glAttachShader(m_ProgramId, ShaderObj);

		return KLS_SUCCESS;
	}

	void KLS_OpenGL_Pipeline::bind()
	{
		glUseProgram(getProgramId());
	}

	void KLS_OpenGL_Pipeline::unbind()
	{
		glUseProgram(0);
	}

	unsigned int KLS_OpenGL_Pipeline::uniformLocation(std::string name)
	{
		//for (const auto& pair : m_UniformLocations) {
		//	KLS_WARN("{} {}",pair.first.c_str(), pair.second);
		//}

		std::unordered_map<std::string, GLint>::const_iterator got = m_UniformLocations.find(name);
		if (got != m_UniformLocations.end())
		{
			return got->second;
		}
		else
		{
			GLint location = glGetUniformLocation(getProgramId(), name.c_str());
			if (location != -1)
			{
				m_UniformLocations.emplace(name, location);
				return location;
			}
		}

		return -1;
	}

	void KLS_OpenGL_Pipeline::setInt(const std::string& name, int value)
	{
		glUniform1i(uniformLocation(name), value);
	}

	void KLS_OpenGL_Pipeline::setIntArray(const std::string& name, int* values, uint32_t count)
	{
		glUniform1iv(uniformLocation(name), count, values);
	}

	void KLS_OpenGL_Pipeline::setFloat(const std::string& name, float value)
	{
		uint32_t i = uniformLocation(name);
		glUniform1f(i, value);
	}

	void KLS_OpenGL_Pipeline::setFloat2(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(uniformLocation(name), value.x, value.y);
	}

	void KLS_OpenGL_Pipeline::setFloat3(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(uniformLocation(name), value.x, value.y, value.z);
	}

	void KLS_OpenGL_Pipeline::setFloat4(const std::string& name, const glm::vec4& value)
	{
		glUniform4f(uniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void KLS_OpenGL_Pipeline::setMat4(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(uniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

} // end namespace
