#pragma once

#ifdef KLSGAMEENGINE_EXPORTS
#define KLS_API __declspec(dllexport)
#else
#define KLS_API __declspec(dllimport)
#endif

#define UUID_TYPE int

#define KLS_DELAYLOAD
#define KLS_USE_GPU true
#define KLS_USE_DEBUGRENDER false
#define KLS_USE_PVD true
#define KLS_PHYSX_GRAVITY -32.0f
#define KLS_PHYSX_GRAVITY_CHARACTER KLS_PHYSX_GRAVITY  * 3
#define KLS_ANGULAR_DAMPENING 0.2f	// slow the object rotation down each frame by this much
#define KLS_LINEAR_DAMPENING  0.2f	// slow the object velocity down each frame by this much

// message source / destination
#define KLS_MESSAGE_APPLICATION -7
#define KLS_MESSAGE_LEVEL -77
#define KLS_MAINCHARACTER -777

// message types
#define KLS_MT_COLLISION				1
#define KLS_MT_COLLISIONLOST			2
#define KLS_MT_CHARACTER_COLLISION		3
#define KLS_MT_CHARACTER_COLLISIONLOST	4

#define KLS_SUCCESS true
#define KLS_FAILURE false
#define KLS_DEFAULT_STRING "KLS_DEFAULT"
#define KLS_IDENTITY_QUAT glm::quat(1.0f,0.0f,0.0f,0.0f)

// check methods
#define KLS_CHECK(x,y,z) if (!x) { KLS_WARN(y); return z; }
#define KLS_VK_CHECK(x,y,z) if (x != VK_SUCCESS) { KLS_WARN(y); return z; }
#define KLS_SAFE_CLEANUPANDDELETE(x) if (x) x->cleanup(); delete(x); x=nullptr;
#define KLS_SAFE_RELEASE(x) if (x) x->release(); x = nullptr;

#define ComponentSourcePathStr "../../KLSGameEngine/_KLS_Core/_KLS_ECS/_Include/_KLS_ECS_Components.h"
#define ComponentDestPathStr "_TempFiles/_KLS_ECS_Components.h"

// define the logger methods
#ifdef _DEBUG
// if we are in debug mode then log the requested items
#define KLS_LOGLEVEL spdlog::level::trace
#define KLS_LOGGING_FLAGS KLS_LOGGING_FLAGS_ALL
#define KLS_LOG_EVENT(x) ::KLS::KLS_Logger::LogEvent(x)
#define FILE_COPY_SOURCE "\\_DLLFiles\\_Debug"
#else
// if we are not in debug mode then define these out so they do nothing
#define KLS_LOGLEVEL spdlog::level::info
#define KLS_LOGGING_FLAGS KLS_LOGGING_FLAGS_NONE
#define KLS_LOG_EVENT(x) 
#define FILE_COPY_SOURCE "\\_DLLFiles\\_Release"
#endif

// logging flags to toggle what events / activites get logged through the logger
#define KLS_LOGGING_FLAG_SYSTEM		1<<1
#define KLS_LOGGING_FLAG_KEYBOARD	1<<2
#define KLS_LOGGING_FLAG_MOUSE		1<<3
#define KLS_LOGGING_FLAG_MOUSEMOVE	1<<4
#define KLS_LOGGING_FLAGS_ALL (KLS_LOGGING_FLAG_SYSTEM|KLS_LOGGING_FLAG_KEYBOARD|KLS_LOGGING_FLAG_MOUSE) //|KLS_LOGGING_FLAG_MOUSEMOVE)
#define KLS_LOGGING_FLAGS_NONE 0

// padding to make the logging look better
#define KLS_PAD_EVENTTYPE	45
#define KLS_PAD_EVENTTYPE2	KLS_PAD_EVENTTYPE + 26
#define KLS_PAD_X			77
#define KLS_PAD_Y			80

