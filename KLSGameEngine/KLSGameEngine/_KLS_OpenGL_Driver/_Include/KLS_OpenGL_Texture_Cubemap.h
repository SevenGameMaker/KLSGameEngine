// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Texture_Cubemap.h"

// unclutter the global namespace
namespace KLS
{
	// forward declarations

	class KLS_API KLS_OpenGL_Texture_Cubemap : public KLS_Texture_Cubemap
	{
	private:
	public:
		// class constructor / destructor
		KLS_OpenGL_Texture_Cubemap();
		virtual ~KLS_OpenGL_Texture_Cubemap();

		// create acubemap (this class calls the base class to store the info first) then provides the functionality)
		virtual bool create(KLS_Driver* driver, std::string name, int flip,
			std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back);

		// bind / unbind the cubemap
		virtual void bind(uint32_t slot);
		virtual void unbind();
	};


} // end namespace