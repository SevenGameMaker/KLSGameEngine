// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Texture.h"

// unclutter the global namespace
namespace KLS
{
	// forward declarations

	class KLS_API KLS_Texture_Cubemap : public KLS_Texture
	{
	private:
		// 6 individual textures make up a cubemap
		KLS_VARIABLE_GET(std::string, Right, KLS_DEFAULT_STRING);
		KLS_VARIABLE_GET(std::string, Left, KLS_DEFAULT_STRING);
		KLS_VARIABLE_GET(std::string, Top, KLS_DEFAULT_STRING);
		KLS_VARIABLE_GET(std::string, Bottom, KLS_DEFAULT_STRING);
		KLS_VARIABLE_GET(std::string, Front, KLS_DEFAULT_STRING);
		KLS_VARIABLE_GET(std::string, Back, KLS_DEFAULT_STRING);

	public:
		// class constructor / destructor
		KLS_Texture_Cubemap() {}
		virtual ~KLS_Texture_Cubemap() {}

		// crate acubemap (this base class only stores the info, derived classes must provide the functionality)
		virtual bool create(KLS_Driver* driver, std::string name, int flip,
			std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back)
		{
			KLS_Texture::create(driver, name, flip);
			m_Right = right; m_Left = left; m_Top = top; m_Bottom = bottom; m_Front = front; m_Back = back;
			return KLS_SUCCESS;
		}

		// bind / unbind the cubemap
		virtual void bind(uint32_t slot) = 0;
		virtual void unbind() = 0;
	};


} // end namespace