// log macros to reduce the amount of typeing we have to do
#ifdef _DEBUG
#define KLS_TRACE(...)		::KLS::KLS_Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define KLS_INFO(...)		::KLS::KLS_Logger::GetCoreLogger()->info(__VA_ARGS__)
#define KLS_WARN(...)		::KLS::KLS_Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define KLS_ERROR(...)		::KLS::KLS_Logger::GetCoreLogger()->error(__VA_ARGS__)
#define KLS_CRITICAL(...)	::KLS::KLS_Logger::GetCoreLogger()->critical(__VA_ARGS__)
#else
#define KLS_TRACE(...)		
#define KLS_INFO(...)		
#define KLS_WARN(...)		::KLS::KLS_Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define KLS_ERROR(...)		::KLS::KLS_Logger::GetCoreLogger()->error(__VA_ARGS__)
#define KLS_CRITICAL(...)	::KLS::KLS_Logger::GetCoreLogger()->critical(__VA_ARGS__)
#endif

// declare variables with setters / getters (just reducing the amount of typeing we do and makes the code cleaner)
#define KLS_VARIABLE(x,y,z) protected : x m_##y = z;
#define KLS_VARIABLE_SET(x,y,z) KLS_VARIABLE(x,y,z) public : virtual void set##y(x value) { m_##y = value;}
#define KLS_VARIABLE_GET(x,y,z) KLS_VARIABLE(x,y,z) public : virtual x get##y() { return m_##y;}
#define KLS_VARIABLE_SETGET(x,y,z) KLS_VARIABLE(x,y,z) public : virtual void set##y(x value) { m_##y = value;} virtual x get##y() { return m_##y;}

#define KLS_VARIABLE_EDITABLE(x,y,z) public : x m_##y = z;
#define KLS_VARIABLE_EDITABLE_SET(x,y,z) KLS_VARIABLE_EDITABLE(x,y,z) public : virtual void set##y(x value) { m_##y = value;}
#define KLS_VARIABLE_EDITABLE_GET(x,y,z) KLS_VARIABLE_EDITABLE(x,y,z) public : virtual x get##y() { return m_##y;}
#define KLS_VARIABLE_EDITABLE_SETGET(x,y,z) KLS_VARIABLE_EDITABLE(x,y,z) public : virtual void set##y(x value) { m_##y = value;} virtual x get##y() { return m_##y;}

// opengl specific defines
#define KLS_OPENGL_MAJOR_VERSION	4
#define KLS_OPENGL_MINOR_VERSION	1
#define KLS_OPENGL_VERSION "#version 410"
#define CHECK_GL_ERROR { GLenum error = glGetError(); if (error != GL_NO_ERROR) KLS_ERROR("OPENGL ERROR! File: {} Line: {} Error: {}", __FILE__, __LINE__, error); }
#define KLS_MAX_LIGHTS		8	//!! WARNING - must change this in all (every) shader!!
#define MAX_BONE_WEIGHTS	4
#define MAX_BONE_INFLUENCE	4

// material defines
#define KLS_MAX_TEXTURES_DIFFUSE 2
#define KLS_MAX_TEXTURES_SPECULAR KLS_MAX_TEXTURES_DIFFUSE
#define KLS_MAX_TEXTURES_NORMAL KLS_MAX_TEXTURES_DIFFUSE
#define KLS_MAX_TEXTURES_DISPLACEMENT KLS_MAX_TEXTURES_DIFFUSE
#define KLS_MAX_CUBEMAPS KLS_MAX_TEXTURES_DIFFUSE
#define KLS_MAX_REPEAT KLS_MAX_TEXTURES_DIFFUSE
#define KLS_DEFAULT_TEXTURE_NAME "default_texture"
#define SHADERPATH(x) std::string("../../../../_media/_assets/_shaders/_opengl/_default/") + std::string(x)
#define SHADERPATHPE(x) std::string("../../../../_media/_assets/_shaders/_opengl/_posteffects/") + std::string(x)

#define KLS_ID_EDITOR -77

#define KLS_FILES_ALL "All Files (*.*)\0*.*\0"
#define KLS_FILES_MESH "Mesh Files (*.obj;*.fbx;)\0*.obj;*.fbx;\0"
#define KLS_FILES_TEXTURE "Texture Files (*.png;*.jpg;*.jpeg;*.bmp)\0*.png;*.jpg;*.jpeg;*.bmp\0"
#define KLS_MESHPATH "../../../../_media/_assets/_Models/" 
#define KLS_TEXTUREPATH "../../../../_media/_assets/_Textures/"
