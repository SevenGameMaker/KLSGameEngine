// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

// unclutter the global namespace
namespace KLS
{
	// forward declarations
	typedef class KLS_Driver KLS_Driver;

	class KLS_API KLS_Texture
	{
	private:
		KLS_VARIABLE_GET(KLS_Driver*, Driver, nullptr);				// we need to know the driver
		KLS_VARIABLE_GET(uint32_t, RendererID, 0);					// the renderer object id
		KLS_VARIABLE_GET(std::string, Name, KLS_DEFAULT_STRING);	// the name of this texture for lookup functionality
		KLS_VARIABLE_GET(int, Width, 0);							// the width of this texture
		KLS_VARIABLE_GET(int, Height, 0);							// height of this texture
		KLS_VARIABLE_GET(int, Channels, 0);							// for example 4 = RGBA, 3 = RGB
		//KLS_VARIABLE(unsigned char*, Data, nullptr);			// the actual pixel data
		KLS_VARIABLE(void*, Data, nullptr);			// the actual pixel data
		KLS_VARIABLE_GET(KLS_TextureFormat, Format, KLSTF_RGBA);	// texture format

		KLS_VARIABLE_GET(uint32_t, InternalFormat, 0);
		KLS_VARIABLE_GET(uint32_t, GPUFormat, 0);
		KLS_VARIABLE_GET(uint32_t, DataType, 0);

	public:
		// class constructor / destructor
		KLS_Texture();
		virtual ~KLS_Texture();

		// create the texture and name it 'name'
		virtual bool create(KLS_Driver* driver, std::string name, int flip);
		virtual bool create(KLS_Driver* driver, std::string name, int flip, 
			int width, int height, KLS_TextureFormat format);

		// bind / unbond the texture
		virtual void bind(uint32_t slot) = 0;
		virtual void unbind() = 0;

		unsigned char*& getCharData() { return *reinterpret_cast<unsigned char**>(&m_Data); } // the actual pixel data in char* format
		int*& getIntData() { return *reinterpret_cast<int**>(&m_Data); } // the actual pixel data in int format

		// return the number of bytes per pixel based on the texture format
		int getBytesPerPixel(KLS_TextureFormat textureType);

		// set the color fo a pixel (always assume 4 RGBA but will detect if only RGB)
		virtual void setPixel(int x, int y, glm::vec4 color);

		// get teh color of a pixel (will return RGBA even if this format is RGB, in this case the alpha will be 1.0f)
		virtual glm::vec4 getPixel(int x, int y);

		// fill the texture with a color (takes into account if RGBA or RGB)
		virtual void fill(glm::vec4 color);

	protected:
		// uploads the m_Data info to the GPU buffer
		virtual void updateData() {};
	};


} // end namespace