// include the needed header files
#include "KLS_OpenGL_Texture.h"
#include "KLS_Logger.h"
#include <glad/gl.h>

// unclutter the global namespace
namespace KLS
{
	void KLS_OpenGL_Texture::setFormatInfo(KLS_TextureFormat format)
	{
		switch (m_Format)
		{
		case KLSTF_RG16F:
			m_InternalFormat = GL_RG16F;
			m_GPUFormat = GL_RG;
			m_DataType = GL_FLOAT;
			break;
		case KLSTF_RGB:
			m_InternalFormat = GL_RGB;
			m_GPUFormat = GL_RGB;
			m_DataType = GL_UNSIGNED_BYTE;
			break;
		case KLSTF_RGBA:
			m_InternalFormat = GL_RGBA;
			m_GPUFormat = GL_RGBA;
			m_DataType = GL_UNSIGNED_BYTE;
			break;
		case KLSTF_DEPTH: // For depth texture
			m_InternalFormat = GL_DEPTH_COMPONENT;
			m_GPUFormat = GL_DEPTH_COMPONENT;
			m_DataType = GL_FLOAT; // Adjust this if you use a different precision for depth values
			break;
		}
	}

	// class constructor / destructor
	KLS_OpenGL_Texture::KLS_OpenGL_Texture() : KLS_Texture()
	{
		// log this activity
		KLS_TRACE("  KLS_OpenGL_Texture::KLS_OpenGL_Texture()");
	}

	KLS_OpenGL_Texture::~KLS_OpenGL_Texture()
	{
		// log this activity
		KLS_TRACE("  KLS_OpenGL_Texture::~KLS_OpenGL_Texture({})", getName().c_str());

		glDeleteTextures(1, &m_RendererID);
	}

	bool KLS_OpenGL_Texture::create(KLS_Driver* driver, std::string name, int flip)
	{
		KLS_TRACE("  KLS_OpenGL_Texture::create({})", name.c_str());

		if (!KLS_Texture::create(driver, name, flip)) return KLS_FAILURE;

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		switch (m_Channels)
		{
		case 3: m_Format = KLSTF_RGB; break;
		case 4: m_Format = KLSTF_RGBA; break;
		default:
		{
			m_Format = KLSTF_RGBA;
		} break;
		}

		// Upload the image data to the texture
		setFormatInfo(m_Format);
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_GPUFormat, m_DataType, m_Data);

		// Generate mipmaps (optional, but recommended)
		glGenerateMipmap(GL_TEXTURE_2D);

		return KLS_SUCCESS;
	}

	bool KLS_OpenGL_Texture::create(KLS_Driver* driver, std::string name, int flip,
		int width, int height, KLS_TextureFormat format)
	{
		// log this activity
		KLS_TRACE("  KLS_OpenGL_Texture::create({} {} {})", name.c_str(), width, height);

		KLS_CHECK(KLS_Texture::create(driver, name, flip, width, height, format), "base class creation failed", KLS_FAILURE);

		// Generate a texture ID
		glGenTextures(1, &m_RendererID);

		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		switch (m_Format)
		{
		case KLSTF_NONE:
		case KLSTF_RG16F:
		case KLSTF_RGB:
		case KLSTF_RGBA:
		case KLSTF_RGBA32F:
		{
			// Create a buffer to store pixel data
			m_Data = new unsigned char[width * height * getBytesPerPixel(format)];
		} break;
		case RED_INTEGER:
		{
			// Create a buffer to store int data
			m_Data = new int[width * height * getBytesPerPixel(format)];
		} break;
		case KLSTF_DEPTH:
		{
		} break;
		}

		// Load the pixel data into the texture
		setFormatInfo(m_Format);
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_GPUFormat, m_DataType, m_Data);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return KLS_SUCCESS;
	}

	void KLS_OpenGL_Texture::bind(uint32_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void KLS_OpenGL_Texture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void KLS_OpenGL_Texture::updateData()
	{
		KLS_Texture::updateData();

		// Bind the texture and update the pixel data
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_GPUFormat, m_DataType, m_Data);
	}

} // end namespace