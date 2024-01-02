// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Texture.h"
#include <glad/gl.h>

// unclutter the global namespace
namespace KLS
{
	class KLS_API KLS_OpenGL_Texture : public KLS_Texture
	{
	private:

	public:
		// class constructor / destructor
		KLS_OpenGL_Texture();
		virtual ~KLS_OpenGL_Texture();

		// create the texture and name it 'name'
		virtual bool create(KLS_Driver* driver, std::string name, int flip);
		virtual bool create(KLS_Driver* driver, std::string name, int flip, 
			int width, int height, KLS_TextureFormat format);

		// bind / unbond the texture
		virtual void bind(uint32_t slot);
		virtual void unbind();

	protected:
		// uploads the m_Data info to the GPU buffer
		virtual void updateData();

		virtual void setFormatInfo(KLS_TextureFormat format);
	};

} // end namespace